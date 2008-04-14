#
# Convert GraphML to Yin.
#
# Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
#

class GraphML_To_Yin
  TYPE_MAP = {
    'NEURONTYPE_KBLIF' => 'Neuron_SRM01',
    'NEURONTYPE_EKERNEL' => 'Neuron_SRM02',
    'SYNAPSE_DEFAULT' => 'Synapse_Default',
    'SYNAPSE_HEBB' => 'Synapse_Hebb'
  }

  PARAM_MAP = {
    'absRefPeriod' => ['abs_refr_duration', 'real'],
    'neuronLFT' => ['last_fire_time', 'real'],
    'neuronLSET' => ['last_spike_time', 'real'],
    'neuron_tauM' => ['tau_m', 'real'],
    'neuron_tauRef' => ['tau_ref', 'real'],
    'neuron_constThreshold' => ['const_threshold', 'real'],
    'neuron_refWeight' => ['ref_weight', 'real'],
    'neuron_arpTime' => ['abs_refr_duration', 'real'],
    'synapse_weight' => ['weight', 'real'],
    'synapse_delay' => ['delay', 'real'],
    'neuronPSP' => ['mem_pot', 'real'],
    'neuronReset' => ['reset', 'real'],
    'neuron_tauM' => ['tau_m', 'real'],
    'neuron_tauRecov' => ['tau_ref', 'real'],
    'neuron_uReset' => ['u_reset', 'real'],
    'neuron_threshold' => ['const_threshold', 'real']
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
    @out.puts "ENTITY #{id} = #{entity_type} {"
    params.keys.sort.each {|k| @out.puts "  #{k} = #{params[k]}" }
    @out.puts "}"
    @out.puts
  end

  def connect(from, to)
    @out.puts "CONNECT #{from} -> #{to}"
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
    hash = {}
    data.each {|k,v|
      name, type = PARAM_MAP[k] || raise
      case type
      when 'real'
        hash[name] = v.strip.to_f
      else
        raise
      end
    }
    return hash
  end

end

if __FILE__ == $0
  $LOAD_PATH.unshift File.dirname(__FILE__)
  GraphML_To_Yin.new.convert(STDIN, STDOUT)
end
