from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

#checkpoint = "bigcode/starcoder"
checkpoint = "/Users/awk/sky/coder/Llama2-Chinese-7b-Chat"
device = "cpu"

tokenizer = AutoTokenizer.from_pretrained(checkpoint, use_fast=False)
# to save memory consider using fp16 or bf16 by specifying torch_dtype=torch.float16 for example
model = AutoModelForCausalLM.from_pretrained(checkpoint, torch_dtype=torch.bfloat16).to(device)

prompt = '"8月16日,外交部发言人汪文斌主持例行记者会称中国经济持续恢复，总体回升向好，依然是世界经济增长的重要引擎" 缩写成10个字以内'
while True:
  input_ids = tokenizer(prompt, return_tensors="pt",add_special_tokens=False).input_ids.to(device)
  generate_input = {
    "input_ids":input_ids,
    "max_new_tokens":1024,
    "do_sample":True,
    "top_k":50,
    "top_p":0.95,
    "temperature":0.8,
    "repetition_penalty":1.2,
    "eos_token_id":tokenizer.eos_token_id,
    "bos_token_id":tokenizer.bos_token_id,
    "pad_token_id":tokenizer.pad_token_id
  }
  generate_ids = model.generate(**generate_input)
  generate_ids = [item[len(input_ids[0]):-1] for  item in generate_ids]
  ans = tokenizer.batch_decode(generate_ids, skip_special_tokens=True, clean_up_tokenization_spaces=False)[0]
  print(ans)
  prompt = input('>> ')
  if prompt == 'exit':
    break
