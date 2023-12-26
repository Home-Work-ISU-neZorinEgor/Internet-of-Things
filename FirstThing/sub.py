import time
import paho.mqtt.client as paho
import serial

port = "COM9"
connection = serial.Serial(port, timeout=1)


broker = 'broker.emqx.io'

responses = {
              'd':7,
              'u':6
            }

def send_command(cmd: str, responses_len:int) -> str:
  connection.write(cmd.encode())
  if (responses_len != 0):
    #connection.in_waiting <-> available
    resp = connection.read(responses_len)
    str_resp = resp.decode()
  return str_resp

def on_message(client, userdata, message):
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    if data in list(responses.keys()):
        send_command(data, responses[data])
        print("received message =", data)


client = paho.Client('client-isu-742')
client.on_message = on_message

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
client.subscribe("esp8266Egor/command")
time.sleep(30)
client.disconnect()
client.loop_stop()

#просмотр