import requests
import serial
import datetime
import sys
import select


# Function that interprets data from USB port
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


# Function that sends data to SmartStorm
# In case of failure, data is written to file
def send_data(sensor_id, value):
    dt = datetime.datetime.now()
    url = "http://alfa.smartstorm.io/api/v1/measure"
    request_data = {"user_id": "126127@interia.pl",
                    "sensor_id": sensor_id,
                    "desc": str(dt),
                    "measure_value": str(value)}
    try:
        requests.post(url, request_data, timeout=1)
    except requests.exceptions.RequestException:
        print("Could not send data to Smart Storm")
        with open('failed_data', 'a') as f:
            f.write(str(dt) + ',' + sensor_id + ',' + str(value) + '\n')


if __name__ == '__main__':
    # Initialization of USB connection
    ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0)
    print("connected to: " + ser.portstr)

    # Main loop
    while True:
        # Non-blocking reading from standard input
        # Written input is sent to microcontroller after pressing Enter
        while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
            line = sys.stdin.readline()
            if line:
                ser.write(str.encode(line[:-1]))

        # Non-blocking reading from USB port
        # Read data is verified and sent to SmartStorm if it is measurement
        line = ser.readline()
        if line:
            string = line.decode()
            if string[0:2] == "ID" and string[2] != ' ':
                sensor_id, value = interpret_data(string)
                sersor_valid = False
                if string[2] == '1':
                    sensor_id = "5a68bf39846646151caf16ba"
                    sersor_valid = True
                elif string[2] == '2':
                    sensor_id = "5a68bf28846646151caf16b9"
                    sersor_valid = True
                elif string[2] == '5':
                    sensor_id = "5a68bf39846646151caf16bb"
                    sersor_valid = True
                print(sensor_id, value)
                if sersor_valid:
                    send_data(sensor_id, value)
            else:
                print(string)


