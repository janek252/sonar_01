import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

def connect(): #funkcja laczaca sie z mikrokontrolerem poprzez serial
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM4'
    ser.timeout = 0.04
    ser.open()
    return ser

def disconnect(ser): #funkcja rozlaczajaca port serial
    ser.close()
    return

def draw(ser): #funkcja czytajaca dane oraz rysujaca wykres kolowy
    while(True):
        dist = ser.read()
        angle = ser.read()
    return

if __name__ == "__main__":
    ser = connect()
    print(ser)
    draw(ser)
    disconnect(ser)