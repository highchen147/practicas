# Animación de evolución temporal de Burgers viscosa
set xrange[0:100]
set yrange[-1:15]

pause -1
do for [i=0:399] {
plot 'sol-burg-vis1DDF-nu-0.30.dat' index i u 2:3 w l
pause 0.02
print i
}