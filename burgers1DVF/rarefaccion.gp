# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-1.5:1.5]
print 'Presione Enter'
pause -1

do for [i=0:999] {
plot 'rarefaccion.dat' index i u 2:3 w l, 'godunov/step_pos-fija.dat' index i u 2:3 w l
pause 0.02
print i
}