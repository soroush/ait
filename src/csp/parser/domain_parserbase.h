// Generated by Bisonc++ V4.05.00 on Tue, 17 Jun 2014 23:26:16 +0430

#ifndef DomainParserBase_h_included
#define DomainParserBase_h_included

#include <exception>
#include <vector>
#include <iostream>

// $insert preincludes
#include <vector>

namespace // anonymous
{
    struct PI__;
}



class DomainParserBase
{
    public:
// $insert tokens

    // Symbolic tokens:
    enum Tokens__
    {
        INTEGER = 257,
        RANGE_SEP,
    };

// $insert STYPE
union STYPE__
{
int i;
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

        DomainParserBase();

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

inline bool DomainParserBase::debug() const
{
    return d_debug__;
}

inline void DomainParserBase::setDebug(bool mode)
{
    d_debug__ = mode;
}

inline void DomainParserBase::ABORT() const
{
    throw PARSE_ABORT__;
}

inline void DomainParserBase::ACCEPT() const
{
    throw PARSE_ACCEPT__;
}

inline void DomainParserBase::ERROR() const
{
    throw UNEXPECTED_TOKEN__;
}


// As a convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define DomainParser DomainParserBase


#endif


