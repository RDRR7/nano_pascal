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
		else
		{
			s_statement_list += "\nstatement";
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

	switch (this->return_type)
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

	std::string s_argument_decl_list = "";
	if (!this->argument_decl_list.empty())
	{
		s_argument_decl_list = this->argument_decl_list.front()->to_string();
		this->argument_decl_list.pop_front();
		for (auto &argument_decl : this->argument_decl_list)
		{
			s_argument_decl_list += "; " + argument_decl->to_string();
		}
	}

	std::string s_statement_list = "";
	for (auto &statement : this->statement_list)
	{
		if (statement != nullptr)
			s_statement_list += "\n" + statement->to_string();
	}

	return s_subprogram_type + this->id + "(" + s_argument_decl_list + "): " + s_type + "\nbegin" +
		   s_statement_list +
		   "\nend;";
}

std::string VariableDeclNode::to_string()
{
	std::string s_id_list = this->id_list.front();
	this->id_list.pop_front();
	for (auto id : this->id_list)
	{
		s_id_list += ", " + id;
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
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Integer;";
			break;
		case ReturnType::Boolean:
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Boolean;";
			break;
		case ReturnType::Char:
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Char;";
			break;
		}
	}

	return s_id_list +
		   ": " +
		   s_type;
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
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Integer";
			break;
		case ReturnType::Boolean:
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Boolean";
			break;
		case ReturnType::Char:
			s_type = "Array [" + std::to_string(index1) + ".." + std::to_string(index2) + "] of Char";
			break;
		}
	}

	return id + ": " + s_type;
}

std::string AssignNode::to_string()
{
	return "assign";
}

std::string SubprogramCallNode::to_string()
{
	return this->id + "();";
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
		else
		{
			s_block_true += "\nstatement";
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
			else
			{
				s_block_false += "\nstatement";
			}
		}
	}
	return "if expr then" + s_block_true + s_block_false;
}

std::string WhileNode::to_string()
{
	return "while";
}

std::string RepeatNode::to_string()
{
	return "repeat";
}

std::string ForNode::to_string()
{
	return "for";
}

std::string BranchingStatementNode::to_string()
{
	return "branching";
}

std::string NotExprNode::to_string()
{
	return "not";
}

std::string UnaryExprNode::to_string()
{
	return "-";
}

std::string BinaryExprNode::to_string()
{
	return "binary";
}

std::string ParExprNode::to_string()
{
	return "parexpr";
}

std::string NumberNode::to_string()
{
	return "number";
}

std::string StringNode::to_string()
{
	return "string";
}
