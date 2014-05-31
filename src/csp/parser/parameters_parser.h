// Generated by Bisonc++ V2.09.03 on Tue, 16 Jul 2013 11:37:42 +0430

#ifndef ParametersParser_h_included
#define ParametersParser_h_included

#include "parameters_parserbase.h"
#include "parameters_scanner.h"
#include "../csp-problem.h"
#include <sstream>
#include <vector>
#include <map>

#undef ParametersParser
class ParametersParser: public ParametersParserBase {

public:
    ParametersParser(const std::string& input, const std::string& predicate,
            const AIT::CSP::CSP_Problem& instance);
    int parse();

private:
    std::istringstream str;
    ParametersLexer d_scanner;
    const std::string predicate;
    const AIT::CSP::CSP_Problem& instance;

    void error(char const *msg);
    int lex();
    void print();
    void executeAction(int ruleNr);
    void errorRecovery();
    int lookup(bool recovery);
    void nextToken();
};

inline ParametersParser::ParametersParser(const std::string& input,
        const std::string& predicate_, const AIT::CSP::CSP_Problem& instance_) :
        str(input), d_scanner(str, std::cout), predicate(predicate_), instance(
                instance_) {
}

inline void ParametersParser::error(char const *msg) {
    std::cerr << msg << '\n';
}

inline int ParametersParser::lex() {
    return d_scanner.lex();
}

inline void ParametersParser::print() {
}

#endif
