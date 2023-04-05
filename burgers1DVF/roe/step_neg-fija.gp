# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-1.25:6.25]
print 'Presione Enter'
pause -1

do for [i=0:399] {
plot 'roe/step_neg-fija.dat' index i u 2:3 w l
pause 0.02
print i
}