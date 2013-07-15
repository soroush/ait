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

#ifndef CSP_SOLVER_H_
#define CSP_SOLVER_H_

#include <string>
#include <fstream>
#include <vector>
#include <forward_list>
#include "constraint.h"

namespace AIT {
namespace CSP {

class LIBRARY_API CSP_Solver {
public:
	virtual ~CSP_Solver();
	virtual void parseFromFile(const std::string&)=0;
	virtual void parseFromStream(const std::ifstream&)=0;
	virtual void parseFromContent(const std::string&)=0;
protected:
	// In formal definition of CSP, X, D and C are finite sets.
	// That's the reason to provide integer-only CSP solvers
	std::vector<size_t> variables; // X
	std::vector<std::vector<int>> domains; // D
	std::vector<Constraint> constraints; // C
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CSP_SOLVER_H_ */
