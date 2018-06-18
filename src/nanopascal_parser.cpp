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
}

void NanoPascalParser::assign()
{
}

void NanoPascalParser::lvalue()
{
}

void NanoPascalParser::expr()
{
}

void NanoPascalParser::exprpfi()
{
}

void NanoPascalParser::exprpse()
{
}

void NanoPascalParser::exprpth()
{
}

void NanoPascalParser::exprpfo()
{
}

void NanoPascalParser::exprpla()
{
}

void NanoPascalParser::subprogram_call()
{
}

void NanoPascalParser::expr_list()
{
}

void NanoPascalParser::argument_list()
{
}

void NanoPascalParser::argument()
{
}

void NanoPascalParser::constant()
{
}

void NanoPascalParser::bool_constant()
{
}

void NanoPascalParser::block()
{
}
