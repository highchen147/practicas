# Animación de evolución temporal de Burgers1DV F
set xrange[0:100]
set yrange[-1:7]

do for [i=0:199] {
plot 'sol-burgers1DVFG.dat' index i u 2:3 w l
pause 0.05
print i
}