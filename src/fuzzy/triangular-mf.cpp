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

#include "triangular-mf.hpp"
#include "triangular-imf.hpp"

using namespace AIT::FUZZY;
using namespace std;

TriangularMF::TriangularMF(const string& name, const float& start,
        const float& peak, const float& end) :
        MembershipFunction(name), m_start(start), m_peak(peak), m_end(end) {
    // TODO: Implement sanity check
    this->m_rising = +1.0F / (this->m_peak - this->m_start);
    this->m_falling = 1.0F / (this->m_peak - this->m_end);
}

TriangularMF::~TriangularMF() {
}

float TriangularMF::operator ()(const float& input) const {
    if (input < this->m_start) {
        return 0.0;
    } else if (input >= this->m_start && input < this->m_peak) {
        return this->m_rising * (input - this->m_start);
    } else if (input >= this->m_peak && input < this->m_end) {
        return this->m_falling * (input - this->m_end);
    } else {
        return 0.0;
    }
}

float TriangularMF::startPoint() const {
    return this->m_start;
}

float TriangularMF::peakPoint() const {
    return this->m_peak;
}

float TriangularMF::endPoint() const {
    return this->m_end;
}

float TriangularMF::risingSlope() const {
    return this->m_rising;
}

float TriangularMF::fallingSlope() const {
    return this->m_falling;
}

const ImplicatedMembershipFunction* TriangularMF::implicate(const float& value) const {
    // TODO: Add smart pointers and memory ownership implementation
    return new TriangularIMF(this,value);
}
