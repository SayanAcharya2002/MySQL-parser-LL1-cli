import sys
file=sys.argv[1]
sym=set()
all=set()

with open(file,'r') as f:
  for i in f.readlines():
    i=i.strip('\r\n').strip('\n').strip('\r')
    l=i.split()
    sym.add(l[0])
    all.update(l)

print(*(all-sym))