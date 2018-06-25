#include "nanopascal_parser.h"
#include <stdlib.h>

void NanoPascalParser::parse()
{
	this->current_token = this->lexer.get_next_token();
	UP_ProgramNode o_program_node = program();
	expected_token(Symbol::Eof, "'EOF'");
	std::cout << o_program_node->to_string() << "\nParsed" << std::endl;
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

UP_ProgramNode NanoPascalParser::program()
{
	expected_token(Symbol::KwProgram, "'program'");

	std::string id = this->lexer.get_lexeme();
	expected_token(Symbol::ID, "'id'");
	expected_token(Symbol::Semicolon, "';'");

	VariableDeclList o_variable_section;
	if (this->current_token == Symbol::KwVar)
	{
		o_variable_section = variable_section();
	}

	SubprogramDeclList o_subprogram_decl;
	while (this->current_token == Symbol::KwFunction ||
		   this->current_token == Symbol::KwProcedure)
	{
		o_subprogram_decl.push_back(subprogram_decl());
	}

	expected_token(Symbol::KwBegin, "'begin'");

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
		statement_list();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Dot, "'.'");

	StatementList o_statement_list;

	return std::make_unique<ProgramNode>(id,
										 std::move(o_variable_section),
										 std::move(o_subprogram_decl),
										 std::move(o_statement_list));
}

VariableDeclList NanoPascalParser::variable_section()
{
	expected_token(Symbol::KwVar, "'var'");

	VariableDeclList o_variable_section;
	do
	{
		o_variable_section.push_back(variable_decl());
	} while (this->current_token == Symbol::ID);

	return std::move(o_variable_section);
}

UP_VariableDeclNode NanoPascalParser::variable_decl()
{
	std::list<std::string> o_id_list;
	o_id_list = id_list();

	expected_token(Symbol::Colon, "':'");

	std::tuple<ReturnType, int, int> o_type = type();

	expected_token(Symbol::Semicolon, "';'");

	return std::make_unique<VariableDeclNode>(std::move(o_id_list),
											  std::get<0>(o_type),
											  std::get<1>(o_type),
											  std::get<2>(o_type));
}

std::list<std::string> NanoPascalParser::id_list()
{
	std::list<std::string> o_id_list;

	o_id_list.push_back(this->lexer.get_lexeme());
	expected_token(Symbol::ID, "'id'");

	while (this->current_token == Symbol::Comma)
	{
		get_next_token();

		o_id_list.push_back(this->lexer.get_lexeme());
		expected_token(Symbol::ID, "'id'");
	}

	return std::move(o_id_list);
}

std::tuple<ReturnType, int, int> NanoPascalParser::type()
{
	if (this->current_token == Symbol::KwInteger ||
		this->current_token == Symbol::KwBoolean ||
		this->current_token == Symbol::KwChar)
	{
		switch (this->current_token)
		{
		case Symbol::KwInteger:
			get_next_token();
			return std::make_tuple(ReturnType::Integer, -1, -1);
		case Symbol::KwBoolean:
			get_next_token();
			return std::make_tuple(ReturnType::Boolean, -1, -1);
		case Symbol::KwChar:
			get_next_token();
			return std::make_tuple(ReturnType::Char, -1, -1);
		}
	}
	else if (this->current_token == Symbol::KwArray)
	{
		return array_type();
	}
	else
	{
		print_error("'integer', 'boolean', 'char' or 'array'");
		exit(1);
	}
	return std::make_tuple(ReturnType::Non, -1, -1);
}

std::tuple<ReturnType, int, int> NanoPascalParser::array_type()
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

	int index1 = -1;
	switch (this->current_token)
	{
	case Symbol::IntConstantBin:
		index1 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 2);
		break;
	case Symbol::IntConstantDec:
		index1 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 10);
		break;
	case Symbol::IntConstantHex:
		index1 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 16);
		break;
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

	int index2 = -1;
	switch (this->current_token)
	{
	case Symbol::IntConstantBin:
		index2 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 2);
		break;
	case Symbol::IntConstantDec:
		index2 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 10);
		break;
	case Symbol::IntConstantHex:
		index2 = strtol(this->lexer.get_lexeme().c_str(), nullptr, 16);
		break;
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

	switch (this->current_token)
	{
	case Symbol::KwInteger:
		get_next_token();
		return std::make_tuple(ReturnType::Integer, index1, index2);
	case Symbol::KwBoolean:
		get_next_token();
		return std::make_tuple(ReturnType::Boolean, index1, index2);
	case Symbol::KwChar:
		get_next_token();
		return std::make_tuple(ReturnType::Char, index1, index2);
	}
	return std::make_tuple(ReturnType::Non, -1, -1);
}

UP_SubprogramDeclNode NanoPascalParser::subprogram_decl()
{
	std::tuple<std::string, ArgumentDeclList, ReturnType> o_subprogram_header = subprogram_header();

	if (this->current_token == Symbol::KwVar)
	{
		variable_section();
	}

	expected_token(Symbol::KwBegin, "'begin'");

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
		statement_list();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Semicolon, "';'");
}

std::tuple<std::string, ArgumentDeclList, ReturnType> NanoPascalParser::subprogram_header()
{
	if (this->current_token == Symbol::KwFunction)
	{
		return function_header();
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

std::tuple<std::string, ArgumentDeclList, ReturnType> NanoPascalParser::function_header()
{
	expected_token(Symbol::KwFunction, "'function'");

	std::string id = this->lexer.get_lexeme();
	expected_token(Symbol::ID, "'id'");

	ArgumentDeclList o_argument_decl_list;
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

	std::tuple<ReturnType, int, int> o_type = type();

	expected_token(Symbol::Semicolon, "';'");

	return std::make_tuple(id, o_argument_decl_list, std::get<0>(o_type));
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

void NanoPascalParser::statement_list()
{
	statement();
	while (this->current_token == Symbol::Semicolon)
	{
		get_next_token();
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
	}
}

void NanoPascalParser::statement()
{
	if (this->current_token == Symbol::ID)
	{
		get_next_token();

		if (this->current_token == Symbol::Assign) // assign
		{
			get_next_token();
			expr();
		}
		else if (this->current_token == Symbol::OpenBra)
		{
			get_next_token();
			expr();

			expected_token(Symbol::CloseBra, "']'");
			expected_token(Symbol::Assign, "':='");

			expr();
		}
		else if (this->current_token == Symbol::OpenPar) // subprogram_call
		{
			get_next_token();
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
				expr_list();
			}

			expected_token(Symbol::ClosePar, "')'");
		}
	}
	else if (this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead)
	{
		subprogram_call();
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
	}
	else if (this->current_token == Symbol::KwContinue)
	{
		get_next_token();
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
				expr_list();
			}

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
			statement_list();
		}

		expected_token(Symbol::KwEnd, "'end'");
		expected_token(Symbol::Semicolon, "';'");
	}
	else
	{
		print_error("'id', 'write', 'writeln', 'read', 'if', 'while', 'repeat', 'for', 'break', 'continue' or 'begin'");
	}
}
