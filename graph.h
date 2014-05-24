// Victor Guerra <vguerra@gmail.com>
// Nov 2014

// A graph object has the following structure:

// There is a vector '_VertexValue' where each vertice's value is stored ( this
// will store the total path cost calculated by the shortest path algo to reach
// the
// given vertex from the source vertex.)

// The edges are presented by a vector of sets '_Edge'. Given an edge (i, j): i
// is used to index the vector and j is stored in the given set to indicate that
// an edge connects i and j.
//
// Given the fact that we are dealing with an undirected graph, when
// manipulating edges, they are always normilized in a way that i is always
// less than j. That means that when setting a weight or length for the edge
// 5,3; internally we set a weight for the edge 3,5. This normalization allows
// us to save space when representing edges, e.g. the edges (3,5) and (5,3) have
// as normilized representation : 3,5. - For storing the edge value we used a
// map 'EdgeValue' where the key is a pair that represents the edge(in the
// normalized form).

#include "pq.h"

#include <vector>
#include <map>
#include <set>
#include <string>

class Graph {
  public:
  Graph(unsigned int V);  // Constructor that initializes a graph for a given
                          // number of vertex.
  Graph(unsigned int V, double EdgeDensity, unsigned int lowerDistance,
        unsigned int upperDistance);  // Constructor that initializes a graph
  // given a density and a distance range to choose the value of the edge from
  Graph(const Graph &other);  // Copy Constructor
  Graph(const char *file_path);

  ~Graph();
  unsigned int V() const { return _V; }  // Returns the count of vertex.
  unsigned int E() const { return _E; }  // Returns the count of edges.

  inline bool adjacent(unsigned int source, unsigned int tail)
      const;  // Proofs if two given vertex are adjacent
  std::set<unsigned int> neighbors(unsigned int vertex) const;  // Returns a set
                                                                // of neighbors
  void add(unsigned int source,
           unsigned int tail);  // Adds an edge between two vertices.
  void del(unsigned int source,
           unsigned int tail);  // Deletes the edge between two given vertex.
  unsigned int get_vertex_value(unsigned int vertex) const;  // Returns the
                                                             // value assigned
                                                             // to a vertex.
  void set_vertex_value(unsigned int vertex,
                        unsigned int value);  // Sets the value assgined to a
                                              // vertex.
  unsigned int get_edge_value(unsigned int source,
                              unsigned int tail);  // Returns the distance value
                                                   // assigned to a given edge.
  void set_edge_value(unsigned int source, unsigned int tail,
                      unsigned int value);  // Sets a given edge value.
  std::string toString() const;             // Pretty print for a graph.

  // algorithms

  unsigned int mst(unsigned int source, std::set<unsigned> &mst_vertices);

  private:
  unsigned int _V;
  unsigned int _E;
  std::vector<unsigned int> _VertexValue;
  std::vector<std::set<unsigned int>> _Edge;
  std::map<std::pair<unsigned int, unsigned int>, unsigned int> _EdgeValue;

  void initVertices();
  inline void normalizeIndeces(unsigned int &source, unsigned int &tail) const;
  void randomInit(double EdgeDensity, unsigned int lowerDistance,
                  unsigned int upperDistance);
  void add_neighbors_to_queue(unsigned int source, MinPriorityQueue &pq);
};

// prototypes
std::ostream &operator<<(std::ostream &out, const Graph &graph);
