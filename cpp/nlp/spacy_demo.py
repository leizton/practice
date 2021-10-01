import spacy
from spacy import displacy

nlp = spacy.load("en_core_web_sm")
doc = nlp("We present a novel generative model for natural language tree structures in which semantic structures are scored with separate models")
doc = nlp("he says that you like books")

displacy.serve(doc, style='dep', options={'compact':True})