set term pngcairo size 800,600 enhanced font "Verdana,12"
# set terminal pdf
unset key
set xrange[0:100]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"
ttotal = 1.2
nouts = 47
dt = (ttotal/nouts)

instantes = "1 26 37 40 44 45"
k = 1
# do for [n in instantes] {
#     i = real(n)
#     set output sprintf("results/frame-%04d.png", k)
#     # set output sprintf("results/frame%03d.pdf", i)
#     set title sprintf("t = %0.3f s", i*dt)
#     plot 'sol-burgers1D.dat' index i u 2:3 w l
#     print i
#     k = k + 1
# }   

do for [i=0:47]{
    set output sprintf("results/frame-%04d.png", k)
    # set output sprintf("results/frame%03d.pdf", i)
    set title sprintf("t = %0.3f s", i*dt)
    plot 'sol-burgers1D.dat' index i u 2:3 w l
    print i
    k = k + 1
}

# ffmpeg -i results/frame-%04d.png -c:v libx264 -pix_fmt yuv420p -r 30 results/simu.mp4