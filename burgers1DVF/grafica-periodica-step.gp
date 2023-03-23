# Animación de evolución temporal de Burgers1DV F
set xrange[0:100]
set yrange[-1:7]
print 'Presione Enter'
pause -1

do for [i=0:999] {
plot 'periodica-step.dat' index i u 2:3 w l
pause 0.02
print i
}