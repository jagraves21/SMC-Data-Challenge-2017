reset
set datafile separator ","

set title "Community Evaluation"

set xlabel "Communities (K)"

set ylabel "Modularity"

set style line 4 lt rgb "red"  lw 4 pt 7
set style line 5 lt rgb "blue" lw 4 pt 5

set terminal postscript eps color
set output "modularity.eps"

set key autotitle columnhead

plot "results.csv" using 1:2 with linespoints linestyle 4 ,\
     "results.csv" using 1:3 with linespoints linestyle 4

