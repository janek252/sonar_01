import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# Ustawienia początkowe
fig, ax = plt.subplots()
x = np.linspace(0, 2 * np.pi, 100)
line, = ax.plot(x, np.sin(x))

# Funkcja aktualizująca dane
def update(frame):
    line.set_ydata(np.sin(x + frame / 10.0))  # Aktualizuje dane y
    return line,

# Tworzenie animacji
ani = animation.FuncAnimation(fig, update, frames=100, blit=True, interval=50)

plt.show()