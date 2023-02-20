# Animación de evolución temporal de Burgers1DDF
set xrange[0:100]
set yrange[-1:15]

do for [i=0:47] {
plot 'sol-burgers1D.dat' index i u 2:3 w lp
pause -1
print i
}