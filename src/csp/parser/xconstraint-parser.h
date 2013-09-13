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

#ifndef XCONSTRAINT_PARSER_H_
#define XCONSTRAINT_PARSER_H_

#include "xcsp-pskel.hxx"

namespace AIT {
namespace CSP {

class CSP_Problem;

class XConstraintParser: public constraint_t_pskel {
public:
	XConstraintParser(CSP_Problem&);
	~XConstraintParser();
	void parameters(const std::string&);
	void name(const std::string&);
	void scope(const std::string&);
	void reference(const std::string&);
	//FIXME: Control behavior of XSD to generate more relax data types. Absolutely NOT long long!
	void arity(unsigned long long);
	Constraint post_constraint_t();
private:
	std::string m_parameters;
	std::string m_name;
	std::string m_scope;
	std::string m_reference;
	unsigned long long m_arity;
	CSP_Problem& m_instance;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* XCONSTRAINT_PARSER_H_ */
