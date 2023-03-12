# Animación de evolución temporal de Burgers viscosa
set xrange[0:100]
set yrange[-1:7]

pause -1
do for [i=0:399] {
plot 'sol-burg-vis1DDF-nu-1.60.dat' index i u 2:3 w l title "nu = 1.60", 'sol-burg-vis1DDF-nu-3.00.dat' index i u 2:3 w l title "nu = 3.00", 'sol-burg-vis1DDF-nu-6.00.dat' index i u 2:3 w l title "nu = 6.00"
pause 0.02
print i
}