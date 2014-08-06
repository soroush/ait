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

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include "relation-base.hpp"
#include "parser/expression.hpp"
#include <memory>

namespace AIT {
namespace CSP {

class CSP_Problem;

class LIBRARY_API Predicate: public RelationBase {
    friend class FunctionalParser;
public:
    enum class Type {
        Functional, Infix, Postfix, MathML
    };
    struct Reference {
        int value;
        std::string name;
    };
    Predicate(const std::string& name, const std::string& parameters,
            const std::string& input, const Type&);
    Predicate(Predicate&&);
    Predicate& operator =(Predicate&&);
    ~Predicate();
    bool evaluate(const std::vector<int>&);
    bool evaluate(std::vector<int>&&);
    bool evaluate(const std::vector<int*>&);
    void addParameter(const std::string&name, const int& value = 0);
    void addPostfixExpression(Expression*);
    const std::vector<Reference>& getParameters() const;
    int parameter(const std::string& name) const;
    const int* getParameterPointer(const std::string& name) const;
private:
    /**
     * Parameters of a predicate. This vector holds actual values of parameters
     * processed by the predicate
     */
    std::vector<Reference> parameters;

    /**
     * This vector holds pointers to expressions held by current predicate. A
     * predicate can hold a variety of representations, one for each type.
     * In order to get a simple computational approach over all types of
     * representations, all types of expression will be converted to postfix
     * notation.
     * Currently only functional representation is supported. If there is more
     * than one representation, then result of converting all representations to
     * postfix should be same.
     */
    std::vector<std::unique_ptr<Expression>> postfix;
    /**
     * Evaluation stack used by expressions. After finishing evaluation
     * operation, this stack will have one value, either 0 or 1 which represents
     * if predicate is satisfied by given values or not. If there exists more
     * than one value in stack, then there is something wrong with evaluation.
     * Either parser did not do the job correctly or evaluation is not done
     * properly by the @ref Expression (Specifically the @ref exp_func class and
     * its derivatives.)
     */
    std::stack<int> evaluation;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* PREDICATE_H_ */
