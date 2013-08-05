// Generated by Bisonc++ V2.09.03 on Mon, 05 Aug 2013 03:11:54 +0430

#ifndef ConstraintParametersParserBase_h_included
#define ConstraintParametersParserBase_h_included

#include <vector>
#include <iostream>


namespace // anonymous
{
    struct PI__;
}


class ConstraintParametersParserBase
{
    public:
// $insert tokens

    // Symbolic tokens:
    enum Tokens__
    {
        INT = 257,
        VAR,
    };

// $insert STYPE
union STYPE__
{
 int i;
 std::string* s;
};


    private:
        int d_stackIdx__;
        std::vector<size_t>   d_stateStack__;
        std::vector<STYPE__>  d_valueStack__;

    protected:
        enum Return__
        {
            PARSE_ACCEPT__ = 0,   // values used as parse()'s return values
            PARSE_ABORT__  = 1
        };
        enum ErrorRecovery__
        {
            DEFAULT_RECOVERY_MODE__,
            UNEXPECTED_TOKEN__,
        };
        bool        d_debug__;
        size_t      d_nErrors__;
        size_t      d_requiredTokens__;
        size_t      d_acceptedTokens__;
        int         d_token__;
        int         d_nextToken__;
        size_t      d_state__;
        STYPE__    *d_vsp__;
        STYPE__     d_val__;
        STYPE__     d_nextVal__;

        ConstraintParametersParserBase();

        void ABORT() const;
        void ACCEPT() const;
        void ERROR() const;
        void clearin();
        bool debug() const;
        void pop__(size_t count = 1);
        void push__(size_t nextState);
        void popToken__();
        void pushToken__(int token);
        void reduce__(PI__ const &productionInfo);
        void errorVerbose__();
        size_t top__() const;

    public:
        void setDebug(bool mode);
}; 

inline bool ConstraintParametersParserBase::debug() const
{
    return d_debug__;
}

inline void ConstraintParametersParserBase::setDebug(bool mode)
{
    d_debug__ = mode;
}

inline void ConstraintParametersParserBase::ABORT() const
{
    throw PARSE_ABORT__;
}

inline void ConstraintParametersParserBase::ACCEPT() const
{
    throw PARSE_ACCEPT__;
}

inline void ConstraintParametersParserBase::ERROR() const
{
    throw UNEXPECTED_TOKEN__;
}


// As a convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define ConstraintParametersParser ConstraintParametersParserBase


#endif


