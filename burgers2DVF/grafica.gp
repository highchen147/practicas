set title "Burgers 2D"
set xlabel "X"
set ylabel "Y"
set size ratio 1
set pm3d map
set palette rgbformulae 33,13,10
archivo = "square-periodica.dat"

stats archivo using 4 nooutput 
set cbrange [STATS_min:STATS_max]

do for[i=0:300]{
    splot archivo index i using 2:3:4 with pm3d
    print i
    pause 0.002
}