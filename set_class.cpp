"""
Implement Set class and its operations in C++
"""

#include "tuple.h"
#include "set.h"
#include "invariant.h"
#include <iostream>
#include <limits.h>   // needed for INT_MIN

// Set up an error set to be returned as necessary
const Set emptySet(0,(int*)0);
const Set errorSet(-1,(int*)0);

bool Set::isEmpty() const {
	return _numElements== 0;
}

bool Set::isError() const {
	if (_numElements == -1)
		return true;
	return false;
}
  
#include "tuple.h"
#include "set.h"
#include "invariant.h"
#include <iostream>
#include <limits.h>   // needed for INT_MIN

// Set up an error set to be returned as necessary
const Set emptySet(0,(int*)0);
const Set errorSet(-1,(int*)0);

bool Set::isEmpty() const {
  if (  _numElements < 0) {
    std::cerr << "Error: This is an error set. You are not supposed to do any work with it" << std::endl;
    return false;
  }
  if (_numElements == 0) return true;
  else return false;
}

bool Set::isError() const {
  if (_numElements < 0) return true;
  if (_numElements > 0 && _pTuples == NULL) return true;
  if (_numElements == 0 && !_pTuples) return true;
  if (_tupleArraySize <= 0) return true;
  return false;
}

int Set::cardinality() const {
  if (_numElements < 0) {
    std::cerr << "Error: This is an error set. You are not supposed to do any work with it" << std::endl;
    return -1;
  }
  return _numElements;
}

Set Set::union_(const Set& s) const {
  if (s._numElements < 0 || s._pTuples == NULL) {
    std::cerr << "Error: Error set passed into the function. Terminating union" << std::endl;
    return errorSet;
  }
  Tuple* ts = new Tuple[_numElements + s._numElements];
  int tsIndex = 0;
  for (int i=0; i<_numElements; i++) {
    ts[tsIndex] = _pTuples[i];
    tsIndex++;
  }

  for (int j=0; j<s._numElements; j++) {
    bool isIn = false;
    for (int i=0; i<_numElements; i++) {
      if (_pTuples[i] == s._pTuples[j]) {
        isIn = true;
      }
    }
    if (!isIn) {
      ts[tsIndex] = s._pTuples[j];
      tsIndex++;
    }
  }
  return Set(tsIndex, ts);
}

Set Set::intersection(const Set& s) const {
  if (s._numElements < 0 || s._pTuples == NULL) {
    std::cerr << "Error: Error set passed into the function. Terminating instersect" << std::endl;
    return errorSet;
  }
  Tuple* ts = new Tuple[s._numElements];
  int tsIndex = 0;
  for (int j=0; j<s._numElements; j++) {
    bool isIn = false;
    for (int i=0; i<_numElements; i++) {
      if (_pTuples[i] == s._pTuples[j]) {
        isIn = true;
      }
    }
    if (isIn) {
      ts[tsIndex] = s._pTuples[j];
      tsIndex++;
    }
  }
  return Set(tsIndex, ts);
}

Set Set::difference(const Set& s) const {
  if (s._numElements < 0 || s._pTuples == NULL) {
    std::cerr << "Error: Error set passed into the function. Terminating difference" << std::endl;
    return errorSet;
  }
  Tuple* ts = new Tuple[_numElements];
  int tsIndex = 0;
  for (int i=0; i<_numElements; i++) {
    bool isIn = false;
    for (int j=0; j<s._numElements; j++) {
      if (_pTuples[i] == s._pTuples[j]) {
        isIn = true;
      }
    }
    if (!isIn) {
      ts[tsIndex] = _pTuples[i];
      tsIndex++;
    }
  }
  return Set(tsIndex, ts);
}

Set Set::select(predicate* p) const {
  if (p == NULL) {
    std::cerr << "Error: Error function passed into the function. Terminating select" << std::endl;
    return errorSet;
  }
  Tuple* ts = new Tuple[_numElements];//could be improved!!!///
  int tsIndex = 0;
  for (int i=0; i<_numElements; i++) {
    if(p(_pTuples[i])) {
      ts[tsIndex] = _pTuples[i];
      tsIndex++;
    };
  }
  return Set(tsIndex, ts);
}


Set Set::project(const int numItems, const int items[]) const {
  if (numItems < 0 || items == NULL) {
    std::cerr << "Error: Error function passed into the function. Terminating select" << std::endl;
    return errorSet;
  }
  if (_numElements == 0) {
    std::cerr << "Error: This is an empty set. Cannot be projected" << std::endl;
    return emptySet;
  }
  for (int i=0; i<numItems; i++) {
    if (_tupleArraySize < items[i]){
      std::cerr << "Error: Cannot project. Index exceed tuple array size" << std::endl;
      return errorSet;
    }
  }
  Tuple* ts = new Tuple[_numElements];
  int tsIndex = 0;
  for (int i=0; i<_numElements; i++) {
    Tuple tmpt = _pTuples[i].project(numItems, items);
    bool isIn = false;
    for (int j=0; j<tsIndex; j++) {
      if(ts[j] == tmpt) isIn = true;
    }
    if (!isIn) {
      ts[tsIndex] = tmpt;
      tsIndex++;
    }
  }
  return Set(tsIndex, ts);
}

Set Set::cartesian(const Set& s) const {
  if (s._numElements < 0) {
    std::cerr << "Error: the set is empty, cannot take the cartesian product" << std::endl;
    return emptySet;
  }

  int setElements = _numElements * s._numElements;
  int setTupleArraySize = _tupleArraySize + s._tupleArraySize;
  Tuple* ts = new Tuple[setElements];
  int tIndex = 0;
  for (int i=0; i<_numElements; i++) {
    for (int j=0; j<s._numElements; j++) {
      int data[setTupleArraySize];
      int k=0;
      for (int m=0; m<_tupleArraySize; m++) {
        data[k] = _pTuples[i](m+1);
        k++;
      };
      for (int n=0; n<s._tupleArraySize; n++) {
        data[k] = s._pTuples[j](n+1);
        k++;
      };
      ts[tIndex] = Tuple(setTupleArraySize, data);
      tIndex++;
    }
  }
  return Set(setElements, ts);
}

Set Set::operator()(const int item) const {
  if (item > _numElements) {
    std::cerr << "Error: Index exceed the element size of this set" << std::endl;
    return errorSet;
  }
  Tuple* ts = new Tuple[1];
  ts = &(_pTuples[item-1]);
  return Set(1, ts);
}

void Set::operator=(const Set& s) {
  if (s._numElements < 0) {
    std::cerr << "Error: Error set passed into the function. Terminating operator=" << std::endl;
    return;
  }
  if (s._numElements == 0) {
    _numElements = 0;
    _tupleArraySize = 0;
    _pTuples = NULL;
  } else {
    Tuple* ts = new Tuple[s._numElements];
    for (int i=0; i<s._numElements; i++) {
      ts[i] = s._pTuples[i];
    }
    _numElements = s._numElements;
    _tupleArraySize = s._tupleArraySize;
    _pTuples = ts;
  }
}

Set::Set() {
  _numElements = 0;
  _tupleArraySize = 0;
  _pTuples = NULL;
}

Set::Set(const Set& s) {
  if (s._numElements < 0) {
    std::cerr << "Error: Error set passed into constructor. Terminating" << std::endl;
  }
  if (s._numElements > 0 && s._pTuples == NULL) {
    std::cerr << "Error: Error set passed into constructor. Terminating" << std::endl;
  }
  _numElements = s._numElements;
  _tupleArraySize = s._tupleArraySize;
  _pTuples = s._pTuples;
}

Set::Set(const int numElements, const int data[]) {
  if (numElements < 0) {
    std::cerr << "Error: Invalid constructor argument" << std::endl;
  } else if (numElements == 0) {
    Set();
  } else {
    int data_cleaned[numElements];
    int dcIndex = 0;
    for (int i=0; i<numElements; i++) {
      bool flag = false;
      for (int j=0; j<i; j++) {
        if(data[i] == data[j]) flag = true;
      }
      if (!flag) {
        data_cleaned[dcIndex] = data[i];
        dcIndex++;
      }
    }
    _numElements = dcIndex;
    _tupleArraySize = 1;
    _pTuples = new Tuple[dcIndex];
    for (int i=0; i<dcIndex; i++) {
      _pTuples[i] = Tuple(data_cleaned[i]);
    }
  }
}

Set::Set(const int numElements, const Tuple tuples[]) {
  if (numElements < 0) {
    std::cerr << "Error: Cannot construct set. numElements cannot be zero" << std::endl;
  }
  if (numElements == 0) {
    _numElements = 0;
    _tupleArraySize = 0;
    _pTuples = NULL;
  } else {
    _numElements = numElements;
    _tupleArraySize = tuples[0].size();
    _pTuples = new Tuple[numElements];
    for (int i=0; i<numElements; i++) {
      _pTuples[i] = tuples[i];
    }
  }
}

Set::~Set() {
  if (_numElements > 0) delete[] _pTuples;
}

std::ostream& operator<<(std::ostream& os, const Set& s) {
#ifdef DEBUG
  std::cerr << "*** " << t._numItems << " ***" << std::endl;
#endif
  if(s._numElements <= 0) {
    os << "{}";
    return os;
  } else if (s._numElements > 0) {
    os << "{";
    Tuple ts[s._numElements];
    for (int i=0; i<s._numElements; i++) {
      ts[i] = s._pTuples[i];
      os << ts[i];
      if (i < s._numElements - 1) os << ",";
    }
    os << "}";
    return os;
  } else {
    std::cerr << "Error: Attempted to output deleted set, aborting this task";
    exit(-1);
  }
}
