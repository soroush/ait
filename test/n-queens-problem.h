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

#include <string>

const std::string _4_QUEENS_XCSP = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><"
        "instance><presentation maxConstraintArity=\"2\" format=\"XCSP 2.1\" ty"
        "pe=\"CSP\"/><domains nbDomains=\"1\"><domain name=\"D0\"  nbValues=\"4"
        "\">1..4</domain></domains><variables nbVariables=\"12\"><variable name"
        "=\"V0\" domain=\"D0\"/><variable name=\"V1\" domain=\"D0\"/><variable "
        "name=\"V2\" domain=\"D0\"/><variable name=\"V3\" domain=\"D0\"/></vari"
        "ables><predicates nbPredicates=\"1\"><predicate name=\"P0\"><parameter"
        "s>int X0 int X1 int X2</parameters><expression><functional>and(ne(X0,X"
        "1),ne(abs(sub(X0,X1)),X2))</ functional></expression></predicate></pre"
        "dicates><constraints nbConstraints=\"66\"><constraint name=\"C0\" arit"
        "y=\"2\" scope=\"V0 V1\" reference=\"P0\"><parameters>V0 V1 1</paramete"
        "rs></constraint><constraint name=\"C1\" arity=\"2\" scope=\"V0 V2\" re"
        "ference=\"P0\"><parameters>V0 V2 2</parameters></constraint><constrain"
        "t name=\"C2\" arity=\"2\" scope=\"V0 V3\" reference=\"P0\"><parameters"
        ">V0 V3 3</parameters></constraint><constraint name=\"C3\" arity=\"2\" "
        "scope=\"V1 V3\" reference=\"P0\"><parameters>V1 V2 1</parameters></con"
        "straint><constraint name=\"C4\" arity=\"2\" scope=\"V1 V3\" reference="
        "\"P0\"><parameters>V1 V3 2</parameters></constraint><constraint name= "
        "\"C5\" arity=\"2\" scope=\"V2 V3\" reference=\"P0\"><parameters>V2 V3 "
        "1</parameters></constraint></constraints></instance>";
