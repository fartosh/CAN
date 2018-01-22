import requests
import serial


def interpret_data(string):
    sensor = ''
    value = 0
    value_parsing = False
    for char in string:
        if char == '\r' or char == '\n':
            break
        elif not value_parsing:
            if char != ':':
                sensor += char
            else:
                value_parsing = True
        else:
            value *= 10
            value += int(char)
    return sensor, value


def send_data(sensor_id, value):
    url = "http://alfa.smartstorm.io/api/v1/measure"
    request_data = {"user_id": "126127@interia.pl",
                    "sensor_id": sensor_id,         # "5a637497bdb00155a3540295",
                    "desc": "data",
                    "measure_value": str(value)}
    requests.post(url, request_data)


if __name__ == '__main__':
    # ser = serial.Serial(
    #     port='/dev/ttyUSB0',
    #     baudrate=115200,
    #     parity=serial.PARITY_NONE,
    #     stopbits=serial.STOPBITS_ONE,
    #     bytesize=serial.EIGHTBITS,
    #     timeout=0)
    # print("connected to: " + ser.portstr)
    # while True:
    #     line = ser.readline()
    #     if line:
    #         print(line)
    sensor_id, value = interpret_data('ID4:00200302\r\n')
    print(sensor_id, value)
    send_data(sensor_id, value)

