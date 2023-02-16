set xrange [0:3]
set yrange [40:60]
set xlabel "t"
set ylabel "U"
# do for [i=40:60:2] {
A = 3.5
mu = 50
b = 0.05

unset key
plot for [i=40:60] A*exp(-b*(i-mu)**2)*x + i 