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

#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <vector>
#include <string>
#include "../global.h"

namespace AIT {
namespace CSP {

class CSP_Problem;

class LIBRARY_API Domain {
public:
	Domain(const size_t& nbValues, const std::string& content, const std::string& name);
	Domain(Domain&& );
	Domain& operator =(Domain&&);
	~Domain();
	void reConstruct();
	const std::string& getName() const;
	void setName(const std::string& name);
	const std::vector<int>& getValues() const;
	void setValues(const std::vector<int>& values);

	static Domain empty;
private:
	std::vector<int> m_values;
	std::string m_name;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* DOMAIN_H_ */
