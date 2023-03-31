# Animación de evolución temporal de Burgers1DV F
set xrange[0:100]
set yrange[-4.37368:0.874736]
print 'Presione Enter'
pause -1

do for [i=0:399] {
plot 'gaussiana-gudonov1.dat' index i u 2:3 w l
pause 0.02
print i
}