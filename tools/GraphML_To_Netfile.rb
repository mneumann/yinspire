#
# Convert GraphML to Netfile.
#
# Copyright (c) 2008 by Michael Neumann (mneumann@ntecs.de)
#

class GraphML_To_Netfile
  TYPE_MAP = {
    'NEURONTYPE_KBLIF' => 'NEURONTYPE_KBLIF',
    'NEURONTYPE_EKERNEL' => 'NEURONTYPE_EKERNEL',
    'SYNAPSE_DEFAULT' => 'SYNAPSETYPE',
  }

  # MAPS GraphML param name to Netfile position
  PARAM_MAP = {
    'absRefPeriod' => 4,
    'neuron_tauM' => 1,
    'neuron_tauRef' => 3,
    'neuron_constThreshold' => 5,
    'neuron_refWeight' => 2,
    'neuron_arpTime' => 4,
    'synapse_weight' => 1,
    'synapse_delay' => 2, 
    'neuron_tauRecov' => 3,
    'neuron_threshold' => 5
  }

  def convert(input, out=STDOUT)
    @out = out
    require 'GraphML'
    gml = GraphML.parse(input)
    g = gml.graphs.values.first
    default_neuron_type = g.data['graph_default_neuron_type']
    default_synapse_type = g.data['graph_default_synapse_type']

    #
    # Create Neurons
    #
    g.nodes.each_value {|node|
      create(node.id, node.data, default_neuron_type, 'neuron_type')
    }

    # 
    # Create Synapses
    #
    g.edges.each_value {|edge|
      create(edge.id, edge.data, default_synapse_type, 'synapse_type')
    }

    #
    # Create Connections between Neurons and Synapses.
    #
    g.edges.each_value {|edge|
      a = edge.source.id || raise
      b = edge.id || raise
      c = edge.target.id || raise
      connect(a, b)
      connect(b, c)
    }
  end

  def create_entity(entity_type, id, params)
    @out.puts "#{entity_type} __type_#{id} #{params.join(' ')}" 
    entity_type.gsub(/TYPE(.*)/, '')
    @out.puts " #{id} = #{entity_type} {"
    params.keys.sort.each {|k| @out.puts "  #{k} = #{params[k]}" }
    @out.puts "}"
    @out.puts
  end

  def connect(from, to)
    @out.puts "CONNECT #{from} TO #{to}"
  end

  protected

  #
  # Parameter +kind+ is either of "neuron_type" or "synapse_type".
  #
  def create(id, data, default_type, kind)
    entity_type = (TYPE_MAP[data[kind] || default_type] || raise)
    data.delete(kind)
    create_entity(entity_type, id, conv_params(data))
  end

  def conv_params(data)
    res = Array.new
    data.each {|k,v|
      res[(PARAM_MAP[k] || raise) - 1] = v.strip.to_f
    }
    return res
  end

end

if __FILE__ == $0
  $LOAD_PATH.unshift File.dirname(__FILE__)
  GraphML_To_Yin.new.convert(STDIN, STDOUT)
end
