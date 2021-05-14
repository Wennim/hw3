import paho.mqtt.client as paho
import time
import serial


serdev = '/dev/ttyACM0'
s = serial.Serial(serdev, 9600)

# https://os.mbed.com/teams/mqtt/wiki/Using-MQTT#python-client

# MQTT broker hosted on local machine
mqttc = paho.Client()

# Settings for connection
# TODO: revise host to your IP
host = "192.168.0.32"
topic = "Mbed"


s.write(bytes("/stop_condition/run 0 0\r", 'UTF-8'))

s.write(bytes("/gesture_UI/run\r", 'UTF-8'))




# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    #print(str(msg.payload));
    split=str(msg.payload).split("\\")
    command=str(split[0]).split(" ")
    print(split[0])
    if(command[2]=="30"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        time.sleep(1)

        s.write(bytes("/detection/run 30 30\r", 'UTF-8'))
        

        

        
        
    elif(command[2]=="45"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
    
        time.sleep(1)

        
        s.write(bytes("/detection/run 45 45\r", 'UTF-8'))
       

        
        

    elif(command[2]=="60"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        
        time.sleep(1)

        
        s.write(bytes("/detection/run 60 60\r", 'UTF-8'))
        

        
        

    elif(command[2]=="80"):
        s.write(bytes("/stop_condition/run 1 1\r", 'UTF-8'))
        time.sleep(1)

        
        s.write(bytes("/detection/run 80 80\r", 'UTF-8'))

    elif(command[1]=="threshold"):
        print("Returning to gesture UI")
        s.write(bytes("/stop_condition/run 0 0\r", 'UTF-8'))

        s.write(bytes("/gesture_UI/run\r", 'UTF-8'))

        
        
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