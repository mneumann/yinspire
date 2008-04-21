#
# Convert Netfile (from Gereon's Inspire simulator) to Yin.
#
# Copyright (c) 2008 by Michael Neumann (mneumann@ntecs.de)
#

class Netfile_To_Yin
  def initialize
    @syn_cnt = 0
  end

  def neuron_type(out, type, rest)
    typename, tau_m, ref_weight, tau_ref, 
      abs_refr_duration, const_threshold, *r = rest  
    raise unless r.empty?

    out.puts "TEMPLATE #{typename} < #{type} {"
    out.puts "  tau_m = #{tau_m}"
    out.puts "  ref_weight = #{ref_weight}"
    out.puts "  tau_ref = #{tau_ref}"
    out.puts "  abs_refr_duration = #{abs_refr_duration}"
    out.puts "  const_threshold = #{const_threshold}"
    out.puts "}"
  end

  def convert(input, out=STDOUT)
    input.each_line do |line|
      next if line.strip.empty?

      cmd, *rest = line.split
      case cmd
      when 'NEURONTYPE', 'NEURONTYPE_KBLIF'
        neuron_type(out, 'Neuron_SRM01', rest)
      when 'NEURONTYPE_EKERNEL'
        neuron_type(out, 'Neuron_SRM02', rest)
      when 'NEURONS'
        id, typename, n, *r = rest 
        raise unless r.empty?

        if n
          out.print "ENTITY "
          out.print((1 .. Integer(n)).map {|i| "#{id}#{i}"}.join(", "))
          out.puts " = #{typename}"
        else
          out.puts "ENTITY #{id} = #{typename}"
        end
      when 'SYNAPSETYPE'
        typename, weight, delay, *r = rest
        raise unless r.empty?

        out.puts "TEMPLATE #{typename} < Synapse_Default {"
        out.puts "  weight = #{weight}"
        out.puts "  delay = #{delay}"
        out.puts "}"
      when 'CONNECT'
        from_n = []
        to_n = []
        syn_type = nil
        state = 0
        rest.each do |r|
          case state
          when 0
            if r == 'TO'
              state = 1
            else
              from_n << r
            end
          when 1
            if r == 'SYNAPSES'
              state = 2
            else
              to_n << r
            end
          when 2
            syn_type = r
            state = 3
          when 3
            raise
          end
        end

        raise unless syn_type
        raise if from_n.empty? or to_n.empty?

        from_n.each do |from|
          to_n.each do |to|
            i = (@syn_cnt += 1)
            out.puts "ENTITY syn_#{i} = #{syn_type}"
            out.puts "CONNECT #{from} -> syn_#{i} -> #{to}"
          end
        end

      else
        raise
      end
    end
  end

end

if __FILE__ == $0
  Netfile_To_Yin.new.convert(STDIN, STDOUT)
end
