import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def generar_subsets(data: pd.DataFrame, col: str) -> list[pd.DataFrame]:
    
    vacios = data[pd.isna(data["t"])]
    bounds = np.array([-1] + list(vacios.index))
    desfases = np.zeros(len(bounds), dtype=int)
    desfases[::2] = 1
    bounds = np.add(bounds, desfases)
    bounds = bounds[:-1]

    subsets = [data.iloc[bounds[i]:bounds[i+1], :] for i in range(len(bounds))[::2]]

    return subsets

data_d = pd.read_csv("data/densidad.dat", delimiter='\t', skip_blank_lines=False).columns = ["t", "x", "u"]
data_p = pd.read_csv("data/presion.dat", delimiter="\t", skip_blank_lines=False).columns = ["t", "x", "u"]
data_u = pd.read_csv("data/velocidad.dat", delimiter="\t", skip_blank_lines=False).columns = ["t", "x", "u"]


generar_subsets(data_d, 's')