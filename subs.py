import time
import network
from machine import Pin
from umqtt.simple import MQTTClient
import utime
import machine
pin = machine.Pin(18, machine.Pin.OUT)
# Fill in your WiFi network name (ssid) and password here:
wifi_ssid = "Pengejar Deadline"
wifi_password = "123456778899"
# Connect to WiFi
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(wifi_ssid, wifi_password)
while wlan.isconnected() == False:
    print('Waiting for connection...')
    time.sleep(1)
print("Connected to WiFi")
mqtt_host = "io.adafruit.com"
mqtt_username = "username"
mqtt_password = "password"
mqtt_receive_topic = "topic/feeds/try" # The MQTT topic for your Adafruit IO Feed
mqtt_client_id = "clientid"
mqtt_client = MQTTClient(
client_id=mqtt_client_id,
server=mqtt_host,
user=mqtt_username,
password=mqtt_password)

def mqtt_subscription_callback(topic, message):
    # Get the current timestamp
    current_time = utime.localtime()
    timestamp = "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(
    current_time[0], current_time[1], current_time[2],
    current_time[3], current_time[4], current_time[5]
    )
    print (f'{timestamp} - Topic {topic} received message {message}') #Debug print out of what was received over MQTT
    if message == b'1':
        pin.value(1)
    else :
        pin.value(0)
mqtt_client.set_callback(mqtt_subscription_callback)
mqtt_client.connect()
mqtt_client.subscribe(mqtt_receive_topic)
print("Connected and subscribed")

try:
    while True:
        print(f'Waiting for messages on {mqtt_receive_topic}')
        mqtt_client.wait_msg()
    
except Exception as e:
    print(f'Failed to wait for MQTT messages: {e}')
finally:
    mqtt_client.disconnect(