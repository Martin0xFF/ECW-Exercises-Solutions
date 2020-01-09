import serial
import time

import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation 
from   matplotlib import style, use

use('TkAgg')
queue = []
style.use('bmh')
fig = plt.figure(figsize=(16,9))
ax1 = fig.add_subplot(1,1,1)
   
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def animate(i):
    xs = []
    ys = [] 
    global queue
    for i in range(0,1):
        while True:
            line = ser.readline().decode('utf-8')
            
            if (',' in line) and (not '-1' in line):
                break
        queue.append(line)
        if len(queue) >150:
            queue = queue[1:-1] 



    if len(queue)>0:
        for el in queue: 
            x, y = el.split(',')
            xs.append(float(y)*numpy.cos(float(x)*numpy.pi/180))
            ys.append(float(y)*numpy.sin(float(x)*numpy.pi/180))

    m = ys[-1]/xs[-1]

    ye = [0, m*xs[-1]]
    xe = [0,   xs[-1]]
    
    ax1.clear()
   # ax1.set_xlim([400, 2000])
   # ax1.set_ylim([0, 1600])
    ax1.plot(xs, ys, '.r')
    ax1.plot(xe, ye, '--r')
ani = animation.FuncAnimation(fig, animate, interval=1)
plt.show()
