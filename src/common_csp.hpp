/*
 * csp-common.hpp
 *
 *  Created on: May 17, 2013
 *      Author: soroush
 */

#ifndef COMMON_CSP_HPP_
#define COMMON_CSP_HPP_

#include <set>
#include <vector>
#include <algorithm>

namespace AIT {

template<typename T, typename V>
class Assignment {
public:
	Assignment();
	Assignment(T* instance, const T& value);
	bool operator ==(const Assignment& rhs);
	bool operator !=(const Assignment& rhs);
private:
	T* instance;
	V value;
};

template<typename T, typename V>
class CompoundAssignment {
public:
	CompoundAssignment();
	void add(const Assignment<V, T>&);
private:
	std::vector<Assignment<V, T>> assignments;
};

}

template<typename T, typename V>
AIT::Assignment<T, V>::Assignment(T* instance_, const T& value_) :
		instance(instance_), value(value_) {
}

template<typename T, typename V>
inline bool AIT::Assignment<T, V>::operator ==(const Assignment& rhs) {
	if (rhs.instance == this->instance && rhs.value == this->value)
		return true;
	return false;
}

template<typename T, typename V>
inline bool AIT::Assignment<T, V>::operator !=(const Assignment& rhs) {
	return !(*this == rhs);
}

template<typename T, typename V>
inline void AIT::CompoundAssignment<T, V>::add(const Assignment<V, T>& item) {
	if (std::find(this->assignments.begin(), this->assignments.end(), item)
			!= this->assignments.end()) {
		this->assignments.push_back(item);
	}
}

// end of namespace

#endif /* COMMON_CSP_HPP_ */
