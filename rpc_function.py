import serial
import time
import paho.mqtt.client as paho
import time

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev, 9600)

s.write(bytes("\r", 'UTF-8'))
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
time.sleep(1)

s.write(bytes("/selecting/run\r", 'UTF-8'))
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
time.sleep(20)

s.write(bytes("/detection/run\r", 'UTF-8'))
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)
time.sleep(10)


s.close()