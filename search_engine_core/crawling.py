import requests
from bs4 import BeautifulSoup, SoupStrainer
from pymongo import MongoClient 
from nltk.tokenize import RegexpTokenizer
#
# import hashlib 
#
import pymongo
import langid

import asyncio
import re 

#
# import os
#

tokenizer = RegexpTokenizer(r'\w+|$[0-9.]+') 

async def crawling():
    base_url = "https://en.wikipedia.org"
    visited_urls = set()
    urls = ["https://en.wikipedia.org/wiki/Main_Page"]
    depth_limit = 1  # Depth limit for the crawler

    while len(urls) != 0 and depth_limit > 0:
        current_url = urls.pop()

        if current_url not in visited_urls:
            try:
                response = requests.get(current_url) # Sends an HTTP GET request to `current_url` 
                response.raise_for_status()  # Raise an exception for HTTP errors

                #
                # cached_content = load_cached_content(current_url)
                # if cached_content:
                #     visited_urls.add(current_url)
                #     depth_limit -= 1
                #     continue
                #

                my_parser = BeautifulSoup(response.content, "html.parser") # To parse the HTML content

                link_elements = my_parser.select("a[href]")  # All `<a>` elements (links) with an `href` attribute within the parsed HTML content

                for link in link_elements:
                    url = link['href']  # Extracts the value of the `href` attribute from the current `<a>` element and assigns it to `url`.

                    if url.startswith("/wiki/") and ":" not in url:
                        url = base_url + url

                        if url not in urls:
                            urls.append(url)  # Appends the modified URL to the `urls` list

                visited_urls.add(current_url)  # Adds the `current_url` to the `visited_urls` set to mark it as visited.
                depth_limit -= 1

            except requests.RequestException as e:
                print(f"Error fetching URL {current_url}: {e}")
    return urls

async def fetch_url_content(url):
    try:
        lang = None

        response = requests.get(url)
        response.raise_for_status() 
        strainer = SoupStrainer(['h1', 'h2', 'h3', 'h4', 'p']) # span
        soup = BeautifulSoup(response.content, "html.parser", parse_only=strainer)
        # Extract text from the parsed HTML
        text_content = soup.get_text().lower()
        
        lang = detect_language(text_content)

        text_content = tokenizer.tokenize(text_content)
        # Remove all blank lines
        # text_content = '\n'.join([tag.get_text() for tag in soup.find_all(['h1', 'h2', 'p', 'span'])])
        return text_content, lang

    except requests.RequestException as e:
        print(f"Error fetching URL {url}: {e}")
        return None, None
    
def detect_language(text):
    return langid.classify(text)[0]

def save_to_mongodb(data):
    client = MongoClient("mongodb://localhost:27017/")
    db = client["search-engine"]
    collection = db["contents"]

    collection.delete_many({}) 

    collection.insert_many(data)


#
# def load_cached_content(url):
#     cache_folder = 'cache'
#     os.makedirs(cache_folder, exist_ok=True)
#     cache_file = os.path.join(cache_folder, hashlib.md5(url.encode()).hexdigest())

#     if os.path.exists(cache_file):
#         with open(cache_file, 'r') as f:
#             return f.read()

#     return None
#

#
# def cache_content(url, content):
#     cache_folder = 'cache'
#     os.makedirs(cache_folder, exist_ok=True)
#     cache_file = os.path.join(cache_folder, hashlib.md5(url.encode()).hexdigest())

#     with open(cache_file, 'w') as f:
#         f.write(content)
#


async def main(): 
    visited_urls = await crawling()
    print("Size of visited_urls map:", len(visited_urls))
    data = []
    for url in visited_urls:
        content, lang = await fetch_url_content(url)
        if content:
            data.append({"url": url, "content": content, "language": lang})
            #
            # cache_content(url, '\n'.join(content))
            #
    save_to_mongodb(data)
    print("Data saved to MongoDB successfully.")

if __name__ == "__main__":
    # test_urls = crawling()
    # print(test_urls[100])
    # print(fetch_url_content(test_urls[100]))

    # main()
    asyncio.run(main())