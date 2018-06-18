#include "nanopascal_parser.h"

void NanoPascalParser::parse()
{
	this->current_token = this->lexer.get_next_token();
	program();
	expected_token(Symbol::Eof, "'EOF'");
	std::cout << "Parsed" << std::endl;
}

void NanoPascalParser::print_error(std::string expected)
{
	std::cerr << "parser:"
			  << this->lexer.get_line_number()
			  << ":" << this->lexer.get_column_number()
			  << ": expected " << expected
			  << " got '" << this->lexer.get_lexeme() << "'"
			  << std::endl;
}

void NanoPascalParser::program()
{
	expected_token(Symbol::KwProgram, "'program'");
	expected_token(Symbol::ID, "'id'");
	expected_token(Symbol::Semicolon, "';'");

	if (this->current_token == Symbol::KwVar)
	{
		variable_section();
	}

	while (this->current_token == Symbol::KwFunction ||
		   this->current_token == Symbol::KwProcedure)
	{
		subprogram_decl();
	}

	expected_token(Symbol::KwBegin, "'begin'");

	while (this->current_token == Symbol::ID ||
		   this->current_token == Symbol::KwWrite ||
		   this->current_token == Symbol::KwWriteln ||
		   this->current_token == Symbol::KwRead ||
		   this->current_token == Symbol::KwIf ||
		   this->current_token == Symbol::KwWhile ||
		   this->current_token == Symbol::KwRepeat ||
		   this->current_token == Symbol::KwFor ||
		   this->current_token == Symbol::KwBreak ||
		   this->current_token == Symbol::KwContinue)
	{
		statement();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Dot, "'.'");
}

void NanoPascalParser::variable_section()
{
	expected_token(Symbol::KwVar, "'var'");
	do
	{
		variable_decl();
	} while (this->current_token == Symbol::ID);
}

void NanoPascalParser::variable_decl()
{
	id_list();

	expected_token(Symbol::Colon, "':'");

	type();

	expected_token(Symbol::Semicolon, "';'");
}

void NanoPascalParser::id_list()
{
	expected_token(Symbol::ID, "'id'");

	while (this->current_token == Symbol::Comma)
	{
		get_next_token();
		expected_token(Symbol::ID, "'id'");
	}
}

void NanoPascalParser::type()
{
	if (this->current_token == Symbol::KwInteger ||
		this->current_token == Symbol::KwBoolean ||
		this->current_token == Symbol::KwChar)
	{
		get_next_token();
	}
	else if (this->current_token == Symbol::KwArray)
	{
		array_type();
	}
	else
	{
		print_error("'integer', 'boolean', 'char' or 'array'");
		exit(1);
	}
}

void NanoPascalParser::array_type()
{
	expected_token(Symbol::KwArray, "'array'");
	expected_token(Symbol::OpenBra, "'['");

	if (this->current_token != Symbol::IntConstantBin ||
		this->current_token != Symbol::IntConstantDec ||
		this->current_token != Symbol::IntConstantHex)
	{
		print_error("int constant");
		exit(1);
	}
	get_next_token();

	expected_token(Symbol::DotDot, "'..'");

	if (this->current_token != Symbol::IntConstantBin ||
		this->current_token != Symbol::IntConstantDec ||
		this->current_token != Symbol::IntConstantHex)
	{
		print_error("int constant");
		exit(1);
	}
	get_next_token();

	expected_token(Symbol::CloseBra, "']'");
	expected_token(Symbol::KwOf, "'of'");

	if (this->current_token != Symbol::KwInteger ||
		this->current_token != Symbol::KwBoolean ||
		this->current_token != Symbol::KwChar)
	{
		print_error("'integer', 'boolean' or 'char'");
		exit(1);
	}
	get_next_token();
}

void NanoPascalParser::subprogram_decl()
{
	subprogram_header();

	if (this->current_token == Symbol::KwVar)
	{
		variable_section();
	}

	expected_token(Symbol::KwBegin, "'begin'");

	while (this->current_token == Symbol::ID ||
		   this->current_token == Symbol::KwWrite ||
		   this->current_token == Symbol::KwWriteln ||
		   this->current_token == Symbol::KwRead ||
		   this->current_token == Symbol::KwIf ||
		   this->current_token == Symbol::KwWhile ||
		   this->current_token == Symbol::KwRepeat ||
		   this->current_token == Symbol::KwFor ||
		   this->current_token == Symbol::KwBreak ||
		   this->current_token == Symbol::KwContinue)
	{
		statement();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Semicolon, "';'");
}

void NanoPascalParser::subprogram_header()
{
	if (this->current_token == Symbol::KwFunction)
	{
		function_header();
	}
	else if (this->current_token == Symbol::KwProcedure)
	{
		procedure_header();
	}
	else
	{
		print_error("'function' or 'procedure'");
		exit(1);
	}
}

void NanoPascalParser::function_header()
{
	expected_token(Symbol::KwFunction, "'function'");
	expected_token(Symbol::ID, "'id'");

	if (this->current_token == Symbol::OpenPar)
	{
		get_next_token();
		if (this->current_token == Symbol::KwVar ||
			this->current_token == Symbol::ID)
		{
			argument_decl_list();
		}

		expected_token(Symbol::ClosePar, "')'");
	}

	expected_token(Symbol::Colon, "':'");

	type();

	expected_token(Symbol::Semicolon, "';'");
}

void NanoPascalParser::procedure_header()
{
	expected_token(Symbol::KwProcedure, "'procedure'");
	expected_token(Symbol::ID, "'id'");

	if (this->current_token == Symbol::OpenPar)
	{
		get_next_token();
		if (this->current_token == Symbol::KwVar ||
			this->current_token == Symbol::ID)
		{
			argument_decl_list();
		}

		expected_token(Symbol::ClosePar, "')'");
	}

	expected_token(Symbol::Semicolon, "';'");
}

void NanoPascalParser::argument_decl_list()
{
	argument_decl();
	while (this->current_token == Symbol::Semicolon)
	{
		get_next_token();
		argument_decl();
	}
}

void NanoPascalParser::argument_decl()
{
	if (this->current_token == Symbol::KwVar)
	{
		get_next_token();
		expected_token(Symbol::ID, "'id'");
	}
	else if (this->current_token == Symbol::ID)
	{
		get_next_token();
	}
	else
	{
		print_error("'var' or 'id'");
		exit(1);
	}

	expected_token(Symbol::Colon, "':'");

	type();
}

void NanoPascalParser::statement()
{
	if (this->current_token == Symbol::ID)
	{
		assign();
		expected_token(Symbol::Semicolon, "';'");
	}
	else if (this->current_token == Symbol::ID ||
			 this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead)
	{
		subprogram_call();
		expected_token(Symbol::Semicolon, "';'");
	}
	else if (this->current_token == Symbol::KwIf)
	{
		get_next_token();
		expr();

		expected_token(Symbol::KwThen, "'then'");

		block();

		if (this->current_token == Symbol::KwElse)
		{
			get_next_token();
			block();
		}
	}
	else if (this->current_token == Symbol::KwWhile)
	{
		get_next_token();
		expr();

		expected_token(Symbol::KwDo, "'do'");

		block();
	}
	else if (this->current_token == Symbol::KwRepeat)
	{
		get_next_token();
		block();

		expected_token(Symbol::KwUntil, "'until'");

		expr();

		expected_token(Symbol::Semicolon, "';'");
	}
	else if (this->current_token == Symbol::KwFor)
	{
		get_next_token();
		assign();

		expected_token(Symbol::KwTo, "'to'");

		expr();

		expected_token(Symbol::KwDo, "'do'");

		block();
	}
	else if (this->current_token == Symbol::KwBreak)
	{
		get_next_token();
		expected_token(Symbol::Semicolon, "';'");
	}
	else if (this->current_token == Symbol::KwContinue)
	{
		get_next_token();
		expected_token(Symbol::Semicolon, "';'");
	}
	else
	{
		print_error("'id', 'write', 'writeln', 'read', 'if', 'while', 'repeat', 'for', 'break' or'continue'");
		exit(1);
	}
}

void NanoPascalParser::assign()
{
	lvalue();
	expected_token(Symbol::Assign, "':='");
	expr();
}

void NanoPascalParser::lvalue()
{
	expected_token(Symbol::ID, "'id'");

	if (this->current_token == Symbol::OpenBra)
	{
		get_next_token();
		expr();

		expected_token(Symbol::CloseBra, "']'");
	}
}

void NanoPascalParser::expr()
{
	if (this->current_token == Symbol::OpenPar ||
		this->current_token == Symbol::IntConstantBin ||
		this->current_token == Symbol::IntConstantDec ||
		this->current_token == Symbol::IntConstantHex ||
		this->current_token == Symbol::CharConstant ||
		this->current_token == Symbol::KwTrue ||
		this->current_token == Symbol::KwFalse ||
		this->current_token == Symbol::ID ||
		this->current_token == Symbol::KwWrite ||
		this->current_token == Symbol::KwWriteln ||
		this->current_token == Symbol::KwRead ||
		this->current_token == Symbol::KwNot ||
		this->current_token == Symbol::OpSub)
	{
		exprpfi();
	}
	else
	{
		print_error("'(', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}
}

void NanoPascalParser::exprpfi()
{
	exprpse();
	while (this->current_token == Symbol::OpEqual ||
		   this->current_token == Symbol::OpNotEqual ||
		   this->current_token == Symbol::OpLessThan ||
		   this->current_token == Symbol::OpGreaterThan ||
		   this->current_token == Symbol::OpLessThanOrEqual ||
		   this->current_token == Symbol::OpGreaterThanOrEqual)
	{
		get_next_token();
		exprpse();
	}
}

void NanoPascalParser::exprpse()
{
	exprpth();
	while (this->current_token == Symbol::OpAdd ||
		   this->current_token == Symbol::OpSub ||
		   this->current_token == Symbol::KwOr ||
		   this->current_token == Symbol::KwXor)
	{
		get_next_token();
		exprpth();
	}
}

void NanoPascalParser::exprpth()
{
	exprpla();
	while (this->current_token == Symbol::OpMul ||
		   this->current_token == Symbol::KwDiv ||
		   this->current_token == Symbol::KwMod ||
		   this->current_token == Symbol::KwAnd ||
		   this->current_token == Symbol::KwShl ||
		   this->current_token == Symbol::KwShr ||
		   this->current_token == Symbol::OpLeftShift ||
		   this->current_token == Symbol::OpRightShift)
	{
		get_next_token();
		exprpla();
	}
}

void NanoPascalParser::exprpla()
{
	if (this->current_token == Symbol::OpenPar)
	{
		get_next_token();
		exprpfi();

		expected_token(Symbol::ClosePar, "')'");
	}
	else if (this->current_token == Symbol::IntConstantBin ||
			 this->current_token == Symbol::IntConstantDec ||
			 this->current_token == Symbol::IntConstantHex ||
			 this->current_token == Symbol::CharConstant ||
			 this->current_token == Symbol::KwTrue ||
			 this->current_token == Symbol::KwFalse)
	{
		constant();
	}
	else if (this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead)
	{
		subprogram_call();
	}
	else if (this->current_token == Symbol::ID) // lvalue and subprogram-call for id
	{
		get_next_token();

		if (this->current_token == Symbol::OpenBra)
		{
			get_next_token();
			expr();

			expected_token(Symbol::CloseBra, "']'");
		}
		else if (this->current_token == Symbol::OpenPar)
		{
			get_next_token();
			expr_list();

			expected_token(Symbol::ClosePar, "')'");
		}
	}
	else if (this->current_token == Symbol::KwNot ||
			 this->current_token == Symbol::OpSub)
	{
		get_next_token();
		expr();
	}
	else
	{
		print_error("'(', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}
}

void NanoPascalParser::subprogram_call()
{
	// Id allready consumed by exprpla
	if (this->current_token == Symbol::KwWrite)
	{
		get_next_token();
		expected_token(Symbol::OpenPar, "'('");

		argument_list();

		expected_token(Symbol::ClosePar, "')'");
	}
	if (this->current_token == Symbol::KwWriteln)
	{
		get_next_token();

		if (this->current_token == Symbol::OpenPar)
		{
			get_next_token();

			if (this->current_token == Symbol::StringConstant ||
				this->current_token == Symbol::OpenPar ||
				this->current_token == Symbol::IntConstantBin ||
				this->current_token == Symbol::IntConstantDec ||
				this->current_token == Symbol::IntConstantHex ||
				this->current_token == Symbol::CharConstant ||
				this->current_token == Symbol::KwTrue ||
				this->current_token == Symbol::KwFalse ||
				this->current_token == Symbol::ID ||
				this->current_token == Symbol::KwWrite ||
				this->current_token == Symbol::KwWriteln ||
				this->current_token == Symbol::KwRead ||
				this->current_token == Symbol::KwNot ||
				this->current_token == Symbol::OpSub)
			{
				argument_list();
			}

			expected_token(Symbol::ClosePar, "')'");
		}
	}
}

void NanoPascalParser::expr_list()
{
	expr();
	while (this->current_token == Symbol::Comma)
	{
		get_next_token();
		expr();
	}
}

void NanoPascalParser::argument_list()
{
	argument();
	while (this->current_token == Symbol::Comma)
	{
		get_next_token();
		argument();
	}
}

void NanoPascalParser::argument()
{
	if (this->current_token == Symbol::StringConstant)
	{
		get_next_token();
	}
	else if (this->current_token == Symbol::OpenPar ||
			 this->current_token == Symbol::IntConstantBin ||
			 this->current_token == Symbol::IntConstantDec ||
			 this->current_token == Symbol::IntConstantHex ||
			 this->current_token == Symbol::CharConstant ||
			 this->current_token == Symbol::KwTrue ||
			 this->current_token == Symbol::KwFalse ||
			 this->current_token == Symbol::ID ||
			 this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead ||
			 this->current_token == Symbol::KwNot ||
			 this->current_token == Symbol::OpSub)
	{
		expr();
	}
	else
	{
		print_error("'string constant', (', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}
}

void NanoPascalParser::constant()
{
	if (this->current_token == Symbol::IntConstantBin ||
		this->current_token == Symbol::IntConstantDec ||
		this->current_token == Symbol::IntConstantHex ||
		this->current_token == Symbol::CharConstant ||
		this->current_token == Symbol::KwTrue ||
		this->current_token == Symbol::KwFalse)
	{
		get_next_token();
	}
	else
	{
		print_error("'int constant', 'char constant', 'true' or 'false'");
		exit(1);
	}
}

void NanoPascalParser::block()
{
	if (this->current_token == Symbol::ID ||
		this->current_token == Symbol::KwWrite ||
		this->current_token == Symbol::KwWriteln ||
		this->current_token == Symbol::KwRead ||
		this->current_token == Symbol::KwIf ||
		this->current_token == Symbol::KwWhile ||
		this->current_token == Symbol::KwRepeat ||
		this->current_token == Symbol::KwFor ||
		this->current_token == Symbol::KwBreak ||
		this->current_token == Symbol::KwContinue)
	{
		statement();
	}
	else if (this->current_token == Symbol::KwBegin)
	{
		get_next_token();

		while (this->current_token == Symbol::ID ||
			   this->current_token == Symbol::KwWrite ||
			   this->current_token == Symbol::KwWriteln ||
			   this->current_token == Symbol::KwRead ||
			   this->current_token == Symbol::KwIf ||
			   this->current_token == Symbol::KwWhile ||
			   this->current_token == Symbol::KwRepeat ||
			   this->current_token == Symbol::KwFor ||
			   this->current_token == Symbol::KwBreak ||
			   this->current_token == Symbol::KwContinue)
		{
			statement();
		}

		expected_token(Symbol::KwEnd, "'end'");
		expected_token(Symbol::Semicolon, "';'");
	}
	else
	{
		print_error("'id', 'write', 'writeln', 'read', 'if', 'while', 'repeat', 'for', 'break', 'continue' or 'begin'");
	}
}
