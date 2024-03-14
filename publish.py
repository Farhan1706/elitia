import network
import time
from umqtt.simple import MQTTClient
import utime
import machine

button = machine.Pin(18, machine.Pin.IN)

wifi_ssid = "SSId"
wifi_password = "PWAIFU"

mqtt_host = "io.adafruit.com"
mqtt_username = "username" 
mqtt_password = "pwmqtt" 
mqtt_publish_topic = "topic/feeds/try" #The MQTT topic for your Adafruit IO Feed
mqtt_client_id = "client-id"

#Connect to WiFi
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(wifi_ssid, wifi_password)
while wlan.isconnected() == False:
    print('Waiting for connection...')
    time.sleep(1)
print("Connected to WiFi")

#Initialize our MQTTClient and connect to the MQTT server
mqtt_client = MQTTClient(
    client_id=mqtt_client_id,
    server=mqtt_host,
    user=mqtt_username,
    password=mqtt_password)
mqtt_client.connect()

try:
    while True:
        pb = button.value()
        #print(pb)
    #Generate some dummy data that changes every loop
        #counter += 3
    #Get the current timestamp
        current_time = utime.localtime()
        timestamp = "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(
        current_time[0], current_time[1], current_time[2],
        current_time[3], current_time[4], current_time[5]
        )
 
        #Publish the data to the topic!
        print(f'{timestamp} - Publish {pb}')
        mqtt_client.publish(mqtt_publish_topic, str(pb))
     
        #Delay a bit to avoid hitting the rate limit
        time.sleep(3)
except Exception as e:
    print(f'Failed to publish message: {e}')
finally:
    mqtt_client.disconnect()