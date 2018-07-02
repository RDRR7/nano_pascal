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

void ProgramNode::exec()
{
	for (auto &statement : this->statement_list)
	{
		if (statement != nullptr)
		{
			statement->exec();
		}
	}
}
