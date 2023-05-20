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
set term pdf
unset key

do for [file in "godunov/gauss-fija.dat"] {
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

do for [i=0:0] {
    set output "results/graficas_iniciales/gauss.pdf"
    set title "Gráfica de {/:Italic gauss}"
    set xlabel "x"
    set ylabel "u"
    plot 'godunov/gauss-fija.dat' index i u 2:3 w l
    pause -1
}

