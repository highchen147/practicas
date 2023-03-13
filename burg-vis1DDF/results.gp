set terminal pdf
set xrange[0:100]
set yrange[-1:7]
set xlabel "x (m)"
set ylabel "u (m/s)"
t = 8
num_outs = 400

dt = (0.02)
print dt
# pause -1
instantes = "0 16 30 50 90 190"
do for [n in instantes] {
i = real(n)
set output sprintf("results/frame%03d.pdf", i)
set title sprintf("t = %0.3f s", i*dt)
plot 'sol-burg-vis1DDF-nu-0.50.dat' index i u 2:3 w l title "nu = 0.50", 'sol-burg-vis1DDF-nu-1.60.dat' index i u 2:3 w l title "nu = 1.60", 'sol-burg-vis1DDF-nu-3.00.dat' index i u 2:3 w l title "nu = 3.00"
# pause 0.02
# pause -1
print i
}


# instantes = "1 26 37 40 44 45"
# do for [n in instantes] {
#     i = real(n)
#     set output sprintf("results/frame%03d.pdf", i)
#     set title sprintf("t = %0.3f s", i*dt)
#     plot 'sol-burgers1D.dat' index i u 2:3 w l
#     print i
# }   
