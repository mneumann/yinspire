require 'yinspire_ext'

module Yinspire
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
