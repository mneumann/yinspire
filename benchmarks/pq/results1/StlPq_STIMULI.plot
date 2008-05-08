
    set terminal pdf
    # dashed
    set output "work/StlPq_STIMULI.pdf"
    set xlabel "Queue size (ld)"
    set ylabel "Mean access time (ns)"
    set format x "%L"
    set logscale x 2
    #set logscale y 2
    set grid x y
    set size ratio 1
    set data style linespoints
    set key top left
    set key box
  plot   "work/StlPq_STIMULI.dat" index 0 using 1:2 title "Random",\
  "work/StlPq_STIMULI.dat" index 1 using 1:2 title "Exponential(1)",\
  "work/StlPq_STIMULI.dat" index 2 using 1:2 title "Uniform(0,2)",\
  "work/StlPq_STIMULI.dat" index 3 using 1:2 title "Triangular(0,1.5)",\
  "work/StlPq_STIMULI.dat" index 4 using 1:2 title "NegativeTriangular(0,1.5)"
