// Victor Guerra <vguerra@gmail.com>
// Assignment 02 - C++ for C Programmers Coursera.
// 23 Oct 2013

// Declaration for the Priority Queue

// The priority queue implementation relies on a vector and the
// functionality to create and manage heaps provided by the
// algorithm stl (std::make_heap, std::push_heap, std::pop_head , etc.)

// The elements of our vector '_pq' are pairs that represent a priority
// (in this case the distance to reach a given vertex and the vertex).
// By using std::make_heap we make a max heap out of the vector.
// We use a very simple hack to convert a max-heap into a min-heap by
// just negating the distance. The lowest distances have bigger negative
// values that higher distances.

#include <vector>
#include <utility>

typedef struct edge_t {
  unsigned _v;
  unsigned _w;
  unsigned _cost;
  edge_t(unsigned v, unsigned w, unsigned cost) : _v(v), _w(w), _cost(cost) {}
} edge_t;

bool edge_t_comp(edge_t e1, edge_t e2);

class MinPriorityQueue {
 public:
  MinPriorityQueue();          // Default Constructor
  inline size_t size() const;  // Returns the size of elements on the priority
                               // queue.
  bool empty() const;  // Tells whether the priority queue is empty or not.
  edge_t top() const;  //
  void pop();          // Removes the top of the priority queue.
  bool contains(edge_t edge) const;  // Checks if a given value is
                                     // contained already.
  void push(edge_t edge);            // Pushes a value with the given priority.
  // void change_priority(unsigned int priority,
  //                      unsigned int value);  // Changes the priority of a
  // given
  //                                            // value already stored into the
  //                                            // priority queue.

 private:
  std::vector<edge_t> _pq;
};
