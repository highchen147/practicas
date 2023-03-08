# Animación de evolución temporal de Burgers1DDF
set terminal pdf
unset key
set xrange[0:100]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"
dt = 0.005
instantes = "0 16 32 47"
# do for [i=0:47] {
# plot 'sol-burgers1DCON.dat' index i u 2:3 w lp
# pause -1
# print i
# }
do for[n in instantes]{
    i = real(n)
    set output sprintf("resultados-conservativa/frame%03d.pdf", i)
    set title sprintf("t = %0.3f s", i*dt)
    plot 'sol-burgers1DCON.dat' index i u 2:3 w l
    print i
}