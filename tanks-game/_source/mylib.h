#pragma once
#include <algorithm> //std::min_element, std::max_element, std::find
#include <concepts>

template<typename T>
inline T findMin(const T* arr, int len) { return *std::min_element(arr, arr + len); }
template<typename T>
inline T findMax(const T* arr, int len) { return *std::max_element(arr, arr + len); }
template<typename T>
inline int findMinIndex(const T* arr, int len) { return std::distance(arr, std::min_element(arr, arr + len)); }
template<typename T>
inline int findMaxIndex(const T* arr, int len) { return std::distance(arr, std::max_element(arr, arr + len)); }

constexpr bool XOR(bool a, bool b) {
	return (a != b);
	//return ((a || b) && !(a && b));
}

//from: https://github.com/nicolausYes/iterator-template-sort-library/blob/master/src/InsertionSort.h
template< class RandomAccessIterator >
inline void InsertionSort( RandomAccessIterator first, RandomAccessIterator last ) {
	InsertionSort( first, last, std::less< typename std::iterator_traits<RandomAccessIterator>::value_type >() );
}
template< class RandomAccessIterator, class Compare >
void InsertionSort( RandomAccessIterator first, RandomAccessIterator last, Compare comp ) {
	for( auto i = first; i != last; i++ ) {
		for( auto j = i; j != first; j-- ) {
			if( comp( *j, *(j-1) ) )
				std::swap( *j, *(j-1) );
			else
				break;
		}
	}
}

//algorithm from https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
bool pointInPolygon(int vertNum, const double* vertX, const double* vertY, double testX, double testY);
bool pointInPolygon(int vertNum, const float* vertX, const float* vertY, float testX, float testY);

template<typename T>
inline bool isInArray(T value, const T* arr, int length) {
	return (std::find(arr, arr + length, value) != (arr + length));
}

template<std::floating_point T>
int weightedSelect(const T* weights, int num);
