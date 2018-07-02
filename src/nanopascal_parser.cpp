#include "nanopascal_parser.h"
#include <stdlib.h>

void NanoPascalParser::parse()
{
	this->current_token = this->lexer.get_next_token();
	UP_ProgramNode o_program_node = program();
	expected_token(Symbol::Eof, "'EOF'");
	// std::cout << o_program_node->to_string() << "\nParsed" << std::endl;
	std::map<std::string, ReturnType> variables_type;
	std::map<std::string, std::map<int, int>> variables_value;
	std::map<std::string, UP_SubprogramDeclNode> functions;
	o_program_node->exec(variables_type, variables_value, functions);
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

	StatementList o_statement_list;
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
		o_statement_list = statement_list();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Dot, "'.'");

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

	if (this->current_token != Symbol::IntConstantBin &&
		this->current_token != Symbol::IntConstantDec &&
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

	if (this->current_token != Symbol::IntConstantBin &&
		this->current_token != Symbol::IntConstantDec &&
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

	if (this->current_token != Symbol::KwInteger &&
		this->current_token != Symbol::KwBoolean &&
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
	std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> o_subprogram_header =
		subprogram_header();

	VariableDeclList o_variable_section;
	if (this->current_token == Symbol::KwVar)
	{
		variable_section();
	}

	expected_token(Symbol::KwBegin, "'begin'");

	StatementList o_statement_list;
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
		o_statement_list = statement_list();
	}

	expected_token(Symbol::KwEnd, "'end'");
	expected_token(Symbol::Semicolon, "';'");

	return std::make_unique<SubprogramDeclNode>(std::get<0>(o_subprogram_header),
												std::move(std::get<1>(o_subprogram_header)),
												std::get<2>(o_subprogram_header),
												std::get<3>(o_subprogram_header),
												std::get<4>(o_subprogram_header),
												std::move(o_variable_section),
												std::move(o_statement_list));
}

std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> NanoPascalParser::subprogram_header()
{
	if (this->current_token == Symbol::KwFunction)
	{
		return function_header();
	}
	else if (this->current_token == Symbol::KwProcedure)
	{
		return procedure_header();
	}
	else
	{
		print_error("'function' or 'procedure'");
		exit(1);
	}
	ArgumentDeclList o_argument_decl_list;
	return std::make_tuple("", std::move(o_argument_decl_list), ReturnType::Non, -1, -1);
}

std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> NanoPascalParser::function_header()
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
			o_argument_decl_list = argument_decl_list();
		}

		expected_token(Symbol::ClosePar, "')'");
	}

	expected_token(Symbol::Colon, "':'");

	std::tuple<ReturnType, int, int> o_type = type();

	expected_token(Symbol::Semicolon, "';'");

	return std::make_tuple(id,
						   std::move(o_argument_decl_list),
						   std::get<0>(o_type),
						   std::get<1>(o_type),
						   std::get<2>(o_type));
}

std::tuple<std::string, ArgumentDeclList, ReturnType, int, int> NanoPascalParser::procedure_header()
{
	expected_token(Symbol::KwProcedure, "'procedure'");

	std::string id = this->lexer.get_lexeme();
	expected_token(Symbol::ID, "'id'");

	ArgumentDeclList o_argument_decl_list;
	if (this->current_token == Symbol::OpenPar)
	{
		get_next_token();
		if (this->current_token == Symbol::KwVar ||
			this->current_token == Symbol::ID)
		{
			o_argument_decl_list = argument_decl_list();
		}

		expected_token(Symbol::ClosePar, "')'");
	}

	expected_token(Symbol::Semicolon, "';'");

	return std::make_tuple(id, std::move(o_argument_decl_list), ReturnType::Non, -1, -1);
}

ArgumentDeclList NanoPascalParser::argument_decl_list()
{
	ArgumentDeclList o_argument_decl_list;

	o_argument_decl_list.push_back(argument_decl());
	while (this->current_token == Symbol::Semicolon)
	{
		get_next_token();
		o_argument_decl_list.push_back(argument_decl());
	}

	return std::move(o_argument_decl_list);
}

UP_ArgumentDeclNode NanoPascalParser::argument_decl()
{
	std::string id = "";
	if (this->current_token == Symbol::KwVar)
	{
		get_next_token();

		id = this->lexer.get_lexeme();
		expected_token(Symbol::ID, "'id'");
	}
	else if (this->current_token == Symbol::ID)
	{
		id = this->lexer.get_lexeme();
		get_next_token();
	}
	else
	{
		print_error("'var' or 'id'");
		exit(1);
	}

	expected_token(Symbol::Colon, "':'");

	std::tuple<ReturnType, int, int> o_type = type();

	return std::make_unique<ArgumentDeclNode>(id,
											  std::get<0>(o_type),
											  std::get<1>(o_type),
											  std::get<2>(o_type));
}

StatementList NanoPascalParser::statement_list()
{
	StatementList o_statement_list;

	o_statement_list.push_back(statement());
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
			o_statement_list.push_back(statement());
		}
	}

	return o_statement_list;
}

UP_StatementNode NanoPascalParser::statement()
{
	if (this->current_token == Symbol::ID)
	{
		std::string id = this->lexer.get_lexeme();
		get_next_token();

		ASTNodelList ast_node_list;
		if (this->current_token == Symbol::Assign) // assign
		{
			get_next_token();
			UP_ASTNode expr1 = expr();

			return std::make_unique<AssignNode>(id, nullptr, std::move(expr1));
		}
		else if (this->current_token == Symbol::OpenBra)
		{
			get_next_token();
			UP_ASTNode index = expr();

			expected_token(Symbol::CloseBra, "']'");
			expected_token(Symbol::Assign, "':='");

			UP_ASTNode expr1 = expr();

			return std::make_unique<AssignNode>(id, std::move(index), std::move(expr1));
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
				ast_node_list = expr_list();
			}

			expected_token(Symbol::ClosePar, "')'");
		}

		return std::make_unique<SubprogramCallNode>(id, std::move(ast_node_list));
	}
	else if (this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead)
	{
		return subprogram_call();
	}
	else if (this->current_token == Symbol::KwIf)
	{
		get_next_token();
		UP_ASTNode o_expr = expr();

		expected_token(Symbol::KwThen, "'then'");

		StatementList o_statement_list1 = block();

		StatementList o_statement_list2;
		if (this->current_token == Symbol::KwElse)
		{
			get_next_token();
			o_statement_list2 = block();
		}

		return std::make_unique<IfNode>(std::move(o_expr),
										std::move(o_statement_list1),
										std::move(o_statement_list2));
	}
	else if (this->current_token == Symbol::KwWhile)
	{
		get_next_token();
		UP_ASTNode o_expr = expr();

		expected_token(Symbol::KwDo, "'do'");

		StatementList o_statement_list = block();

		return std::make_unique<WhileNode>(std::move(o_expr), std::move(o_statement_list));
	}
	else if (this->current_token == Symbol::KwRepeat)
	{
		get_next_token();
		StatementList o_statement_list = block();

		expected_token(Symbol::KwUntil, "'until'");

		UP_ASTNode o_expr = expr();

		expected_token(Symbol::Semicolon, "';'");

		return std::make_unique<RepeatNode>(std::move(o_statement_list), std::move(o_expr));
	}
	else if (this->current_token == Symbol::KwFor)
	{
		get_next_token();

		std::string id = this->lexer.get_lexeme();
		expected_token(Symbol::ID, "'id'");

		UP_ASTNode index;
		if (this->current_token == Symbol::OpenBra)
		{
			get_next_token();
			index = expr();

			expected_token(Symbol::CloseBra, "']'");
		}

		expected_token(Symbol::Assign, "':='");
		UP_ASTNode expr1 = expr();

		UP_AssignNode assign_node = std::make_unique<AssignNode>(id,
																 std::move(index),
																 std::move(expr1));

		expected_token(Symbol::KwTo, "'to'");

		UP_ASTNode expr2 = expr();

		expected_token(Symbol::KwDo, "'do'");

		StatementList o_statement_list = block();

		return std::make_unique<ForNode>(std::move(assign_node),
										 std::move(expr2),
										 std::move(o_statement_list));
	}
	else if (this->current_token == Symbol::KwBreak)
	{
		get_next_token();

		return std::make_unique<BranchingStatementNode>(BranchingStatement::Break);
	}
	else if (this->current_token == Symbol::KwContinue)
	{
		get_next_token();

		return std::make_unique<BranchingStatementNode>(BranchingStatement::Continue);
	}
	else
	{
		print_error("'id', 'write', 'writeln', 'read', 'if', 'while', 'repeat', 'for', 'break' or'continue'");
		exit(1);
	}

	return nullptr;
}

UP_ASTNode NanoPascalParser::expr()
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
		return exprpfi();
	}
	else
	{
		print_error("'(', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}

	return nullptr;
}

UP_ASTNode NanoPascalParser::exprpfi()
{
	UP_ASTNode expr1 = exprpse();
	while (this->current_token == Symbol::OpEqual ||
		   this->current_token == Symbol::OpNotEqual ||
		   this->current_token == Symbol::OpLessThan ||
		   this->current_token == Symbol::OpGreaterThan ||
		   this->current_token == Symbol::OpLessThanOrEqual ||
		   this->current_token == Symbol::OpGreaterThanOrEqual)
	{
		Symbol last_token = this->current_token;
		get_next_token();

		UP_ASTNode expr2 = exprpse();
		switch (last_token)
		{
		case Symbol::OpEqual:
			expr1 = std::make_unique<EqExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpNotEqual:
			expr1 = std::make_unique<NEqExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpLessThan:
			expr1 = std::make_unique<LTExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpGreaterThan:
			expr1 = std::make_unique<GTExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpLessThanOrEqual:
			expr1 = std::make_unique<LToEExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpGreaterThanOrEqual:
			expr1 = std::make_unique<GToEExprNode>(std::move(expr1), std::move(expr2));
			break;
		}
	}

	return std::move(expr1);
}

UP_ASTNode NanoPascalParser::exprpse()
{
	UP_ASTNode expr1 = exprpth();
	while (this->current_token == Symbol::OpAdd ||
		   this->current_token == Symbol::OpSub ||
		   this->current_token == Symbol::KwOr ||
		   this->current_token == Symbol::KwXor)
	{
		Symbol last_token = this->current_token;
		get_next_token();

		UP_ASTNode expr2 = exprpth();
		switch (last_token)
		{
		case Symbol::OpAdd:
			expr1 = std::make_unique<AddExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpSub:
			expr1 = std::make_unique<SubExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwOr:
			expr1 = std::make_unique<OrExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwXor:
			expr1 = std::make_unique<XorExprNode>(std::move(expr1), std::move(expr2));
			break;
		}
	}

	return std::move(expr1);
}

UP_ASTNode NanoPascalParser::exprpth()
{
	UP_ASTNode expr1 = exprpla();
	while (this->current_token == Symbol::OpMul ||
		   this->current_token == Symbol::KwDiv ||
		   this->current_token == Symbol::KwMod ||
		   this->current_token == Symbol::KwAnd ||
		   this->current_token == Symbol::KwShl ||
		   this->current_token == Symbol::KwShr ||
		   this->current_token == Symbol::OpLeftShift ||
		   this->current_token == Symbol::OpRightShift)
	{
		Symbol last_token = this->current_token;
		get_next_token();

		UP_ASTNode expr2 = exprpla();
		switch (last_token)
		{
		case Symbol::OpMul:
			expr1 = std::make_unique<MulExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwDiv:
			expr1 = std::make_unique<DivExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwMod:
			expr1 = std::make_unique<ModExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwAnd:
			expr1 = std::make_unique<AndExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwShl:
			expr1 = std::make_unique<ShlExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::KwShr:
			expr1 = std::make_unique<ShrExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpLeftShift:
			expr1 = std::make_unique<LSExprNode>(std::move(expr1), std::move(expr2));
			break;
		case Symbol::OpRightShift:
			expr1 = std::make_unique<RSExprNode>(std::move(expr1), std::move(expr2));
			break;
		}
	}

	return std::move(expr1);
}

UP_ASTNode NanoPascalParser::exprpla()
{
	if (this->current_token == Symbol::OpenPar)
	{
		get_next_token();
		UP_ASTNode expr1 = exprpfi();

		expected_token(Symbol::ClosePar, "')'");
		return std::move(expr1);
	}
	else if (this->current_token == Symbol::IntConstantBin ||
			 this->current_token == Symbol::IntConstantDec ||
			 this->current_token == Symbol::IntConstantHex ||
			 this->current_token == Symbol::CharConstant ||
			 this->current_token == Symbol::KwTrue ||
			 this->current_token == Symbol::KwFalse)
	{
		return constant();
	}
	else if (this->current_token == Symbol::KwWrite ||
			 this->current_token == Symbol::KwWriteln ||
			 this->current_token == Symbol::KwRead)
	{
		return subprogram_call();
	}
	else if (this->current_token == Symbol::ID) // lvalue and subprogram-call for id
	{
		std::string id = this->lexer.get_lexeme();
		get_next_token();

		UP_ASTNode expr1;
		if (this->current_token == Symbol::OpenBra)
		{
			get_next_token();
			expr1 = expr();

			expected_token(Symbol::CloseBra, "']'");
		}
		else if (this->current_token == Symbol::OpenPar)
		{
			get_next_token();

			ASTNodelList ast_node_list;
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
				ast_node_list = expr_list();
			}

			expected_token(Symbol::ClosePar, "')'");
			return std::make_unique<SubprogramCallNode>(id, std::move(ast_node_list));
		}

		return std::make_unique<IdNode>(id, std::move(expr1));
	}
	else if (this->current_token == Symbol::KwNot ||
			 this->current_token == Symbol::OpSub)
	{
		Symbol last_token = this->current_token;

		get_next_token();

		UP_ASTNode expr1 = expr();

		switch (last_token)
		{
		case Symbol::KwNot:
			return std::make_unique<NotExprNode>(std::move(expr1));
		case Symbol::OpSub:
			return std::make_unique<UnaryExprNode>(std::move(expr1));
		}
	}
	else
	{
		print_error("'(', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}

	return nullptr;
}

UP_StatementNode NanoPascalParser::subprogram_call()
{
	// Id allready consumed by exprpla
	if (this->current_token == Symbol::KwWrite)
	{
		get_next_token();
		expected_token(Symbol::OpenPar, "'('");

		ASTNodelList ast_node_list;
		ast_node_list = argument_list();

		expected_token(Symbol::ClosePar, "')'");

		return std::make_unique<SubprogramCallNode>("write", std::move(ast_node_list));
	}
	else if (this->current_token == Symbol::KwWriteln)
	{
		get_next_token();

		ASTNodelList ast_node_list;
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
				ast_node_list = argument_list();
			}

			expected_token(Symbol::ClosePar, "')'");
		}

		return std::make_unique<SubprogramCallNode>("writeln", std::move(ast_node_list));
	}
	else if (this->current_token == Symbol::KwRead)
	{
		get_next_token();

		expected_token(Symbol::OpenPar, "'('");

		ASTNodelList ast_node_list;
		ast_node_list = argument_list();

		expected_token(Symbol::ClosePar, "')'");

		return std::make_unique<SubprogramCallNode>("read", std::move(ast_node_list));
	}

	return nullptr;
}

ASTNodelList NanoPascalParser::expr_list()
{
	ASTNodelList ast_node_list;

	ast_node_list.push_back(expr());
	while (this->current_token == Symbol::Comma)
	{
		get_next_token();
		ast_node_list.push_back(expr());
	}

	return std::move(ast_node_list);
}

ASTNodelList NanoPascalParser::argument_list()
{
	ASTNodelList ast_node_list;

	ast_node_list.push_back(argument());
	while (this->current_token == Symbol::Comma)
	{
		get_next_token();
		ast_node_list.push_back(argument());
	}

	return ast_node_list;
}

UP_ASTNode NanoPascalParser::argument()
{
	if (this->current_token == Symbol::StringConstant)
	{
		std::string string_constant = this->lexer.get_lexeme();
		get_next_token();

		return std::make_unique<StringNode>(string_constant);
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
		return expr();
	}
	else
	{
		print_error("'string constant', (', 'int constant', 'char constant', 'true', 'false', 'id', 'write', 'writeln', 'read', 'not' or '-'");
		exit(1);
	}

	return nullptr;
}

UP_ASTNode NanoPascalParser::constant()
{
	if (this->current_token == Symbol::IntConstantBin ||
		this->current_token == Symbol::IntConstantDec ||
		this->current_token == Symbol::IntConstantHex ||
		this->current_token == Symbol::CharConstant ||
		this->current_token == Symbol::KwTrue ||
		this->current_token == Symbol::KwFalse)
	{
		Symbol last_token = this->current_token;
		std::string s_constant = this->lexer.get_lexeme();
		get_next_token();

		int number;
		switch (last_token)
		{
		case Symbol::IntConstantBin:
			number = strtol(s_constant.c_str(), nullptr, 2);
			return std::make_unique<NumberNode>(number);
			break;
		case Symbol::IntConstantDec:
			number = strtol(s_constant.c_str(), nullptr, 10);
			return std::make_unique<NumberNode>(number);
			break;
		case Symbol::IntConstantHex:
			number = strtol(s_constant.c_str(), nullptr, 16);
			return std::make_unique<NumberNode>(number);
			break;
		case Symbol::CharConstant:
			return std::make_unique<StringNode>(s_constant);
		case Symbol::KwTrue:
		case Symbol::KwFalse:
			return std::make_unique<BooleanNode>(s_constant);
		}
	}
	else
	{
		print_error("'int constant', 'char constant', 'true' or 'false'");
		exit(1);
	}

	return nullptr;
}

StatementList NanoPascalParser::block()
{
	StatementList o_statement_list;
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
		o_statement_list.push_back(statement());
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
			o_statement_list.merge(statement_list());
		}

		expected_token(Symbol::KwEnd, "'end'");
		expected_token(Symbol::Semicolon, "';'");
	}
	else
	{
		print_error("'id', 'write', 'writeln', 'read', 'if', 'while', 'repeat', 'for', 'break', 'continue' or 'begin'");
	}

	return std::move(o_statement_list);
}
