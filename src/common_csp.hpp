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

#include "protocols.pb.h"
#include "common_async.hpp"

namespace AIT {

typedef unsigned char byte;

template<typename V, typename T>
class Assignment {
public:
	Assignment();
	Assignment(V variable, const T& value, const AgentID& id,
			const int32_t& pureValue=0);
	bool operator ==(const Assignment& rhs);
	bool operator !=(const Assignment& rhs);
private:
	V instance;
	T value;
};

template<typename V, typename T>
class CompoundAssignment {
public:
	CompoundAssignment();
	void add(const Assignment<V, T>&);
private:
	std::vector<Assignment<V, T>> assignments;
	ABT_Assignment assignmentData;
};

}

// Implementation of Assignment

template<typename V, typename T>
AIT::Assignment<V, T>::Assignment(V instance_, const T& value_,
		const AgentID& id_, const int32_t& pureValue_) :
		instance(instance_), value(value_) {
	this->assignmentData.set_id(id_);
	this->assignmentData.set_value(pureValue_);
}

template<typename V, typename T>
inline bool AIT::Assignment<V, T>::operator ==(const Assignment& rhs) {
	if (rhs.instance == this->instance && rhs.value == this->value)
		return true;
	return false;
}

template<typename V, typename T>
inline AIT::Assignment<V, T>::Assignment() {
}

template<typename V, typename T>
inline bool AIT::Assignment<V, T>::operator !=(const Assignment& rhs) {
	return !(*this == rhs);
}

template<typename V, typename T>
inline AIT::CompoundAssignment<V, T>::CompoundAssignment() {
}

// Implementation of CompoundAssignment

template<typename V, typename T>
inline void AIT::CompoundAssignment<V, T>::add(const Assignment<V, T>& item) {
	if (std::find(this->assignments.begin(), this->assignments.end(), item)
			!= this->assignments.end()) {
		this->assignments.push_back(item);
	}
}

// end of namespace

#endif /* COMMON_CSP_HPP_ */
