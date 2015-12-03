import serial
import math
import numpy as np
import time

sep = 5;

def valueSend(ser, s):
    print s;
    ser.write(s + "\n");
    bytesToRead = ser.inWaiting()
    print ser.read(bytesToRead)
    print
    
    time.sleep(sep);
    
    
with serial.Serial("COM15") as ser:
    valueSend(ser, "V0");
    valueSend(ser, "V25");
    valueSend(ser, "V100");
    valueSend(ser, "V175");
    valueSend(ser, "V250");
    valueSend(ser, "A");
