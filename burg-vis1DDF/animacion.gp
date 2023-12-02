
set terminal pdf
set encoding utf8
set xrange[20:80]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"

# set lmargin at screen 0.1
# set rmargin at screen 1.55
# set bmargin at screen 0.15
# set tmargin at screen 0.85

ttotal = 8
nouts = 400
dt = (ttotal/nouts)
dt = 0.02
k = 1
set size ratio 0.8
do for [i=0:190:47]{
    set output sprintf("results/for_tesis/frame-%04d.pdf", k)
    set title sprintf("t = %0.3f s", i*dt)
    plot 'sol-burg-vis1DDF-nu-0.50.dat' index i u 2:3 w l title "nu = 0.50", 'sol-burg-vis1DDF-nu-1.60.dat' index i u 2:3 w l title "nu = 1.60", 'sol-burg-vis1DDF-nu-3.00.dat' index i u 2:3 w l title "nu = 3.00", '../burgers1DVF/godunov/gauss-fija.dat' index i u 2:3 w l title "nu = 0"
    print i
    k = k + 1
}

# burgers1DVF/godunov/gauss-fija.dat

# Para animar las imágenes producidas correr el siguiente comando:
# ffmpeg -i results/frame-%04d.png -c:v libx264 -pix_fmt yuv420p -r 30 results/simu.mp4