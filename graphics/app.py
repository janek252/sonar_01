import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

def connect(): # Funkcja łącząca się z mikrokontrolerem poprzez UART
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM4'
    ser.timeout = 0.04
    ser.open()
    return ser

def disconnect(ser): # Funkcja rozłączająca UART
    ser.close()
    plt.close()
    return

def read_data(ser):
    try:
        if ser.in_waiting >= 8:  # Upewnij się, że są dostępne przynajmniej 2 bajty
            distance = int.from_bytes(ser.read(4), byteorder='big', signed=False)
            angle = int.from_bytes(ser.read(4), byteorder='big', signed=False)
            return distance, angle
    except:
        return None, None

def update(frame):
    global distance, angle
    new_distance, new_angle = read_data(ser)
    if new_distance is not None and new_angle is not None:
        distance = new_distance
        angle = np.deg2rad(new_angle * 1.41)  # Konwersja kąta na radiany (skala 0-255 na 0-360 stopni)
    line.set_data([angle], [distance])
    ax.set_ylim(0, 255)  # Ustaw zakres dla odległości
    return line,

if __name__ == "__main__":

    ser = connect()
    print(ser)

    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
    distance = 0
    angle = 0
    line, = ax.plot([], [], 'ro')
    
    ani = animation.FuncAnimation(fig, update, blit = True, interval = 100) # Tworzenie animacji

    fig.canvas.mpl_connect('close_event', disconnect) # Zamykanie połączenia UART po zamknięciu okna z animacją

    plt.show()