// Victor Guerra <vguerra@gmail.com>
// Assignment 02 - C++ for C Programmers Coursera.
// 23 Oct 2013

#include "pq.h"

#include <algorithm>
#include <iostream>

bool edge_t_comp(edge_t e1, edge_t e2) { return (e1._cost > e2._cost); }

MinPriorityQueue::MinPriorityQueue() : _pq() {
  std::make_heap(_pq.begin(), _pq.end(), edge_t_comp);
}

inline size_t MinPriorityQueue::size() const { return _pq.size(); }

bool MinPriorityQueue::empty() const { return (_pq.size() == 0); }

edge_t MinPriorityQueue::top() const { return _pq[0]; }

void MinPriorityQueue::pop() {
  std::pop_heap(_pq.begin(), _pq.end(), edge_t_comp);
  _pq.pop_back();
}

void MinPriorityQueue::push(edge_t edge) {
  _pq.push_back(edge);
  std::push_heap(_pq.begin(), _pq.end(), edge_t_comp);
}
