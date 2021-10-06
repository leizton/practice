import spacy
from spacy import displacy

nlp = spacy.load("en_core_web_sm")
doc = nlp("We present a novel generative model for natural language tree structures in which semantic structures are scored with separate models")
doc = nlp("he says that you like books")
doc = nlp("The assumption that complements are generated independently of one another often leads to incorrect parses")

displacy.serve(doc, style='dep', options={'compact':True})