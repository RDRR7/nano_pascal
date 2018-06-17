#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <functional>
#include "nanopascal_lexer.h"

#define RETURN_TOKEN(tk)                     \
	do                                       \
	{                                        \
		this->lexeme = this->current_symbol; \
		get_next_symbol();                   \
		return tk;                           \
	} while (0)

static const char *kw[] = {
	"program",
	"var",
	"array",
	"of",
	"integer",
	"boolean",
	"char",
	"not",
	"and",
	"or",
	"xor",
	"shl",
	"shr",
	"div",
	"mod",
	"begin",
	"end",
	"break",
	"if",
	"then",
	"else",
	"while",
	"repeat",
	"until",
	"for",
	"do",
	"write",
	"writeln",
	"to",
	"function",
	"procedure",
	"read",
	"true",
	"false",
	"continue",
	0,
};

static Symbol kwTk[] = {
	Symbol::KwProgram,
	Symbol::KwVar,
	Symbol::KwArray,
	Symbol::KwOf,
	Symbol::KwInteger,
	Symbol::KwBoolean,
	Symbol::KwChar,
	Symbol::KwNot,
	Symbol::KwAnd,
	Symbol::KwOr,
	Symbol::KwXor,
	Symbol::KwShl,
	Symbol::KwShr,
	Symbol::KwDiv,
	Symbol::KwMod,
	Symbol::KwBegin,
	Symbol::KwEnd,
	Symbol::KwBreak,
	Symbol::KwIf,
	Symbol::KwThen,
	Symbol::KwElse,
	Symbol::KwWhile,
	Symbol::KwRepeat,
	Symbol::KwUntil,
	Symbol::KwFor,
	Symbol::KwDo,
	Symbol::KwWrite,
	Symbol::KwWriteln,
	Symbol::KwTo,
	Symbol::KwFunction,
	Symbol::KwProcedure,
	Symbol::KwRead,
	Symbol::KwTrue,
	Symbol::KwFalse,
	Symbol::KwContinue,
};

void NanoPascalLexer::append_sequence(std::function<bool(char)> func)
{
	while (func(this->current_symbol))
	{
		this->lexeme += this->current_symbol;
		get_next_symbol();
	}
}

Symbol NanoPascalLexer::look_up_keyword()
{
	for (int i = 0; kw[i] != 0; i++)
	{
		if (strcasecmp(kw[i], this->lexeme.c_str()) == 0)
		{
			return kwTk[i];
		}
	}
	return Symbol::ID;
}

bool NanoPascalLexer::is_directive_defined(std::string directive)
{
	for (std::string defined_directive : this->directives)
	{
		if (strcasecmp(defined_directive.c_str(), directive.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}

void NanoPascalLexer::print_error(std::string error)
{
	std::cerr << "lexer:" << this->line_number << ":" << this->column_number << ": " << error << std::endl;
}

Symbol NanoPascalLexer::get_next_token()
{
	this->lexeme = "";
	while (1)
	{
		if (this->current_symbol == ' ' ||
			this->current_symbol == '\t' ||
			this->current_symbol == '\n')
		{
			get_next_symbol();
			continue;
		}

		switch (this->current_symbol)
		{
		case '[':
			RETURN_TOKEN(Symbol::OpenBra);
		case ']':
			RETURN_TOKEN(Symbol::CloseBra);
		case ',':
			RETURN_TOKEN(Symbol::Comma);
		case ';':
			RETURN_TOKEN(Symbol::Semicolon);
		case '(':
			get_next_symbol();
			if (this->current_symbol == '*')
			{
				get_next_symbol();
				do
				{
					if (this->current_symbol == '*')
					{
						get_next_symbol();
						if (this->current_symbol == ')')
						{
							break;
						}
					}
					else
					{
						get_next_symbol();
					}
				} while (this->current_symbol != EOF);
				if (this->current_symbol == EOF)
				{
					print_error("Unclosed comment");
					exit(1);
				}
				get_next_symbol();
				break;
			}
			else
			{
				this->lexeme = '(';
				return Symbol::OpenPar;
			}
		case ')':
			RETURN_TOKEN(Symbol::ClosePar);
		case '=':
			RETURN_TOKEN(Symbol::OpEqual);
		case '-':
			RETURN_TOKEN(Symbol::OpSub);
		case '+':
			RETURN_TOKEN(Symbol::OpAdd);
		case '*':
			RETURN_TOKEN(Symbol::OpMul);
		case '<':
			get_next_symbol();
			if (this->current_symbol == '>')
			{
				this->lexeme = "<>";
				get_next_symbol();
				return Symbol::OpNotEqual;
			}
			else if (this->current_symbol == '=')
			{
				this->lexeme = "<=";
				get_next_symbol();
				return Symbol::OpLessThanOrEqual;
			}
			else if (this->current_symbol == '<')
			{
				this->lexeme = "<<";
				get_next_symbol();
				return Symbol::OpLeftShift;
			}
			this->lexeme = '<';
			return Symbol::OpLessThan;
		case '>':
			get_next_symbol();
			if (this->current_symbol == '=')
			{
				this->lexeme = ">=";
				get_next_symbol();
				return Symbol::OpGreaterThanOrEqual;
			}
			else if (this->current_symbol == '>')
			{
				this->lexeme = ">>";
				get_next_symbol();
				return Symbol::OpRightShift;
			}
			this->lexeme = '>';
			return Symbol::OpGreaterThan;
		case ':':
			get_next_symbol();
			if (this->current_symbol == '=')
			{
				this->lexeme = ":=";
				get_next_symbol();
				return Symbol::Assign;
			}
			this->lexeme = ':';
			return Symbol::Colon;
		case '\'':
			get_next_symbol();
			append_sequence([this](char ch) {
				if(ch=='\"'){
					this->current_symbol = '\'';
				}
				return ch != '\'' &&
					   ch != '\n' &&
					   ch != EOF; });
			if (this->current_symbol == '\'')
			{
				get_next_symbol();
				if (this->lexeme.length() == 0)
				{
					print_error("Invalid empty string");
					exit(1);
				}
				else if (this->lexeme.length() == 1)
				{
					return Symbol::CharConstant;
				}
				return Symbol::StringConstant;
			}
			print_error("Missing closing \' in string constant");
			exit(1);
		case '/':
			get_next_symbol();
			if (this->current_symbol == '/')
			{
				do
				{
					get_next_symbol();
				} while (this->current_symbol != '\n');
				break;
			}
			print_error("Invalid symbol /");
			exit(1);
		case '{':
			get_next_symbol();
			do
			{
				this->lexeme += this->current_symbol;
				if (strcasecmp("$ifdef ", this->lexeme.c_str()) == 0 ||
					strcasecmp("$ifndef ", this->lexeme.c_str()) == 0)
				{
					bool is_ifdef = false;
					if (strcasecmp("$ifdef ", this->lexeme.c_str()) == 0)
					{
						is_ifdef = true;
					}
					get_next_symbol();
					this->lexeme = "";
					append_sequence([](char ch) { return isalnum(ch) ||
														 (ch == '_'); });

					if ((is_directive_defined(this->lexeme) && is_ifdef) ||
						(!is_directive_defined(this->lexeme) && !is_ifdef))
					{
						this->directives_stack.push(0);
					}
					else
					{
						this->directives_stack.push(1);
					}

					if (this->current_symbol != '}')
					{
						print_error("Unclosed directive");
						exit(1);
					}

					if (this->directives_stack.top() == 1)
					{
						get_next_symbol();
						this->lexeme = "";

						while (this->current_symbol != EOF)
						{
							this->lexeme += this->current_symbol;
							if ((this->lexeme.length() >= 8 &&
								 strcasecmp(&this->lexeme.c_str()[this->lexeme.size() - 8], "{$ifdef ") == 0) ||
								(this->lexeme.length() >= 9 &&
								 strcasecmp(&this->lexeme.c_str()[this->lexeme.size() - 9], "{$ifndef ") == 0))
							{
								get_next_symbol();
								append_sequence([](char ch) { return isalnum(ch) ||
																	 (ch == '_'); });

								directives_stack.push(2);
								if (this->current_symbol != '}')
								{
									print_error("Unclosed directive");
									exit(1);
								}
								get_next_symbol();
							}
							if (this->lexeme.length() >= 7 &&
								strcasecmp(&this->lexeme.c_str()[this->lexeme.size() - 7], "{$endif") == 0)
							{
								if (directives_stack.top() != 2)
								{
									directives_stack.pop();
									get_next_symbol();
									if (this->current_symbol != '}')
									{
										print_error("Unclosed directive");
										exit(1);
									}
									break;
								}

								directives_stack.pop();
								get_next_symbol();
								if (this->current_symbol != '}')
								{
									print_error("Unclosed directive");
									exit(1);
								}
							}
							if (this->lexeme.length() >= 6 &&
								strcasecmp(&this->lexeme.c_str()[this->lexeme.size() - 6], "{$else") == 0)
							{
								if (directives_stack.top() != 2)
								{
									get_next_symbol();
									if (this->current_symbol != '}')
									{
										print_error("Unclosed directive");
										exit(1);
									}
									break;
								}
								get_next_symbol();
								if (this->current_symbol != '}')
								{
									print_error("Unclosed directive");
									exit(1);
								}
							}
							get_next_symbol();
						}

						if (this->current_symbol == EOF)
						{
							print_error("Missing endif");
							exit(1);
						}
					}
				}

				if (strcasecmp("$else", this->lexeme.c_str()) == 0)
				{
					get_next_symbol();
					if (this->current_symbol != '}')
					{
						print_error("Unclosed directive");
						exit(1);
					}

					if (!directives_stack.empty())
					{
						if (directives_stack.top() == 0)
						{
							this->lexeme = "";
							while (this->current_symbol != EOF)
							{
								this->lexeme += this->current_symbol;
								if (this->lexeme.length() >= 7 &&
									strcasecmp(&this->lexeme.c_str()[this->lexeme.size() - 7], "{$endif") == 0)
								{
									directives_stack.pop();
									get_next_symbol();
									if (this->current_symbol != '}')
									{
										print_error("Unclosed directive");
										exit(1);
									}
									break;
								}
								get_next_symbol();
							}
							if (this->current_symbol == EOF)
							{
								print_error("Missing endif");
								exit(1);
							}
						}
					}
					else
					{
						print_error("Orphan else");
						exit(1);
					}
				}

				if (strcasecmp("$endif", this->lexeme.c_str()) == 0)
				{
					if (!directives_stack.empty())
					{
						directives_stack.pop();
						get_next_symbol();
						if (this->current_symbol != '}')
						{
							print_error("Unclosed directive");
							exit(1);
						}
					}
					else
					{
						print_error("Orphan endif");
						exit(1);
					}
				}

				if (this->current_symbol == '}')
				{
					break;
				}
				else
				{
					get_next_symbol();
				}
			} while (this->current_symbol != EOF);
			if (this->current_symbol == EOF)
			{
				print_error("Unclosed comment");
				exit(1);
			}
			this->lexeme = "";
			get_next_symbol();
			break;
		case '.':
			get_next_symbol();
			if (this->current_symbol == '.')
			{
				this->lexeme = "..";
				get_next_symbol();
				return Symbol::DotDot;
			}
			this->lexeme = ".";
			return Symbol::Dot;
		case EOF:
			if (!directives_stack.empty())
			{
				print_error("Missing endif");
				exit(1);
			}
			RETURN_TOKEN(Symbol::Eof);
		default:
			if (isdigit(this->current_symbol))
			{
				append_sequence([](char ch) { return isdigit(ch); });
				return Symbol::IntConstantDec;
			}
			else if (this->current_symbol == '$')
			{
				get_next_symbol();
				if (isxdigit(this->current_symbol))
				{
					append_sequence([](char ch) { return isxdigit(ch); });
					return Symbol::IntConstantHex;
				}
				print_error("Expected hexadecimal int constant");
				exit(1);
			}
			else if (this->current_symbol == '%')
			{
				get_next_symbol();
				if (this->current_symbol == '0' || this->current_symbol == '1')
				{
					append_sequence([](char ch) { return ch == '0' ||
														 ch == '1'; });
					return Symbol::IntConstantBin;
				}
				print_error("Expected binary int constant");
				exit(1);
			}
			else if (isalpha(this->current_symbol) || this->current_symbol == '_')
			{
				append_sequence([](char ch) { return isalnum(ch) ||
													 (ch == '_'); });
				return look_up_keyword();
			}
			else
			{
				print_error("Invalid symbol " + std::string(1, this->current_symbol));
				exit(1);
			}
		}
	}
}
