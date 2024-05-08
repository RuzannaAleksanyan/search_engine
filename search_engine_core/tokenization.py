from nltk.tokenize import RegexpTokenizer  


tokenizer = RegexpTokenizer(r'\w+|$[0-9.]+|\S+') 

sentence = 'Thomas Jefferson began building Monticello at the age of 26.'
print(tokenizer.tokenize(sentence))