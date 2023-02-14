
set xrange[0:8]
set yrange[0:100]
set xlabel "tiempo"
set ylabel "X"

splot 'sol-burgers1Dsurf.dat' u 1:2:3 w p
