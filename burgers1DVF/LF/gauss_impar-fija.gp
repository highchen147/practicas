# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-6.36649:6.36649]
print 'Presione Enter'
pause -1

do for [i=0:999] {
plot 'LF/gauss_impar-fija.dat' index i u 2:3 w l
pause 0.02
print i
}