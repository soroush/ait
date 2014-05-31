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

#ifndef CSP_PROBLEM_H_
#define CSP_PROBLEM_H_

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include "domain.h"
#include "variable.h"
#include "relation-base.h"
#include "predicate.h"
#include "constraint.h"

namespace AIT {
namespace CSP {

class LIBRARY_API CSP_Problem {
    friend class XDomainsParser;
    friend class XVariablesParser;
public:
    enum class Type {
        CSP, QCSP, WCSP, Unknown
    };
    enum class Format {
        XCSP_21
    };
    enum class NumberType {
        AtLeast, Exactly, Unknown
    };
    CSP_Problem(){};
    virtual ~CSP_Problem();

    void addDomain(Domain&& d);
    void addVariable(Variable&& v);
    void addConstraint(Constraint&& c);
    void addPredicate(Predicate&& p);

    Variable* variable(const std::string&) const;
    Variable* variable(const size_t&) const;
    size_t variableIndex(const std::string&) const;
    Domain* domain(const std::string&) const;
    RelationBase* relation(const std::string&) const;

    // Interface:
    void setName(const std::string& name);
    void setMaxConstraintArity(const unsigned int& max);
    void setMinViolatedConstraints(std::pair<NumberType, unsigned int> number);
    void setNbSolutions(std::pair<NumberType, unsigned int> number);
    void setSolution(const std::string& solution);
    void setType(const Type& type);
    void setFormat(const Format& format);
    const std::vector<Domain*>& domains() const;
    const std::vector<Variable*>& variables() const;
    const std::vector<RelationBase*>& relationBases() const;
    const std::vector<Constraint*>& constraints() const;

private:
    std::string m_name;
    unsigned int m_maxConstraintArity;
    std::pair<NumberType, unsigned int> m_minViolatedConstraints;
    std::pair<NumberType, unsigned int> m_nbSolutions;
    std::string m_solution;
    Type m_type;
    Format m_format;

protected:
    std::vector<Domain*> m_domains;
    std::vector<Variable*> m_variables;
    std::vector<Constraint*> m_constraints;
    std::vector<RelationBase*> m_relations;

    std::map<std::string, Variable*> variableNames;
    std::map<std::string, Domain*> domainNames;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CSP_PROBLEM_H_ */
