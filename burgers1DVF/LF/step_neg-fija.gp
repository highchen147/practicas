# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-1:10]
print 'Presione Enter'
pause -1

do for [i=0:15] {
plot 'LF/step_neg-fija.dat' index i u 2:3 w l
pause -1
print i
}