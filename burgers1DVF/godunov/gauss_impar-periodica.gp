# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-5.19971:5.19971]
print 'Presione Enter'
pause -1

do for [i=0:399] {
plot 'godunov/gauss_impar-periodica.dat' index i u 2:3 w l
pause 0.02
print i
}