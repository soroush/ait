// Generated by Flexc++ V0.98.00 on Tue, 16 Jul 2013 14:02:07 +0430

#ifndef ParametersLexer_H_INCLUDED_
#define ParametersLexer_H_INCLUDED_

// $insert baseclass_h
#include "parameters_scannerbase.h"

// $insert classHead
class ParametersLexer: public ParametersLexerBase
{
    public:
        explicit ParametersLexer(std::istream &in = std::cin,
                                std::ostream &out = std::cout);

        ParametersLexer(std::string const &infile, std::string const &outfile);
        
        // $insert lexFunctionDecl
        int lex();

    private:
        int lex__();
        int executeAction__(size_t ruleNr);

        void print();
        void preCode();     // re-implement this function for code that must 
                            // be exec'ed before the patternmatching starts
};

// $insert scannerConstructors
inline ParametersLexer::ParametersLexer(std::istream &in, std::ostream &out)
:
    ParametersLexerBase(in, out)
{}

inline ParametersLexer::ParametersLexer(std::string const &infile, std::string const &outfile)
:
    ParametersLexerBase(infile, outfile)
{}

// $insert inlineLexFunction
inline int ParametersLexer::lex()
{
    return lex__();
}

inline void ParametersLexer::preCode() 
{
    // optionally replace by your own code
}

inline void ParametersLexer::print() 
{
    print__();
}


#endif // ParametersLexer_H_INCLUDED_

