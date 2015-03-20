/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
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

#ifndef TRIANGULAR_MF_HPP_
#define TRIANGULAR_MF_HPP_

#include "membership-function.hpp"

namespace AIT {
namespace FUZZY {

class TriangularMF: public MembershipFunction {
public:
    TriangularMF(const std::string& name, const float& start, const float& peak,
            const float& end);
    ~TriangularMF();
    float operator ()(const float& input) const;
    float startPoint() const;
    float peakPoint() const;
    float endPoint() const;
    float risingSlope() const;
    float fallingSlope() const;
    virtual const ImplicatedMembershipFunction* implicate(
            const float &value) const;
private:
    float m_start;
    float m_peak;
    float m_end;
    float m_rising;
    float m_falling;
};

} /* namespace FUZZY */
} /* namespace AIT */

#endif /* TRIANGULAR_MF_HPP_ */
