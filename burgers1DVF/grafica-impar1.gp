# Animación de evolución temporal de Burgers1DV F
set xrange[0:100]
set yrange[-4:4]
print 'Presione Enter'
pause -1

do for [i=0:199] {
plot 'impar1.dat' index i u 2:3 w l
pause 0.05
print i
}