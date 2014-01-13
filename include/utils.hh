#ifndef GODB_UTILS_HH_
#define GODB_UTILS_HH_

// SFINAE bg =D
template <typename T>
class hasIterator
{
  typedef char true_type;
  struct false_type { true_type _[2]; };

  template <typename T1>
  static true_type	_result(typename T1::iterator *);

  template <typename T1>
  static false_type	_result(...);

public:
  static const bool	result = (sizeof(true_type) == sizeof(_result<T>(NULL)));
};

#endif
