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
#include <map>
#include "csp-problem.h"

namespace AIT {
namespace CSP {

class LIBRARY_API CSP_Solver {
public:
	CSP_Solver();
	virtual ~CSP_Solver();
	virtual void parseFromFile(const std::string&);
//	virtual void parseFromStream(const std::ifstream&);
//	virtual void parseFromContent(const std::string&);

protected:
	CSP_Problem instance;

};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CSP_SOLVER_H_ */
