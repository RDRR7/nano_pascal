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
    KwEndDot = 54,
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
    KwFunction = 56,
    KwProcedure = 57,
    KwRead = 58,
    KwTrue = 59,
    KwFalse = 60,
    KwContinue = 61,

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
    IntConstantDec = 50,
    IntConstantHex = 51,
    IntConstantBin = 52,
    CharConstant = 53,
    DotDot = 55,

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

static const char *get_symbol_name(Symbol symbol)
{
    switch (symbol)
    {
    case Symbol::Eof:
        return "End of Input";
    case Symbol::KwProgram:
        return "Program";
    case Symbol::KwVar:
        return "Var";
    case Symbol::KwArray:
        return "Array";
    case Symbol::KwOf:
        return "Of";
    case Symbol::KwInteger:
        return "Integer";
    case Symbol::KwBoolean:
        return "Boolean";
    case Symbol::KwChar:
        return "Char";
    case Symbol::KwNot:
        return "Not";
    case Symbol::KwAnd:
        return "And";
    case Symbol::KwOr:
        return "Or";
    case Symbol::KwXor:
        return "Xor";
    case Symbol::KwShl:
        return "Shl";
    case Symbol::KwShr:
        return "Shr";
    case Symbol::KwDiv:
        return "Div";
    case Symbol::KwMod:
        return "Mod";
    case Symbol::KwBegin:
        return "Begin";
    case Symbol::KwEnd:
        return "End";
    case Symbol::KwEndDot:
        return "EndDot";
    case Symbol::KwBreak:
        return "Break";
    case Symbol::KwIf:
        return "If";
    case Symbol::KwThen:
        return "Then";
    case Symbol::KwElse:
        return "Else";
    case Symbol::KwWhile:
        return "While";
    case Symbol::KwRepeat:
        return "Repeat";
    case Symbol::KwUntil:
        return "Until";
    case Symbol::KwFor:
        return "For";
    case Symbol::KwDo:
        return "Do";
    case Symbol::KwWrite:
        return "Write";
    case Symbol::KwWriteln:
        return "Writeln";
    case Symbol::KwTo:
        return "To";
    case Symbol::KwFunction:
        return "Function";
    case Symbol::KwProcedure:
        return "Procedure";
    case Symbol::KwRead:
        return "Read";
    case Symbol::KwTrue:
        return "True";
    case Symbol::KwFalse:
        return "False";
    case Symbol::KwContinue:
        return "Continue";
    case Symbol::OpenBra:
        return "OpenBra";
    case Symbol::CloseBra:
        return "CloseBra";
    case Symbol::Comma:
        return "Comma";
    case Symbol::Semicolon:
        return "Semicolon";
    case Symbol::OpenPar:
        return "OpenPar";
    case Symbol::ClosePar:
        return "ClosePar";
    case Symbol::OpEqual:
        return "OpEqual";
    case Symbol::OpSub:
        return "OpSub";
    case Symbol::OpAdd:
        return "OpAdd";
    case Symbol::OpMul:
        return "OpMul";
    case Symbol::OpLessThan:
        return "OpLessThan";
    case Symbol::OpGreaterThan:
        return "OpGreaterThan";
    case Symbol::OpNotEqual:
        return "OpNotEqual";
    case Symbol::OpLessThanOrEqual:
        return "OpLessThanOrEqual";
    case Symbol::OpGreaterThanOrEqual:
        return "OpGreaterThanOrEqual";
    case Symbol::Assign:
        return "Assign";
    case Symbol::Colon:
        return "Colon";
    case Symbol::OpLeftShift:
        return "OpLeftShift";
    case Symbol::OpRightShift:
        return "OpRightShift";
    case Symbol::StringConstant:
        return "StringConstant";
    case Symbol::ID:
        return "ID";
    case Symbol::IntConstantDec:
        return "IntConstantDec";
    case Symbol::IntConstantHex:
        return "IntConstantHex";
    case Symbol::IntConstantBin:
        return "IntConstantBin";
    case Symbol::CharConstant:
        return "CharConstant";
    case Symbol::DotDot:
        return "DotDot";
    }
    return "Unknown";
}

#endif
