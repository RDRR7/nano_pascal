#ifndef _NANOPASCAL_SYMBOLS_H
#define _NANOPASCAL_SYMBOLS_H

enum class Symbol : unsigned int
{
    /* Non terminals, use values greater than 255 */

    /* Terminals */
    KwProgram = 0,
    KwBegin = 1,
    KwEnd = 2,
    KwVar = 3,
    KwIf = 4,
    KwThen = 5,
    KwElse = 6,
    KwWriteln = 7,
    StrLiteral = 8,
    Number = 10,
    Ident = 11,
    OpAdd = 12,
    OpSub = 13,
    OpMul = 14,
    OpDiv = 15,
    OpAssign = 16,
    OpenPar = 17,
    ClosePar = 18,
    Semicolon = 19,
    Comma = 20,
    Eof = 255,
};

static inline bool is_terminal(Symbol s)
{
    return static_cast<unsigned int>(s) <= 255;
}

static inline bool is_nonterminal(Symbol s)
{
    return static_cast<unsigned int>(s) > 255;
}

#endif
