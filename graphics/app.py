import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

def connect(): #funkcja laczaca sie z mikrokontrolerem poprzez serial
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM4'
    ser.timeout = 0.045
    ser.open()
    return ser

def disconnect(ser): #funkcja rozlaczajaca port serial
    ser.close()
    plt.close()
    return

def read_data():
    try:
        if ser.in_waiting >= 2:  # Upewnij się, że są dostępne przynajmniej 2 bajty
            distance = int.from_bytes(ser.read(1), byteorder='big', signed=False)
            angle = int.from_bytes(ser.read(1), byteorder='big', signed=False)
            return distance, angle
    except:
        return None, None

def update(frame):
    global distance, angle
    new_distance, new_angle = read_data()
    if new_distance is not None and new_angle is not None:
        distance = new_distance
        angle = np.deg2rad(new_angle * 1.41)  # Konwersja kąta na radiany (skala 0-255 na 0-360 stopni)
    line.set_data([angle], [distance])
    ax.set_ylim(0, 255)  # Ustaw zakres dla odległości
    return line,


def draw(): #funkcja czytajaca dane oraz rysujaca wykres kolowy
    ani = animation.FuncAnimation(fig, update, blit = True, interval = 100)
    plt.show()
    return

if __name__ == "__main__":
    ser = connect()
    print(ser)
    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
    distance = 0
    angle = 0
    line, = ax.plot([], [], 'ro')
    frame = read_data
    line_tmp = update(frame)
    fig.canvas.mpl_connect('disconnect', disconnect)