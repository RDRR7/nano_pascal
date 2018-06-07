#ifndef _NANOPASCAL_SYMBOLS_H
#define _NANOPASCAL_SYMBOLS_H

enum class Symbol : unsigned int
{
    /* Non terminals, use values greater than 255 */

    /* Terminals */
    KwProgram = 0,
    KwVar = 1,
    KwArray = 2,
    KwOf = 3,
    KwInteger = 4,
    KwBoolean = 5,
    KwChar = 6,
    KwNot = 7,
    KwAnd = 8,
    KwOr = 9,
    KwXor = 10,
    KwShl = 11,
    KwShr = 12,
    KwDiv = 13,
    KwMod = 14,
    KwBegin = 15,
    KwEnd = 16,
    KwBreak = 17,
    KwIf = 18,
    KwThen = 19,
    KwElse = 20,
    KwWhile = 21,
    KwRepeat = 22,
    KwUntil = 23,
    KwFor = 24,
    KwDo = 25,
    KwWrite = 26,
    KwWriteln = 27,
    KwTo = 28,

    StrLiteral = 29,
    Number = 30,
    Ident = 31,
    OpAdd = 32,
    OpSub = 33,
    OpMul = 34,
    OpDiv = 35,
    OpAssign = 36,
    OpenPar = 37,
    ClosePar = 38,
    Semicolon = 39,
    Comma = 40,
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
