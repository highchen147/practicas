

file = 'data/velocidad.dat'
stats file using 3 nooutput 
set xrange[0:100]
set yrange[STATS_min:STATS_max]

do for [i=0:500] {
    plot file index i u 2:3 w l
    
    pause 0.1

}