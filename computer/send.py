import requests
import serial
import datetime
import sys
import select


# Funkcja interpretująca dane otrzymane z portu USB, zwraca id sensora oraz pomierzoną przez niego wartość
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


# Funkcja wysyłająca dane na SmartStorm, przyjmuje id sensora oraz wartości przez niego pomierzone
# W razie niepowodzenia dane zostają zapisane do pliku wraz z datą i godziną
def send_data(sensor_id, value):
    dt = datetime.datetime.now()
    url = "http://alfa.smartstorm.io/api/v1/measure"
    request_data = {"user_id": "126127@interia.pl",
                    "sensor_id": sensor_id,         # "5a637497bdb00155a3540295",
                    "desc": str(dt),
                    "measure_value": str(value)}
    try:
        requests.post(url, request_data, timeout=1)
    except requests.exceptions.RequestException:
        print("Could not send data to Smart Storm")
        with open('failed_data', 'a') as f:
            f.write(str(dt) + ',' + sensor_id + ',' + str(value) + '\n')


if __name__ == '__main__':
    # Zestawienie połączenia USB
    ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0)
    print("connected to: " + ser.portstr)

    # Pętla główna skryptu
    while True:
        # Zczytywanie nieblokujące ze standardowego wejścia
        # Po wciśnięciu klawisza Enter następuje wysłanie ciągu znaków do urządzenia podłączonego przez USB
        while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
            line = sys.stdin.readline()
            if line:
                ser.write(str.encode(line[:-1]))

        # Zczytywanie nieblokujące z portu USB
        # Otrzymane dane są weryfikowane oraz wysyłane do SmartStorm jeżeli są pomiarami
        line = ser.readline()
        if line:
            string = line.decode()
            if sensor_id[0:2] == "ID" and sensor_id[2] != ' ':
                sensor_id, value = interpret_data(string)
                print(sensor_id, value)
                send_data(sensor_id, value)
            else:
                print(string)


