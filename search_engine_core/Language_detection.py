import pymongo
import langid

# Connect to MongoDB
client = pymongo.MongoClient("mongodb://localhost:27017/")
db = client["search-engine"]
collection = db["contents"]

# Retrieve all documents from the collection
cursor = collection.find({})

# Initialize an empty dictionary to store content
content_dict = []

# Iterate over the documents and populate the dictionary
for document in cursor:
    content_dict.append(document["content"])


def detect_language(text):
    return langid.classify(text)[0]
    
# Function to detect language for each content
def language_decision(contents):
    result = []
    lang = None
    for  content in contents:
        if isinstance(content, str):  # Check if content is a string
            lang = detect_language(content)
            result.append(lang)
        else:
            result.append(None)
    return result

# Detect language for each content
language_results = language_decision(content_dict)

# Print the language results
#print(language_results)

def add_to_mongodb(language_results):

    client = pymongo.MongoClient("mongodb://localhost:27017/")
    db = client["search-engine"]
    collection = db["contents"]
    cursor = collection.find({})
    count = 0
    try:
        for  document in cursor:
            lang = language_results[count]
            count += 1
            collection.update_one({"_id": document["_id"]}, {"$set": {"lang": lang}})
    except Exception as e:
        print("An error occurred:", e)
    else:
        print("Information about language successfully written to MongoDB.")


add_to_mongodb(language_results)






#def language_decision(data):
   # result = []
    
    #for key, words in data.items():
     #   detected_lang = detect_language(next(iter(words))) 
      #  for word in words:
       #     if detect_language(word) != detected_lang:
        #        detected_lang = "mix"
         #       break 
        #result.append(detected_lang)
    
    #return result

#def language_decision(data):
   # result = []
    #detected_lang = None
    
    #for key, words in data.items():
     #   count = 0
      #  if  detected_lang:  # Check if detected_lang is not empty (or not None)
       #     result.append(detected_lang)

        #for word in words:
         #   if count == 0:
          #      detected_lang = detect_language(word)
           #     count += 1
            #    continue    
           # else:
            #    if detected_lang != detect_language(word):
             #       detected_lang = "mix"
              #  count += 1
               

    #result.append(detected_lang)
    
    #return result

