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

#include "triangular-imf.hpp"
#include "triangular-mf.hpp"
using namespace AIT::FUZZY;
using namespace std;

TriangularIMF::TriangularIMF(const float& start, const float& peak1,
        const float& peak2, const float& end) :
        m_start(start), m_peak1(peak1), m_peak2(peak2), m_end(end) {
    // TODO: Implement sanity check
    this->m_rising = +1.0F / (this->m_peak1 - this->m_start);
    this->m_falling = 1.0F / (this->m_peak2 - this->m_end);
    float maximum = (1.0F / this->m_rising) + this->m_start;
    this->m_cutoff = (this->m_peak1 - this->m_start)
            / (maximum - this->m_start);
}

TriangularIMF::TriangularIMF(const TriangularMF* const mf, const float& cutoff) :
        m_start { mf->startPoint() },
        m_end { mf->endPoint() },
        m_rising { mf->risingSlope() },
        m_falling { mf->fallingSlope() },
        m_cutoff { cutoff } {
            this->m_peak1 = (this->m_cutoff/this->m_rising)+this->m_start;
            this->m_peak2 = (this->m_cutoff/this->m_falling)+this->m_end;
}

TriangularIMF::~TriangularIMF() {
}

float TriangularIMF::operator ()(const float& input) const {
    if (input < this->m_start) {
        return 0.0;
    } else if (input >= this->m_start && input < this->m_peak1) {
        return this->m_rising * (input - this->m_start);
    } else if (input >= this->m_peak1 && input < this->m_peak2) {
        return this->m_cutoff;
    } else if (input >= this->m_peak2 && input < this->m_end) {
        return this->m_falling * (input - this->m_end);
    } else {
        return 0.0;
    }
}
