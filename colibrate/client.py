import serial
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression


def read_val(ser):
    x = []
    y = []
    num_points = int(ser.readline().decode('utf-8').strip())

    for i in range(num_points):
        value = float(ser.readline().decode('utf-8').strip())
        distance = int(ser.readline().decode('utf-8').strip())
        x.append(value)
        y.append(distance)
    return x, y

def calc_values(ser):
    values, distances = read_val(ser)
    X = np.array(values).reshape(-1, 1)
    Y = np.array(distances).reshape(-1, 1)
    model = LinearRegression()
    model.fit(X, Y)

    slope = model.coef_[0]
    intercept = model.intercept_

    show_data(model, slope, intercept, X, Y)


def show_data(model, slope, intercept, x, y):
    print(f'Linear regression equation: y = {slope}x + {intercept}')

    predictions = model.predict(x)

    plt.scatter(y, x, label='Original Data')
    plt.plot(predictions, x, color='red', label='Linear Regression')
    plt.xlabel('Voltage')
    plt.ylabel('Distance')
    plt.legend()
    plt.show()

def get_message(ser):
    mess = ser.read()
    message = mess
    while mess != "\n".encode():
        mess = ser.read()
        message += mess
        print(message)
    return message.decode('utf-8')


ser = serial.Serial('COM3', 9600)

while True:
    message = ser.readline().decode('utf-8').strip()
    print(message)

    case_dict = {
    'Enter the range:': lambda: get_range_values(),
    'message': lambda: process_message()
    }


    if message in case_dict:
        case_dict[message]()
        break

def get_range_values():
    min_value = int(input("Enter the minimum value: "))
    max_value = int(input("Enter the maximum value: "))
    ser.write(f"{min_value}\n".encode())
    ser.write(f"{max_value}\n".encode())

def process_message():
    calc_values(ser)

print("end")
ser.close()