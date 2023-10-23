import time
import paho.mqtt.client as paho
import datetime

broker = "broker.emqx.io"
client = paho.Client("client-isu-741")

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")

min_duration = 20  # Минимальная длительность свечения
max_duration = 40  # Максимальная длительность свечения
current_duration = max_duration  # Начинаем с максимальной длительности
try:
    while True:

        now = datetime.datetime.now()
        # Уменьшаем длительность свечения каждую следующую минуту
        if now.second == 0:
            max_duration -= 1

        # Проверяем и устанавливаем минимальное время свечения
        if max_duration == 30:
            max_duration += 10

        if now.second >= min_duration and now.second <= max_duration:
            state = "0"
        else:
            state = "1"

        current_time = now.strftime("%Y-%m-%d %H:%M:%S")
        print(f'Time: {current_time}, state: {state}, светит с {min_duration} по: {max_duration} секунд')
        client.publish("esp8266Egor/command", state)

        time.sleep(1)



except KeyboardInterrupt:
    client.disconnect()
    client.loop_stop()