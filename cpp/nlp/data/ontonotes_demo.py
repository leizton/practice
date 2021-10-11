import sys

line_i = 0
while True:
  l = sys.stdin.readline()
  if not l:
    break
  l = l.strip()
  if len(l) == 0:
    print('')
    continue
  #
  if l.startswith('INSERT INTO'):
    continue
  print(l)
  #
  line_i += 1
