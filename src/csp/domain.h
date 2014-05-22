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

/**
 * This class represents domain of a CSP variable which includes a set of values
 * and a the name of domain.
 */
class LIBRARY_API Domain {
public:
    /**
     * Constructor of @ref Domain class.
     * @param nbValues Number of values (size of domain). Specifying this value
     * increases performance of push back operation for underlying vector. In
     * the cases that size of domain is not know at construction time, set
     * @ref nbValues to 0.
     * @param content This string is XCSP content (contents of domain tag) which
     * should be parsed to extract members of domain set. Format of this
     * parameter is specified in XCSP definitions. The input can be either of
     * following formats:
     *     - \c "1 5 10" \c corresponds to the set \f$\lbrace 1, 5, 10\rbrace\f$;
     *     - \c "1..3 7 10..14" \c corresponds to the set
     *       \f$\lbrace 1, 2, 3, 7, 10, 11, 12, 13, 14\rbrace \f$.
     * @param name The name of domain. This is a unique string which is used to
     * refer to the domain.
     */
	Domain(const size_t& nbValues, const std::string& content, const std::string& name);
	/**
	 * Move constructor
	 */
	Domain(Domain&& d);
	/**
	 * Move assignment operator
	 */
	Domain& operator =(Domain&& d);
    /**
     * Destructor
     */
	~Domain();

	// void reConstruct(); FIXME: Remember why I wrote this?
	/**
	 * Returns name of current domain.
	 */
	const std::string& getName() const;
	/**
	 * This method changes the name of current domain.
	 * @param name New name
	 * @warning This method should be used carefully to avoid name collisions
	 * between domains. Unique naming of domains is required because XCSP 2.1
	 * refers to domains by names in variable definitions.
	 */
	void setName(const std::string& name);
	/**
	 * Returns all values of the domain in a read-only vector.
	 */
	const std::vector<int>& getValues() const;
	/**
	 * Replaces all values of the domain with new values of given vector.
	 * @param values Vector of new values
	 */
	void setValues(const std::vector<int>& values);

	static Domain empty;
private:
	std::vector<int> m_values;
	std::string m_name;
};

} /* namespace CSP */
} /* namespace AIT */

#endif /* DOMAIN_H_ */
