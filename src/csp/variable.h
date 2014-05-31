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

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "../global.h"
#include "domain.h"
#include <map>

namespace AIT {
namespace CSP {
/**
 * This class represents a variable in a CSP problem. Each variable can take
 * on the values in its nonempty domain @ref m_domain.
 */
class LIBRARY_API Variable {
public:
    Variable(Domain* domain, const std::string& name = "",
            const int& value = 0);
    Variable(Variable&& v);
    Variable& operator =(Variable&& v);
    ~Variable();

    int* value();
    const Domain* getDomain() const;
    void setDomain(Domain* domain);
    const std::string& getName() const;
    void setName(const std::string& name);
    int getValue() const;
    void setValue(int value);
    bool isSet() const;
    void unset();

private:
    Domain* m_domain;
    std::string m_name;
    int m_value;
    bool m_isSet;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* VARIABLE_H_ */
