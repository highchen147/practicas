# Definir una variable para almacenar el valor máximo y mínimo en y
umax = 0
xmax = 0
umin = 0
xmin = 0

# Leer todos los archivos y obtener el valor máximo en y
do for [file in "roe/gauss-fija.dat godunov/gauss-fija.dat"] {
    stats file using 3 nooutput
    umax = (STATS_max > umax ? STATS_max : umax)
    umin = (STATS_min < umin ? STATS_min : umin)
    stats file using 2 nooutput
    xmax = (STATS_max > xmax ? STATS_max : xmax)
    xmin = (STATS_min < xmin ? STATS_min : xmin)
}

# Establecer margen
margen = 0.25
# double margen = 0.25;
ymax = umax + (umax-umin)*margen;
ymin = umin - (umax-umin)*margen;

set xrange[xmin:xmax]
set yrange[ymin:ymax]
print 'Presione Enter'
pause -1
#



