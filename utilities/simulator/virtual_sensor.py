import serial
import math
import numpy as np
import time

with serial.Serial("COM15", 115200) as ser:
    period = 120;
    
    while True:
        for x in np.arange(1, 2*math.pi, 2*math.pi/period):
            s = "%.1f\n\n" % (math.sin(x) * 80 + 100);
            print s;
            ser.write(s);
            bytesToRead = ser.inWaiting()
            print ser.read(bytesToRead)
            print
            time.sleep(1.0);
            
        