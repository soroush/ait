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
#include <memory>
#include "domain.h"
#include "variable.h"
#include "relation-base.h"
#include "predicate.h"
#include "constraint.h"

namespace AIT {
namespace CSP {

class LIBRARY_API CSP_Problem {
    friend class XInstanceParser;
public:
    class Presentation {
    public:
        enum class Type {
            CSP, QCSP, WCSP, Unknown
        };
        enum class Format {
            XCSP_21, Unknown
        };
        enum class NumberType {
            AtLeast, Exactly, Unknown
        };
        Presentation(const std::string& name, const unsigned int& maxConstraint,
                const std::pair<NumberType, unsigned int>& minViolatedConstraints,
                const std::pair<NumberType, unsigned int>& nbSolutions,
                const Type& type,
                const Format& format);
        ~Presentation();
        void setName(const std::string& name);
        void setMaxConstraintArity(const unsigned int& max);
        void setMinViolatedConstraints(
                std::pair<NumberType, unsigned int> number);
        void setNbSolutions(std::pair<NumberType, unsigned int> number);
        void setSolution(const std::string& solution);
        void setType(const Type& type);
        void setFormat(const Format& format);
    private:
        std::string m_name;
        unsigned int m_maxConstraintArity;
        std::pair<NumberType, unsigned int> m_minViolatedConstraints;
        std::pair<NumberType, unsigned int> m_nbSolutions;
        std::string m_solution;
        Type m_type;
        Format m_format;
    };

    CSP_Problem() {
    }
    ;
    virtual ~CSP_Problem();

    void addDomain(std::unique_ptr<Domain> d);
    void addVariable(std::unique_ptr<Variable> v);
    void addConstraint(std::unique_ptr<Constraint> c);
    void addPredicate(std::unique_ptr<Predicate> p);

    Variable* variable(const std::string&) const;
    Variable* variable(const size_t&) const;
    size_t variableIndex(const std::string&) const;
    Domain* domain(const std::string&) const;
    RelationBase* relation(const std::string&) const;

    // Interface:
    const std::vector<std::unique_ptr<Domain>>& domains() const;
    const std::vector<std::unique_ptr<Variable>>& variables() const;
    const std::vector<std::unique_ptr<RelationBase>>& relationBases() const;
    const std::vector<std::unique_ptr<Constraint>>& constraints() const;

protected:
    std::unique_ptr<Presentation> m_presentation;
    std::vector<std::unique_ptr<Domain>> m_domains;
    std::vector<std::unique_ptr<Variable>> m_variables;
    std::vector<std::unique_ptr<Constraint>> m_constraints;
    std::vector<std::unique_ptr<RelationBase>> m_relations;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CSP_PROBLEM_H_ */
