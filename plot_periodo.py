import matplotlib.pyplot as plt
import numpy as np

file = rf"C:\Users\Ale\Desktop\UGR FISICA\FISICA_COMPUTACIONAL\Sistema_solar\t_data.dat"
# Lista de nombres de planetas
planet_names = ["Sol", "Mercurio", "Venus", "Tierra", "Marte", "Júpiter", "Saturno", "Urano", "Neptuno", "Plutón"]

n_planets=10

# Lectura del fichero de datos
with open(file, "r") as f:
    data_str = f.read()

# Inicializa la lista con los datos de cada fotograma.
# frames_data[j] contiene los datos del fotograma j-ésimo
frames_data = []

# Itera sobre los bloques de texto separados por líneas vacías
# (cada bloque corresponde a un instante de tiempo)
for frame_data_str in data_str.split("\n\n"):
    # Inicializa la lista con el periodo de cada planeta
    frame_data = []

    # Itera sobre las líneas del bloque
    # (cada línea da el periodo de un planeta)
    for planet_t in frame_data_str.split("\n"):
        if planet_t:
            frame_data.append(float(planet_t))

    # Verifica que frame_data tenga la longitud esperada
    if len(frame_data) == n_planets:
        # Añade los datos de este fotograma a la lista
        frames_data.append(frame_data)

# El número de planetas es el número de líneas en cada bloque
# Lo calculamos del primer bloque
n_planets = len(frames_data[0])

# Tiempos para cada fotograma
time_interval = 1.0  # Intervalo de tiempo entre fotogramas (puedes cambiarlo)
times = np.arange(len(frames_data)) * time_interval

# Creación de la animación/gráfico
fig, ax = plt.subplots()

for planet_index in range(n_planets):
    planet_periods = [frame_data[planet_index] for frame_data in frames_data]
    ax.plot(times, planet_periods, label=planet_names[planet_index])



ax.set_xlabel("Tiempo")
ax.xaxis
ax.set_ylabel("Período")
ax.legend()
ax.set_yscale('log')
plt.show()

