# Animación de evolución temporal de Burgers1DVF
set xrange[0:100]
set yrange[-1.68384:1.53677]
print 'Presione Enter'
pause -1

do for [i=0:123] {
plot 'roe-fix/step_pos-periodica.dat' index i u 2:3 w l
pause 0.02
print i
}