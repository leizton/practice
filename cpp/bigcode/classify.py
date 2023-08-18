from transformers import AutoModelForCausalLM, AutoTokenizer
from transformers import AutoModelForSequenceClassification, pipeline
import torch

checkpoint = "/Users/awk/sky/coder/roberta-base-finetuned-chinanews-chinese"
device = "cpu"

tokenizer = AutoTokenizer.from_pretrained(checkpoint)
model = AutoModelForSequenceClassification.from_pretrained(checkpoint)
text_classification = pipeline('sentiment-analysis', model=model, tokenizer=tokenizer)

prompt = '8月16日,外交部发言人汪文斌主持例行记者会称中国经济持续恢复，总体回升向好，依然是世界经济增长的重要引擎'
ans = text_classification(prompt)
print('ans: %s' % ans)
