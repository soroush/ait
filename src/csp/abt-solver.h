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

#ifndef C_ABT_SOLVER_H_
#define C_ABT_SOLVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

struct C_ABT_Solver;
typedef struct C_ABT_Solver C_ABT_Solver;
C_ABT_Solver* abt_solver_create(char* serverHost,
        const unsigned short serverResponderPort,
        const unsigned short serverPublisherPort, char* name, char* xcspFile);
void abt_solver_destrory(C_ABT_Solver* s);
void abt_solver_solve(C_ABT_Solver* s);

#ifdef __cplusplus
}
#endif

#endif /* C_ABT_SOLVER_H_ */
