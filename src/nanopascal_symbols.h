#ifndef _NANOPASCAL_SYMBOLS_H
#define _NANOPASCAL_SYMBOLS_H

enum class Symbol : unsigned int
{
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

    OpenBra = 29,
    CloseBra = 30,
    Comma = 31,
    Semicolon = 32,
    OpenPar = 33,
    ClosePar = 34,
    OpEqual = 35,
    OpSub = 36,
    OpAdd = 37,
    OpMul = 38,
    OpLessThan = 39,
    OpGreaterThan = 40,
    OpNotEqual = 41,
    OpLessThanOrEqual = 42,
    OpGreaterThanOrEqual = 43,
    Assign = 44,
    Colon = 45,
    OpLeftShift = 46,
    OpRightShift = 47,
    StringConstant = 48,
    ID = 49,

    Number = 50,
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
