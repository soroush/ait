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

#ifndef TRIANGULAR_IMF_HPP_
#define TRIANGULAR_IMF_HPP_

#include "implicated-mf.hpp"

namespace AIT {
namespace FUZZY {

class TriangularMF;

class TriangularIMF: public ImplicatedMembershipFunction {
public:
    TriangularIMF(const float& start, const float& peak1, const float& peak2,
            const float& end);
    TriangularIMF(const TriangularMF* const mf, const float& cutoff);
    virtual ~TriangularIMF();
    float operator()(const float& input) const;
private:
    float m_start;
    float m_peak1;
    float m_peak2;
    float m_end;
    float m_rising;
    float m_falling;
    float m_cutoff;
};

} /* namespace FUZZY */
} /* namespace AIT */

#endif /* TRIANGULAR_IMF_HPP_ */
