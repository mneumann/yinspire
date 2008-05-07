require 'yinspire_ext'

module Yinspire
  Infinity = 1.0/0.0

  class Simulator
    attr_reader :current_time

    def initialize
      @current_time = 0.0
      if block_given?
        begin
          yield self
        ensure
          destroy
        end
      end
    end

    alias __old_run run

    def run_for(n)
      run(@current_time + n)
    end

    def run(stop_at=Infinity)
      old_current_time, @current_time = @current_time, __old_run(stop_at)
      @current_time != old_current_time
    end
  end

  class NeuralEntity
    def inspect
      hash = dump()
      str = "#<#{self.type}[#{self.id}]"
      hash.keys.sort_by {|a| a.to_s}.each do |k|
        str << "\n  #{k}: #{hash[k]}"
      end
      str << ">"
      str
    end
  end
end
