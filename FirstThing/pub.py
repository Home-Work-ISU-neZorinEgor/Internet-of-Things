import serial
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression


def read_data_from_sensor(ser):
    x_values = []
    y_values = []

    numpoints = int(ser.readline().decode('utf-8').strip())

    for _ in range(num_points):
        value, distance = map(float, ser.readline().decode('utf-8').strip().split())
        x_values.append(value)
        y_values.append(int(distance))

    return x_values, y_values


def compute_regression(ser):
    X, Y = np.array(read_data_from_sensor(ser)).reshape(-1, 1, order='F')

    model = LinearRegression().fit(X, Y)
    slope, intercept = model.coef_[0], model.intercept_

    display_results(model, slope, intercept, X, Y)


def display_results(model, slope, intercept, x, y):
    predictions = model.predict(x)

    plt.scatter(x, y, label='Original Data')
    plt.plot(predictions, x, color='red', label=f'Linear Regression: y = {slope}x + {intercept}')
    plt.xlabel('Voltage')
    plt.ylabel('Distance')
    plt.legend()
    plt.show()


def get_serial_message(ser):
    message = b''
    while True:
        char = ser.read(1)
        message += char
        if char == b'\n':
            break

    return message.decode('utf-8')


ser = serial.Serial('COM3', 9600)

while True:
    received_message = ser.readline().decode('utf-8').strip()
    print(received_message)

    case_dict = {
        'Enter the range:': fetch_range_values,
        'message': handle_message
    }

    case_func = case_dict.get(received_message)
    if case_func:
        case_func()
        break


def fetch_range_values():
    min_value = int(input("Enter the minimum value: "))
    max_value = int(input("Enter the maximum value: "))
    ser.write(f"{min_value}\n".encode())
    ser.write(f"{max_value}\n".encode())


def handle_message():
    compute_regression(ser)


ser.close()