// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "graph.h"

#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <sstream>
#include <fstream>

// Implementation of Graph class

void Graph::randomInit(double EdgeDensity, unsigned int lowerDistance,
                       unsigned int upperDistance) {
  unsigned int distance_value;
  srand(static_cast<unsigned>(time(NULL)));
  for (unsigned int i = 0; i < _V; ++i) {
    for (unsigned int j = i + 1; j < _V; ++j) {
      if (rand() <= RAND_MAX * EdgeDensity) {
        distance_value = (static_cast<unsigned>(rand()) %
                          (upperDistance - lowerDistance + 1)) +
                         lowerDistance;
        add(i, j);
        set_edge_value(i, j, distance_value);
      }
    }
  }
}

inline void Graph::normalizeIndeces(unsigned int &source,
                                    unsigned int &tail) const {
  if (tail < source) {
    source = source xor tail;
    tail = source xor tail;
    source = source xor tail;
  }
}

void Graph::initVertices() {
  _E = 0;
  _VertexValue.resize(_V);
  for (unsigned i = 0; i < _V; ++i) {
    _VertexValue.at(i) = 0;
    _Edge.push_back(*(new std::set<unsigned int>()));
  }
}

Graph::Graph(unsigned int V) : _V(V) { initVertices(); }

Graph::Graph(unsigned int V, double EdgeDensity, unsigned int lowerDistance,
             unsigned int upperDistance)
    : _V(V) {
  initVertices();
  if (EdgeDensity > 0 && EdgeDensity <= 1.0 && upperDistance > lowerDistance) {
    randomInit(EdgeDensity, lowerDistance, upperDistance);
  } else {
    std::cout << "Wrong values for intialization parameters" << std::endl;
  }
}

Graph::Graph(const Graph &other)
    : _V(other._V),
      _E(other._E),
      _VertexValue(other._VertexValue),
      _Edge(other._Edge),
      _EdgeValue(other._EdgeValue) {}

Graph::Graph(const char *file_path) {
  std::ifstream graph_file(file_path, std::ios::in);
  if (graph_file.is_open()) {
    std::string line;
    graph_file >> _V;
    initVertices();
    while (!graph_file.eof()) {
      unsigned int source, tail, weight;
      graph_file >> source >> tail >> weight;
      add(source, tail);
      set_edge_value(source, tail, weight);
    }
    graph_file.close();
  }
}

Graph::~Graph() {}

bool Graph::adjacent(unsigned int source, unsigned int tail) const {
  unsigned int v1 = source, v2 = tail;
  normalizeIndeces(v1, v2);
  return (_Edge.at(v1).find(v2) != _Edge.at(v1).end());
}

std::set<unsigned int> Graph::neighbors(unsigned int vertex) const {
  return _Edge.at(vertex);
}

void Graph::add(unsigned int source, unsigned int tail) {
  if (source != tail) {
    normalizeIndeces(source, tail);

    if (_Edge.at(source).find(tail) == _Edge.at(source).end()) {
      _Edge.at(source).insert(tail);
      _Edge.at(tail).insert(source);
      ++_E;
    }
  }
}

void Graph::del(unsigned int source, unsigned int tail) {
  normalizeIndeces(source, tail);
  _Edge.at(source).erase(tail);
}

void Graph::set_vertex_value(unsigned int vertex, unsigned int value) {
  _VertexValue[vertex] = value;
}

unsigned int Graph::get_vertex_value(unsigned int vertex) const {
  return _VertexValue[vertex];
}

void Graph::set_edge_value(unsigned int source, unsigned int tail,
                           unsigned value) {
  if (source != tail) {
    normalizeIndeces(source, tail);
    _EdgeValue[std::make_pair(source, tail)] = value;
  }
}

unsigned int Graph::get_edge_value(unsigned int source, unsigned int tail) {
  std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator it;
  normalizeIndeces(source, tail);
  it = _EdgeValue.find(std::make_pair(source, tail));
  if (it != _EdgeValue.end())
    return it->second;
  else
    return 0;
}

std::string Graph::toString() const {
  std::ostringstream str_stream;
  str_stream << "Graph (" << _V << ", " << _E << "):" << std::endl;
  str_stream << "Vertices (" << _VertexValue.size() << "): " << std::endl;
  for (unsigned i = 0; i < _V; ++i) {
    str_stream << i << "(" << _VertexValue.at(i) << ")" << std::endl;
  }
  str_stream << "Edges: " << std::endl;
  for (unsigned i = 0; i < _V; ++i) {
    str_stream << "Vertex " << i << " : ";
    for (std::set<unsigned int>::iterator it = _Edge.at(i).begin();
         it != _Edge.at(i).end(); ++it) {
      str_stream << *it << " ";
    }
    str_stream << std::endl;
  }
  str_stream << std::endl;
  return str_stream.str();
}

std::ostream &operator<<(std::ostream &out, const Graph &graph) {
  out << graph.toString();
  return out;
}

void Graph::add_neighbors_to_queue(unsigned int source, MinPriorityQueue &pq) {
  std::set<unsigned int> vertex_neighbors;
  std::set<unsigned int>::iterator vertex_it;

  vertex_neighbors = neighbors(source);

  for (vertex_it = vertex_neighbors.begin();
       vertex_it != vertex_neighbors.end(); ++vertex_it) {
    unsigned edge_value = get_edge_value(source, *vertex_it);
    edge_t edge1(source, *vertex_it, edge_value);
    edge_t edge2(*vertex_it, source, edge_value);
    pq.push(edge1);
    pq.push(edge2);
  }
}

// This procedure computes the Minimung Spanning Tree (MST).

// Prints out the edges that compose the MST and returns the sum
// of its costs.

// There is a set 'mst_vertices' that keeps track of vertices that
// we reach by the edges on the MST, the idea is to
// expand this set, till all the vertices are included. This code
// assumes that we are working only with connected graphs.

// A priority queue 'sorted_edges' has the avialable edges we can reach
// according to those vertices already reached by the MST edges, sorted
// by cost. The edge at the top of the PQ tells us what edge
// to take next iff the vertex that that edge would reach is not
// in the mst_vertices set already.

// Everytime a vertex is added to the 'mst_vertices' all edges
// are take into account for the next run (adding the edges to the
//   'sorted_edges' PQ.)

// The total cost is returned and the edges choosen for the MST are
// printed out to the stdout.

unsigned int Graph::mst(unsigned int source, std::set<unsigned> &mst_vertices) {
  unsigned int cost = 0;
  MinPriorityQueue sorted_edges;
  mst_vertices.clear();

  mst_vertices.insert(source);
  add_neighbors_to_queue(source, sorted_edges);

  while (mst_vertices.size() != V() && !sorted_edges.empty()) {
    edge_t edge = sorted_edges.top();
    sorted_edges.pop();

    if (mst_vertices.find(edge._v) != mst_vertices.end() &&
        mst_vertices.find(edge._w) == mst_vertices.end()) {
      // std::cout << "edge: "
      //           << "(" << edge._v << ", " << edge._w << ", " << edge._cost
      //           << ")" << std::endl;
      cost += edge._cost;
      mst_vertices.insert(edge._w);

      add_neighbors_to_queue(edge._w, sorted_edges);
    }
  }
  // std::cout << "MST Cost: " << cost << std::endl;
  return cost;
}
