import paho.mqtt.client as paho
import time
import serial


serdev = '/dev/ttyACM1'
s = serial.Serial(serdev, 9600)

# https://os.mbed.com/teams/mqtt/wiki/Using-MQTT#python-client

# MQTT broker hosted on local machine
mqttc = paho.Client()

# Settings for connection
# TODO: revise host to your IP
host = "192.168.0.36"
topic = "Mbed"


s.write(bytes("/stop_condition/run 0 0\r", 'UTF-8'))
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)

s.write(bytes("/gesture_UI/run\r", 'UTF-8'))
line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
print(line)
line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
print(line)



# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    #print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");
    split=str(msg.payload).split("'")
    command=str(split[1]).split("\\",2)
    print(command[0])
    if(command[0]=="30"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)
        time.sleep(1)

        print("It is working")
        s.write(bytes("/detection/run 30 30\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)

        

        
        
    elif(command[0]=="45"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)
        time.sleep(1)

        print("It is working")
        s.write(bytes("/detection/run 45 45\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)

        
        

    elif(command[0]=="60"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)
        time.sleep(1)

        print("It is working")
        s.write(bytes("/detection/run 60 60\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)

        
        

    elif(command[0]=="80"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)
        time.sleep(1)

        print("It is working")
        s.write(bytes("/detection/run 80 80\r", 'UTF-8'))
        line=s.readline() # Read an echo string from mbed terminated with '\n' (putc())
        print(line)
        line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
        print(line)

        
        
def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed OK")

def on_unsubscribe(mosq, obj, mid, granted_qos):
    print("Unsubscribed OK")

# Set callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

# Connect and subscribe
print("Connecting to " + host + "/" + topic)
mqttc.connect(host, port=1883, keepalive=60)
mqttc.subscribe(topic, 0)

# Publish messages from Python
num = 0
while num != 5:
    ret = mqttc.publish(topic, "Message from Python!\n", qos=0)
    if (ret[0] != 0):
            print("Publish failed")
    mqttc.loop()
    time.sleep(1.5)
    num += 1

# Loop forever, receiving messages
mqttc.loop_forever()