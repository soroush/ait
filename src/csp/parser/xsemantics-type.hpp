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

#ifndef XSEMANTICS_TYPE_H_
#define XSEMANTICS_TYPE_H_

#include <xsd/cxx/parser/xml-schema.hxx>
#include "xcsp-pskel.hxx"

// #define BOOST_ALL_DYN_LINK
// #include <boost/log/trivial.hpp>
// #include <boost/log/sources/severity_logger.hpp>

namespace AIT {
namespace CSP {

class RelationBase;

class XSemanticsType: public semanticsType_pskel,
		public virtual xml_schema::string_pimpl {
public:
	XSemanticsType();
	~XSemanticsType();
	virtual RelationBase::Semantics post_semanticsType();
private:
    // boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* XSEMANTICS_TYPE_H_ */
