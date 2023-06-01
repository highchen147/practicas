set term pdf
# Margen
margen = 0.35
# Número de gráficas temporales
num_outs = 123
# valor máximo en u
umax = 0
# valor mínimo en u
umin = 0
# valor mínimo en x
xmin = 0
# valor máximo en x
xmax = 0
dt = 1.064

do for [file in "roe/step_neg-periodica.dat godunov/step_neg-periodica.dat LF/step_neg-periodica.dat"] {
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
set xlabel "x (m)"
set ylabel "u (m/s)"
print 'Presione Enter'
pause -1

do for [i=0:num_outs:21] {
set output sprintf("results/sol_periodicas/step_neg/%03d.pdf", i)
set title sprintf("step neg, t = %0.3f s", i*dt)
plot 'roe/step_neg-periodica.dat' index i u 2:3 w l title "Roe", 'godunov/step_neg-periodica.dat' index i u 2:3 w l title "Godunov", 'LF/step_neg-periodica.dat' index i u 2:3 w l title "Lax-Friedrichs"
# pause -1
print i
}