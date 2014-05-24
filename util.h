// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#ifndef UTIL_H_
#define UTIL_H_

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif  // UTIL_H_
