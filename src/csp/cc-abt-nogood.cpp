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

#include "cc-abt-solver.hpp"

using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::ccabt;

CC_ABT_Solver::Nogood::Nogood() {
}

CC_ABT_Solver::Nogood::Nogood(const Nogood& other) :
		lhs(other.lhs), rhs(other.rhs) {
}

CC_ABT_Solver::Nogood::~Nogood() {
}

bool CC_ABT_Solver::Nogood::operator ==(const Nogood& other) const {
	return (this->lhs == other.lhs and this->rhs == other.rhs);
}

bool CC_ABT_Solver::Nogood::operator !=(const Nogood& other) const {
	return (this->lhs != other.lhs or this->lhs == other.lhs);
}

CC_ABT_Solver::Nogood& CC_ABT_Solver::Nogood::operator =(const Nogood& other) {
	this->lhs = other.lhs;
	this->rhs = other.rhs;
	return *this;
}

CC_ABT_Solver::Nogood::Nogood(const CompoundAssignment& lhs_,
		const Assignment& rhs_) :
		lhs(lhs_), rhs(rhs_) {
}

CC_ABT_Solver::Nogood::operator P_Nogood() {
	P_Nogood pngd;
	pngd.mutable_lhs()->CopyFrom(this->lhs);
	pngd.mutable_rhs()->CopyFrom(this->rhs);
	return pngd;
}
