def parse(filename)
  a = {} 
  File.readlines(filename).each do |line|
    id, time = line.split("!", 2)
    next if time.nil?
    (a[id.strip] ||= []) << time.to_f
  end
  a.each_value {|arr| arr.sort!}
  return a
end

def maximum(a)
  a.map {|k,v| v.max}.max
end

a = parse(ARGV.shift)
b = parse(ARGV.shift)

max = [maximum(a), maximum(b)].max

as = a.inject(0) {|s, kv| s + kv[1].size} 
bs = b.inject(0) {|s, kv| s + kv[1].size} 
max_sz = [as, bs].max

STDERR.puts "Total size differs: #{((as - bs).abs * 100.0 / max_sz)} %"
raise "keys failed" if a.keys.sort != b.keys.sort

grand_total_err = 0.0

for k in a.keys

  diff_sz = (a[k].size - b[k].size).abs
  if diff_sz > 3
    STDERR.puts "number of fires differs too much for key #{k} (#{diff_sz})"
  end

  err = 0.0  
  n = [a[k].size, b[k].size].min

  for i in 0 ... n 
    err += (a[k][i] - b[k][i]).abs
  end

  total_err = err / n
  grand_total_err += total_err
end

grand_total_err /= a.keys.size

STDERR.puts "grand total error: #{grand_total_err}"
