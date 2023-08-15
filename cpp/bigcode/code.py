from transformers import AutoModelForCausalLM, AutoTokenizer

#checkpoint = "bigcode/starcoder"
checkpoint = "/Users/awk/sky/coder/starcoderbase-1b"
device = "cpu"

tokenizer = AutoTokenizer.from_pretrained(checkpoint)
# to save memory consider using fp16 or bf16 by specifying torch_dtype=torch.float16 for example
model = AutoModelForCausalLM.from_pretrained(checkpoint).to(device)

inputs = tokenizer.encode("def plus(x, y):", return_tensors="pt").to(device)
outputs = model.generate(inputs)
# clean_up_tokenization_spaces=False prevents a tokenizer edge case which can result in spaces being removed around punctuation
print(tokenizer.decode(outputs[0], clean_up_tokenization_spaces=False))
