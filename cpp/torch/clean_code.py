a = '''
'''

def main():
  st = 0
  for l in a.split('\n'):
    if st == 0:
      if l == '#ifdef _WIN32':
        st = 1
      else:
        print(l)
    elif st == 1:
      if l == '#else':
        st = 2
      elif l == '#endif':
        st = 0
    elif st == 2:
      if l == '#endif':
        st = 0
      else:
        print(l)

if __name__ == '__main__':
  main()
