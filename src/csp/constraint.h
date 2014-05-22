/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <vector>
#include <string>
#include "relation-base.h"
#include "variable.h"
#include "../global.h"

namespace AIT {
namespace CSP {

class CSP_Problem;

class LIBRARY_API Constraint {
public:
	Constraint(const std::string& name, const size_t& arity,
			const std::string& scope, const std::string& reference,
			const std::string& parameters,
			CSP_Problem* instance);
	Constraint(Constraint&&);
	Constraint& operator =(Constraint&&);
	~Constraint();
	bool satisfies();

	const std::vector<Variable*>& Scope() const;
private:
	/**
	 * List of all pointers to all variables of current constraint. This
	 * variable is useless in current computation and it's computed and held
	 * for future usages or external algorithms.
	 */
	std::vector<Variable*> scope;
	/**
	 * A vector of pointers to values of all variables and numerals that should
	 * be passed
	 * through predicate. If @ref reference is a relation (current constraint is
	 * an extension) then this vector should be identical to @ref scope in terms
	 * of logic, though for computational reasons we keep it empty in such case.
	 */
	std::vector<int*> parameters;
	/**
	 * Pointer to a @ref Relation or @ref Predicate.
	 */
	RelationBase* reference;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CONSTRAINT_H_ */
