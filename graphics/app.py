import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

def connect():  # Funkcja łącząca się z mikrokontrolerem poprzez UART
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM4'
    ser.timeout = 0.04
    ser.open()
    return ser

def disconnect(event, ser):  # Funkcja rozłączająca UART
    ser.close()
    plt.close()
    return

def read_data(ser):
    try:
        line = ser.readline().decode('utf-8').strip()
        if line:
            parts = line.split()
            if len(parts) == 2:
                distance = int(parts[0])
                angle = int(parts[1])
                return distance, angle
    except Exception as e:
        print(f"Error reading data: {e}")
    return None, None


def update(frame):
    global data
    new_distance, new_angle = read_data(ser)
    if new_distance is not None and new_angle is not None:
        data[new_angle] = new_distance  # Nadpisz starą wartość dla tego samego kąta
    if data:
        angles = np.deg2rad(list(data.keys()))  # Konwersja kątów na radiany
        distances = list(data.values())
        line.set_data(angles, distances)
        ax.set_ylim(0, max(max(distances) + 10, 150))  # Ustaw zakres dla odległości, aby nie był statyczny
    return line,

if __name__ == "__main__":
    ser = connect()
    print(ser)

    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
    data = {} # Słownik do przechowywania danych (angle: distance)
    line, = ax.plot([], [], 'ro')
    
    ani = animation.FuncAnimation(fig, update, blit=True, interval=100)  # Tworzenie animacji

    fig.canvas.mpl_connect('close_event', lambda event: disconnect(event, ser))  # Zamykanie połączenia UART po zamknięciu okna z animacją

    plt.show()
