# Margen
margen = 0.25
# Número de iteraciones temporales
N_iter = 999
# valor máximo en y

do for [file in "roe/gauss-fija.dat godunov/gauss-fija.dat"] {
    stats file using 3 nooutput
    umax = (STATS_max > umax ? STATS_max : umax)
    umin = (STATS_min < umin ? STATS_min : umin)
    stats file using 2 nooutput
    xmax = (STATS_max > xmax ? STATS_max : xmax)
    xmin = (STATS_min < xmin ? STATS_min : xmin)
}

# Establecer margen
# margen = 0.25
# double margen = 0.25;
ymax = umax + (umax-umin)*margen;
ymin = umin - (umax-umin)*margen;

set xrange[xmin:xmax]
set yrange[ymin:ymax]
print 'Presione Enter'
pause -1
#



