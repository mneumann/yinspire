class Compare_EventLists
  def compare(fa, fb)
    res = {}
    a = parse(fa)
    b = parse(fb)

    max = [maximum(a), maximum(b)].max

    as = a.inject(0) {|s, kv| s + kv[1].size} 
    bs = b.inject(0) {|s, kv| s + kv[1].size} 
    max_sz = [as, bs].max

    raise "keys failed" if a.keys.sort != b.keys.sort
    res[:total_size_diff_percentage] = ((as - bs).abs * 100.0 / max_sz)

    grand_total_err = 0.0

    for k in a.keys
      err = 0.0  
      n = [a[k].size, b[k].size].min

      for i in 0 ... n 
        err += (a[k][i] - b[k][i]).abs
      end

      total_err = err / n
      grand_total_err += total_err
    end

    grand_total_err /= a.keys.size

    res[:grand_total_error] = grand_total_err

    res
  end

  private

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
end

if __FILE__ == $0
  require 'pp'
  pp Compare_EventLists.new.compare(*ARGV)
end
