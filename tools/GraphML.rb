#
# Parses GraphML.
#
# Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
#

require 'rexml/document'

class GraphML

  class Graph < Struct.new(:id, :data, :nodes, :edges); end
  class Node < Struct.new(:id, :data); end
  class Edge < Struct.new(:id, :data, :source, :target); end

  attr_reader :graphs

  def self.parse(io)
    doc = REXML::Document.new(io)
    new().parse(doc)
  end

  def initialize
    @keys = Hash.new
    @graphs = Hash.new
    @ids = Hash.new
  end

  def parse(doc)
    parse_keys(doc.root)
    parse_graphs(doc.root)
    return self
  end

  private 

  def check_id(id)
    raise "duplicate id: #{ id }" if @ids.has_key?(id)
    @ids[id] = true
    id
  end

  def parse_keys(root)
    root.elements.each('key') do |el|
      id = check_id(el.attributes['id'])
      (@keys[el.attributes['for']] ||= {})[id] = true
    end
  end

  def parse_graphs(root)
    root.elements.each('graph') do |g|
      graph = GraphML::Graph.new
      graph.id = check_id(g.attributes['id'])
      graph.data = parse_data(g, 'graph')
      graph.nodes = Hash.new
      graph.edges = Hash.new
      @graphs[graph.id] = graph

      g.elements.each('node') do |el|
        node = GraphML::Node.new
        node.id = check_id(el.attributes['id'])
        node.data = parse_data(el, 'node')
        graph.nodes[node.id] = node
      end

      g.elements.each('edge') do |el|
        # FIXME: for now always assume directed edges
        edge = GraphML::Edge.new
        edge.id = check_id(el.attributes['id'])
        edge.source = graph.nodes[el.attributes['source']] || raise
        edge.target = graph.nodes[el.attributes['target']] || raise
        edge.data = parse_data(el, 'edge')
        graph.edges[edge.id] = edge
      end
    end
  end

  def parse_data(root, for_el)
    hash = Hash.new
    root.elements.each('data') do |el|
      key, value = el.attributes['key'], el.text
      raise unless @keys[for_el][key]
      hash[key] = value
    end
    hash
  end
end

if __FILE__ == $0
  p GraphML.parse(File.open("../examples/nets/skorpion.graphml"))
end
