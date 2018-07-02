#include "nanopascal_nodes.h"

std::string ProgramNode::to_string()
{
	std::string s_variable_section = "";
	if (!this->variable_section.empty())
	{
		s_variable_section = "\nvar ";
	}
	for (auto &variable_decl : this->variable_section)
	{
		s_variable_section += "\n" + variable_decl->to_string();
	}

	std::string s_subprogram_decl = "";
	for (auto &v_subprogram_decl : this->subprogram_decl)
	{
		s_subprogram_decl += "\n" + v_subprogram_decl->to_string();
	}

	std::string s_statement_list = "";
	for (auto &statement : this->statement_list)
	{
		if (statement != nullptr)
		{
			s_statement_list += "\n" + statement->to_string();
		}
	}

	return "program " + this->id + ";" +
		   s_variable_section +
		   s_subprogram_decl +
		   "\nbegin" +
		   s_statement_list +
		   "\nend.";
}

std::string SubprogramDeclNode::to_string()
{
	std::string s_subprogram_type = "";
	if (this->return_type == ReturnType::Non)
	{
		s_subprogram_type += "procedure ";
	}
	else
	{
		s_subprogram_type += "function ";
	}

	std::string s_type = "";
	if (this->index1 == -1 && this->index2 == -1)
	{
		switch (this->return_type)
		{
		case ReturnType::Integer:
			s_type = ": Integer;";
			break;
		case ReturnType::Boolean:
			s_type = ": Boolean;";
			break;
		case ReturnType::Char:
			s_type = ": Char;";
			break;
		default:
			s_type = "; ";
		}
	}
	else
	{
		switch (this->return_type)
		{
		case ReturnType::Integer:
			s_type = ": Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Integer;";
			break;
		case ReturnType::Boolean:
			s_type = ": Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Boolean;";
			break;
		case ReturnType::Char:
			s_type = ": Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Char;";
			break;
		}
	}

	std::string s_argument_decl_list = "";
	if (!this->argument_decl_list.empty() &&
		this->argument_decl_list.front() != nullptr)
	{
		auto front = std::move(this->argument_decl_list.front());
		this->argument_decl_list.pop_front();

		s_argument_decl_list = front->to_string();
		for (auto &argument_decl : this->argument_decl_list)
		{
			s_argument_decl_list += "; " + argument_decl->to_string();
		}

		this->argument_decl_list.push_front(std::move(front));
	}

	std::string s_statement_list = "";
	for (auto &statement : this->statement_list)
	{
		if (statement != nullptr)
			s_statement_list += "\n" + statement->to_string();
	}

	return s_subprogram_type +
		   this->id + "(" + s_argument_decl_list + ")" + s_type + "\nbegin" +
		   s_statement_list +
		   "\nend;";
}

std::string VariableDeclNode::to_string()
{
	std::string s_id_list = "";
	if (!this->id_list.empty())
	{
		auto front = this->id_list.front();
		this->id_list.pop_front();

		s_id_list = front;
		for (auto id : this->id_list)
		{
			s_id_list += ", " + id;
		}

		this->id_list.push_front(front);
	}

	std::string s_type = "";
	if (this->index1 == -1 && this->index2 == -1)
	{
		switch (this->type)
		{
		case ReturnType::Integer:
			s_type = "Integer;";
			break;
		case ReturnType::Boolean:
			s_type = "Boolean;";
			break;
		case ReturnType::Char:
			s_type = "Char;";
			break;
		}
	}
	else
	{
		switch (this->type)
		{
		case ReturnType::Integer:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Integer;";
			break;
		case ReturnType::Boolean:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Boolean;";
			break;
		case ReturnType::Char:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Char;";
			break;
		}
	}

	return s_id_list + ": " + s_type;
}

std::string ArgumentDeclNode::to_string()
{
	std::string s_type = "";
	if (this->index1 == -1 && this->index2 == -1)
	{
		switch (this->type)
		{
		case ReturnType::Integer:
			s_type = "Integer";
			break;
		case ReturnType::Boolean:
			s_type = "Boolean";
			break;
		case ReturnType::Char:
			s_type = "Char";
			break;
		}
	}
	else
	{
		switch (this->type)
		{
		case ReturnType::Integer:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Integer";
			break;
		case ReturnType::Boolean:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Boolean";
			break;
		case ReturnType::Char:
			s_type = "Array [" +
					 std::to_string(index1) +
					 ".." +
					 std::to_string(index2) +
					 "] of Char";
			break;
		}
	}

	return id + ": " + s_type;
}

std::string AssignNode::to_string()
{
	return this->id +
		   (this->index != nullptr ? "[" +
										 this->index->to_string() +
										 "]"
								   : "") +
		   " := " +
		   this->expr->to_string();
}

std::string SubprogramCallNode::to_string()
{
	std::string s_ast_node_list = "";
	if (!this->ast_node_list.empty() &&
		this->ast_node_list.front() != nullptr)
	{
		auto front = std::move(this->ast_node_list.front());
		this->ast_node_list.pop_front();

		s_ast_node_list = front->to_string();
		for (auto &ast_node : this->ast_node_list)
		{
			s_ast_node_list += ", " + ast_node->to_string();
		}

		this->ast_node_list.push_front(std::move(front));
	}

	return this->id + "(" + s_ast_node_list + ")";
}

std::string IfNode::to_string()
{
	std::string s_block_true = "";
	for (auto &statement : this->block_true)
	{
		if (statement != nullptr)
		{
			s_block_true += "\n" + statement->to_string();
		}
	}

	std::string s_block_false = "";
	if (!block_false.empty())
	{
		s_block_false = "\nelse";
		for (auto &statement : this->block_false)
		{
			if (statement != nullptr)
			{
				s_block_false += "\n" + statement->to_string();
			}
		}
	}
	return "if " + expr->to_string() + " then" + s_block_true + s_block_false;
}

std::string WhileNode::to_string()
{
	std::string s_block = "";
	for (auto &statement : this->block)
	{
		if (statement != nullptr)
		{
			s_block += "\n" + statement->to_string();
		}
	}

	return "while " + this->expr->to_string() + " do" + "\nbegin" + s_block + "\nend";
}

std::string RepeatNode::to_string()
{
	std::string s_block = "";
	for (auto &statement : this->block)
	{
		if (statement != nullptr)
		{
			s_block += "\n" + statement->to_string();
		}
	}

	return "repeat" + s_block + "\nuntil " + this->expr->to_string();
}

std::string ForNode::to_string()
{
	std::string s_block = "";
	for (auto &statement : this->block)
	{
		if (statement != nullptr)
		{
			s_block += "\n" + statement->to_string();
		}
	}

	return "for " +
		   this->assign->to_string() +
		   " to " +
		   this->expr->to_string() +
		   " do\nbegin" +
		   s_block +
		   "\nend";
}

std::string BranchingStatementNode::to_string()
{
	if (this->branching_statement == BranchingStatement::Continue)
	{
		return "continue";
	}
	else if (this->branching_statement == BranchingStatement::Break)
	{
		return "break";
	}
	return "";
}

std::string NotExprNode::to_string()
{
	return "not " + this->expr1->to_string();
}

std::string UnaryExprNode::to_string()
{
	return "-" + this->expr1->to_string();
}

std::string BinaryExprNode::to_string()
{
	std::string string1 = this->expr1->to_string();
	std::string string2 = this->expr2->to_string();
	ASTNode *b_expr1 = static_cast<ASTNode *>(this->expr1.get());
	ASTNode *b_expr2 = static_cast<ASTNode *>(this->expr2.get());

	if (this->get_precedence() > b_expr1->get_precedence())
	{
		string1 = "( " + string1 + " )";
	}
	if (this->get_precedence() > b_expr2->get_precedence())
	{
		string2 = "( " + string2 + " )";
	}

	return string1 + " " + this->get_oper() + " " + string2;
}

std::string NumberNode::to_string()
{
	return std::to_string(val);
}

std::string StringNode::to_string()
{
	return "'" + val + "'";
}

std::string BooleanNode::to_string()
{
	return val;
}

std::string IdNode::to_string()
{
	return val;
}

int MulExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;
	//could be statement
	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 * value2;
}

int DivExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 / value2;
}

int ModExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 % value2;
}

int AndExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 & value2;
}

int ShlExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 << value2;
}

int ShrExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 >> value2;
}

int LSExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 << value2;
}

int RSExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 >> value2;
}

int AddExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 + value2;
}

int SubExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 - value2;
}

int OrExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 | value2;
}

int XorExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 ^ value2;
}

int EqExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 == value2;
}

int NEqExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 != value2;
}

int LTExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 < value2;
}

int GTExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					 std::map<std::string, std::map<int, int>> &variables_value,
					 std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 > value2;
}

int LToEExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					   std::map<std::string, std::map<int, int>> &variables_value,
					   std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 <= value2;
}

int GToEExprNode::eval(std::map<std::string, ReturnType> &variables_type,
					   std::map<std::string, std::map<int, int>> &variables_value,
					   std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int value1 = 0;
	int value2 = 0;

	value1 = ((ExprNode *)this->expr1.get())->eval(variables_type, variables_value, functions);
	value2 = ((ExprNode *)this->expr2.get())->eval(variables_type, variables_value, functions);

	return value1 >= value2;
}

void ProgramNode::exec(std::map<std::string, ReturnType> &variables_type,
					   std::map<std::string, std::map<int, int>> &variables_value,
					   std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	for (auto &variable_decl : this->variable_section)
	{
		if (variable_decl != nullptr)
		{
			variable_decl->exec(variables_type, variables_value, functions);
		}
	}

	for (auto &subprogram : this->subprogram_decl)
	{
		if (subprogram != nullptr)
		{
			std::string s_id = subprogram->id;
			functions[s_id] = std::move(subprogram);
		}
	}

	for (auto &statement : this->statement_list)
	{
		if (statement != nullptr)
		{
			statement->exec(variables_type, variables_value, functions);
		}
	}
}

void VariableDeclNode::exec(std::map<std::string, ReturnType> &variables_type,
							std::map<std::string, std::map<int, int>> &variables_value,
							std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	for (auto id : this->id_list)
	{
		variables_type[id] = this->type;
		if (this->index1 != -1 && this->index2 != -1)
		{
			variables_value[id][-1] = this->index1;
			variables_value[id][-2] = this->index1;
		}
	}
}

void SubprogramDeclNode::exec(std::map<std::string, ReturnType> &variables_type,
							  std::map<std::string, std::map<int, int>> &variables_value,
							  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	std::cout << "TO DO" << std::endl;
}

void AssignNode::exec(std::map<std::string, ReturnType> &variables_type,
					  std::map<std::string, std::map<int, int>> &variables_value,
					  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	int i_index = 0;
	if (this->index != nullptr && this->index->get_kind() == NodeKind::ExprNode)
	{
		i_index = ((ExprNode *)this->index.get())->eval(variables_type, variables_value, functions);
	}

	int value = 0;
	if (this->expr->get_kind() == NodeKind::ExprNode)
	{
		value = ((ExprNode *)this->expr.get())->eval(variables_type, variables_value, functions);
	}
	else if (this->expr->get_kind() == NodeKind::IdNode)
	{
		auto id_node = (IdNode *)this->expr.get();
		int id_node_index = 0;

		if (id_node->index != nullptr && id_node->index->get_kind() == NodeKind::ExprNode)
		{
			id_node_index = ((ExprNode *)id_node->index.get())->eval(variables_type, variables_value, functions);
		}

		value = variables_value[id_node->val][id_node_index];
	}

	variables_value[this->id][i_index] = value;
}

void SubprogramCallNode::exec(std::map<std::string, ReturnType> &variables_type,
							  std::map<std::string, std::map<int, int>> &variables_value,
							  std::map<std::string, UP_SubprogramDeclNode> &functions)
{
	if (this->id.compare("write") == 0)
	{
		for (auto &node : this->ast_node_list)
		{
			switch (node->get_kind())
			{
			case NodeKind::StringNode:
				std::cout << ((StringNode *)node.get())->val;
				break;
			case NodeKind::IdNode:
			{
				auto id_node = ((IdNode *)node.get());
				int index = id_node->index != nullptr ? 0 : 0;
				std::cout << variables_value[id_node->val][index];
			}
			break;
			case NodeKind::ExprNode:
				std::cout << ((ExprNode *)node.get())->eval(variables_type, variables_value, functions);
				break;
			}
		}
	}
	else if (this->id.compare("writeln") == 0)
	{
		for (auto &node : this->ast_node_list)
		{
			switch (node->get_kind())
			{
			case NodeKind::StringNode:
				std::cout << ((StringNode *)node.get())->val;
				break;
			case NodeKind::IdNode:
			{
				auto id_node = ((IdNode *)node.get());
				int index = id_node->index != nullptr ? 0 : 0;
				std::cout << variables_value[id_node->val][index];
			}
			break;
			case NodeKind::ExprNode:
				std::cout << ((ExprNode *)node.get())->eval(variables_type, variables_value, functions);
				break;
			case NodeKind::SubprogramCallNode:
				((SubprogramCallNode *)node.get())->exec(variables_type, variables_value, functions);
				break;
			}
		}
		std::cout << std::endl;
	}
	else if (this->id.compare("read") == 0)
	{
		for (auto &node : this->ast_node_list)
		{
			auto id_node = ((IdNode *)node.get());
			std::cin >> variables_value[id_node->val][0];
		}
	}
	else if (this->id.compare("IntToHex") == 0)
	{
		// Shouldn't be implemented therefore it does not return a value just prints it out
		auto front = std::move(this->ast_node_list.front());
		this->ast_node_list.pop_front();

		int val = ((ExprNode *)this->ast_node_list.front().get())->eval(variables_type, variables_value, functions);

		std::cout << std::uppercase
				  << std::setfill('0')
				  << std::setw(val)
				  << std::hex
				  << ((ExprNode *)front.get())->eval(variables_type, variables_value, functions);

		this->ast_node_list.push_front(std::move(front));
	}
	else if (functions.find(this->id) != functions.end())
	{
		functions[this->id]->exec(variables_type, variables_value, functions);
	}
}
