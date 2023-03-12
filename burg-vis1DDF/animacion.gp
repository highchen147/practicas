set term pngcairo size 800,600 enhanced font "Verdana,12"
unset key
set xrange[0:100]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"
ttotal = 8
nouts = 400
dt = (ttotal/nouts)
dt = 0.02
k = 1
do for [i=0:190]{
    set output sprintf("results/frame-%04d.png", k)
    set title sprintf("t = %0.3f s", i*dt)
    plot 'sol-burg-vis1DDF-nu-1.60.dat' index i u 2:3 w l title "nu = 1.60", 'sol-burg-vis1DDF-nu-3.00.dat' index i u 2:3 w l title "nu = 3.00", 'sol-burg-vis1DDF-nu-6.00.dat' index i u 2:3 w l title "nu = 6.00"
    print i
    k = k + 1
}


# Para animar las imágenes producidas correr el siguiente comando:
# ffmpeg -i results/frame-%04d.png -c:v libx264 -pix_fmt yuv420p -r 30 results/simu.mp4