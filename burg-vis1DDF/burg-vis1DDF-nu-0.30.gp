# Animación de evolución temporal de Burgers viscosa
set terminal pdf
set xrange[0:100]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"
unset key

instantes = "0 67 82 107"
dt = 0.02
# pause -1
do for [n in instantes] {
    i =  real(n)
    set output sprintf("results/frames0.3/frame%03d.pdf", i)
    set title sprintf("t = %0.3f s", i*dt)
    plot 'sol-burg-vis1DDF-nu-0.30.dat' index i u 2:3 w l
    # pause -1
    print i
}