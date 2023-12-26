import time
import paho.mqtt.client as paho

broker = "broker.emqx.io"

client = paho.Client("client-isu-741")

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()

while True:
    user_input = input("Enter a single letter ('d' or 'u'): ")
    if len(user_input) == 1 and user_input.isalpha():
        state = user_input.lower()
        print(f'State is {state}')
        client.publish("esp8266Egor/command", state)
        break
    else:
        print("Invalid input. Please enter a single letter.")

time.sleep(2)

client.disconnect()
client.loop_stop()
