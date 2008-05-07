WARMUP_CYCLES = 10_000_000 
EMPTY_CYCLES = 100_000_000 
CYCLES = 100_000_000 

DISTRIBUTIONS = [
  'Random',
  'Exponential 1', 
  'Uniform 0 2', 
  'Triangular 0 1.5',
  'NegativeTriangular 0 1.5'
]

ALGORITHMS = [
  'BinaryHeap FLOAT',
  'BinaryHeap DOUBLE',
  'BinaryHeap STIMULI',
  'StlPq FLOAT',
  'StlPq DOUBLE',
  'StlPq STIMULI',
  'PairingHeap STIMULI',
  'CalendarQueue STIMULI'
]

REPEAT = 5
QUEUE_SIZES = (1..7).map {|x| 10**x}

def bench(queue_size, warmup_cycles, empty_cycles, cycles, distribution, algorithm) 
  cmd =  "./bench #{queue_size} #{warmup_cycles} #{empty_cycles} #{cycles} ClassicHold"
  cmd << " #{distribution} #{algorithm}"
  p cmd
  parse_result(`#{cmd}`)
end

def parse_result(str)
  h = {}
  str.split("\n").each do |line|
    line.strip!
    next if line.empty?
    key, value = line.split(":", 2)
    h[key] = value.strip
  end
  h
end

def bench_algorithm(algorithm, prefix, repeat=nil, distributions=nil, queue_sizes=nil)
  repeat ||= REPEAT
  distributions ||= DISTRIBUTIONS
  queue_sizes ||= QUEUE_SIZES

  h = {}
  repeat.times do 
    distributions.each do |distr|
      queue_sizes.each do |qs|
        res = bench(qs, WARMUP_CYCLES, EMPTY_CYCLES, CYCLES, distr, algorithm)
        h[res['Distribution']] ||= {}
        (h[res['Distribution']][res['QueueSize']] ||= []) << res['Time'].to_f
      end
    end
  end

  dat = File.open(prefix + ".dat", "w+") 
  plot = File.open(prefix + ".plot", "w+")

  plot << %{
    set terminal pdf
    # dashed
    set output "#{prefix}.pdf"
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
  }

  index = 0
  h.each do |dis, h2|
    dat.puts "# Distribution: #{dis}"
    dat.puts "# queue size    mean access time (ns)"

    if index == 0
      plot.print "plot "
    else
      plot.print ",\\\n"
    end

    plot.print %{  "#{prefix}.dat" index #{index} using 1:2 title "#{dis}"}
    index += 1

    arr = []
    h2.each do |qs, times|
      avg = (times.inject(0.0) {|i, s| s + i}) / times.size 
      avg *= 1_000_000_000 # convert to ns
      arr << [qs, avg]
    end
    arr.sort_by {|qs,_| qs.to_i}.each do |qs, avg|
      dat.print qs.to_s.ljust(11)
      dat.puts avg.to_s.ljust(24)
    end

    dat.puts
    dat.puts
  end

  plot.puts
  dat.close
  plot.close

  system "gnuplot #{prefix}.plot"
end 

Dir.mkdir('work') rescue nil

qs = (14..22).map {|x| 2**x}
#bench_algorithm("BinaryHeap STIMULI", "work/bh", 2, ["Random"], qs)
#bench_algorithm("StlPq STIMULI", "work/stl", 2, ["Random"], qs)
#bench_algorithm("BinaryHeap STIMULI", "work/test5", 2, ["Random"], [2**18, 2**19, 2**20])
#bench_algorithm("PairingHeap STIMULI", "work/pairing", 1, ["Random"], [2**10, 2**14, 2**18])
#bench_algorithm("BinaryHeap STIMULI", "work/binary", 1, ["Random"], [2**10, 2**14, 2**18])
#bench_algorithm("BinaryHeap STIMULI", "work/binary", 1, ["Random"], [2**10, 2**14, 2**18])
bench_algorithm("CalendarQueue STIMULI", "work/cq3", 1, ["Random"], [2**10, 2**14, 2**18, 2**20, 2**21])
