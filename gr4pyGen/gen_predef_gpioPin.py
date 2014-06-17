import time
import os
#import str.join
#import sys

ports = "ABCDEHI"

txt=""

line=[]
line.append("/*")
line.append("i")
line.append("*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIO")
line.append("P")
line.append(",.port=GPIO")
line.append("P")
line.append(",.pin=GPIO")
line.append("X")
line.append(",.exti_enabled=0,.exti=EXTI")
line.append("X")
line.append(",.irq=NVIC_EXTI")
line.append("X")
line.append("_IRQ}\n")

pins=[]
for p in range(0,15+1):
    pins.append(p)

#time.sleep(10)

txt+="S_model_gpioPin predef_gpioPin[] =\n"
txt+="{\n"
txt+="// please remove first comma!\n"

idx=0
for P in ports:
    for X in range(0,15+1):
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

txt+="};\n"

#print(txt)
#time.sleep(0)

# write it to file
#path = os.path.dirname(os.path.realpath(__file__)) + "pins.txt"
path = "predef_gpioPin.txt"
f = open(path, "w")
f.write(txt)
f.close()

f = open(path, "r")
txtR = f.read()
print(txtR)


time.sleep(1)
#raw_input()
#sys.
