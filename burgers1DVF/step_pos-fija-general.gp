# Margen
margen = 0.25
# Número de gráficas temporales
num_outs = 999
# valor máximo en u
umax = 0
# valor mínimo en u
umin = 0
# valor mínimo en x
xmin = 0
# valor máximo en x
xmax = 0
dt = 0.03

do for [file in "roe/step_pos-fija.dat godunov/step_pos-fija.dat"] {
    stats file using 3 nooutput
    umax = (STATS_max > umax ? STATS_max : umax)
    umin = (STATS_min < umin ? STATS_min : umin)
    stats file using 2 nooutput
    xmax = (STATS_max > xmax ? STATS_max : xmax)
    xmin = (STATS_min < xmin ? STATS_min : xmin)
}
# Establecer margen
ymax = umax + (umax-umin)*margen;
ymin = umin - (umax-umin)*margen;

set xrange[xmin:xmax]
set yrange[ymin:ymax]
print 'Presione Enter'
pause -1

do for [i=0:num_outs:170] {
print i
set title sprintf("step pos, t = %0.3f s", i*dt)
plot 'roe/step_pos-fija.dat' index i u 2:3 with lines title "Roe", 'godunov/step_pos-fija.dat' index i u 2:3 w l title "Godunov", 'LF/step_pos-fija.dat'index i u 2:3 w l title "Lax-Friedrichs"
pause -1
}