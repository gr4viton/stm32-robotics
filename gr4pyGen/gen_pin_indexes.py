import time
import os
#import str.join
#import sys

ports = "ABCDEHI"


txt=""

line=[]
line.append("#define P")
line.append("P")
line.append("X")
line.append(" \t\tP")
line.append("P")
line.append("(")
line.append("X")
line.append(")\n")

pins=[]
for p in range(0,15+1):
    pins.append(p)

#time.sleep(10)


txt+="//____________________________________________________\n"
txt+="// pin indexes\n"

idx=0
for P in ports:
    txt+="\n/* PORT"+str(P)+" */\n"
    for X in range(0,16):
        for s in line:
            if s=="i":
                part=idx
            elif s=="P":
                part=P
            elif s=='X':
                part=pins[X]
            else:
                part=s
            #;b = str(add)
            txt += str(part)
            #print(part) 
        idx += 1

#print(txt)
#time.sleep(0)

# write it to file
#path = os.path.dirname(os.path.realpath(__file__)) + "pins.txt"
path = "pin_indexes_definitions.txt"
f = open(path, "w")
f.write(txt)
f.close()

f = open(path, "r")
txtR = f.read()
print(txtR)


time.sleep(1)
#raw_input()
#sys.
