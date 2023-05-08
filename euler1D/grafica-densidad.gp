
set xrange[0:100]
set yrange[0:5]

do for [i=0:50] {
    plot 'data/densidad.dat' index i u 2:3 w l
    
    # pause 0.1
    pause -1
    # print i*0.025+0.01

}