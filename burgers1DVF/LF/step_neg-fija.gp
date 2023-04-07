# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-0.529343:2.47378]
print 'Presione Enter'
pause -1

do for [i=0:999] {
plot 'LF/step_neg-fija.dat' index i u 2:3 w l
pause 0.02
print i
}