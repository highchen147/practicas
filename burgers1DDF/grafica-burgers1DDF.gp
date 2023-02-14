
set xrange[0:100]
set yrange[-1:20]

do for [i=0:300] {
    plot 'sol-burgers1D.dat' index i u 2:3 w lp
    
    # pause 0.1
    pause -1
    print i

}