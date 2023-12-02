# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-0.874561:4.3728]
print 'Presione Enter'
pause -1

do for [i=0:399] {
plot 'LF/gauss-fija.dat' index i u 2:3 w l
pause 0.02
print i
}