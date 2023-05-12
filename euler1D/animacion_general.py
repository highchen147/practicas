import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def generar_subsets(data: pd.DataFrame) -> list[pd.DataFrame]:
    
    vacios = data[pd.isna(data["t"])]
    bounds = np.array([-1] + list(vacios.index))
    desfases = np.zeros(len(bounds), dtype=int)
    desfases[::2] = 1
    bounds = np.add(bounds, desfases)
    bounds = bounds[:-1]

    subsets = [data.iloc[bounds[i]:bounds[i+1], :] for i in range(len(bounds))[::2]]

    return subsets

def animacion(data: pd.DataFrame, subset: pd.DataFrame, cantidad: str, margen: float, ax, line):
    
    # Get the x and y data_d for the current timestep
    x = subset['x'].values
    u = subset['u'].values
    
    # Update the data_d for the line
    line.set_data(x, u)
    
    # Set the x and y limits of the axis
    # margen = 0.25
    min_u = data['u'].min()
    max_u = data['u'].max()
    min_y = min_u - (max_u-min_u)*margen
    max_y = max_u + (max_u-min_u)*margen
    min_x = data['x'].min()
    max_x = data['x'].max()
    ax.set_xlim([min_x, max_x])
    ax.set_ylim([min_y, max_y])
    # Get time
    t = subset['t'].values[0]
    # Set the title of the axis to the current timestamp
    ax.set_title("{}, t={} s".format(cantidad, t))

    # return line



data_d = pd.read_csv("data/densidad.dat", delimiter='\t', skip_blank_lines=False)
data_d.columns = ["t", "x", "u"]
data_p = pd.read_csv("data/presion.dat", delimiter="\t", skip_blank_lines=False)
data_p.columns = ["t", "x", "u"]
data_u = pd.read_csv("data/velocidad.dat", delimiter="\t", skip_blank_lines=False)
data_u.columns = ["t", "x", "u"]

# data_d = data_d.query('t <= 6')
# data_p = data_p.query('t <= 6')
# data_u = data_u.query('t <= 6')
# print(data_d.tail())
# print("j")


# Crear la figura con tres sub-figuras
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(12, 4))

# Primera animación
line1, = ax1.plot([], [], lw=2)

subsets_d = generar_subsets(data_d)

def init():
    line1.set_data([], [])
    return (line1,)
# Definir animacion de densidad
def animate_1(i):

    animacion(data_d, subsets_d[i], 'Densidad', 0.25, ax1, line1)

    return line1,
    
num_frames = len(subsets_d)//2
dt = (subsets_d[1]['t'].values[0] - subsets_d[0]['t'].values[0])*1000

anim1 = animation.FuncAnimation(fig, animate_1, init_func=init, frames=(range(num_frames)), repeat=False, interval=100)

# Segunda animación
line2, = ax2.plot([], [], lw=2)

subsets_p = generar_subsets(data_p)

def init2():
    line2.set_data([], [])
    return (line2,)
# Definir animación de presión
def animate_2(i):
    animacion(data_p, subsets_p[i], 'Presión', 0.25, ax2, line2)
    return line2,


anim2 = animation.FuncAnimation(fig, animate_2, init_func=init2, frames=(range(num_frames)), repeat=False, interval=100)

# Tercera animación
line3, = ax3.plot([], [], lw=2)

subsets_u = generar_subsets(data_u)

def init3():
    line3.set_data([], [])
    return (line3,)
# Definir animación de velocidad
def animate_3(i):
    animacion(data_u, subsets_u[i], 'Velocidad', 0.25, ax3, line3)
    return line3,


anim3 = animation.FuncAnimation(fig, animate_3, init_func=init3, frames=(range(num_frames)), repeat=False, interval=100)

plt.show()