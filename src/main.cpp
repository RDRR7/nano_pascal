#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "nanopascal_lexer.h"
#include "nanopascal_parser.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage " << argv[0] << " <input file> [-d<directive>]..." << std::endl;
		return 1;
	}

	std::ifstream in(argv[1], std::ios::in);

	if (!in.is_open())
	{
		std::cerr << "Cannot open input file '" << argv[1] << "'" << std::endl;
		return 1;
	}

	std::list<std::string> directives;
	for (int i = 2; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == 'd' && argv[i][2] != '\0')
		{
			directives.push_back(&argv[i][2]);
		}
		else
		{
			std::cerr << "Usage " << argv[0] << " " << argv[1] << " [-d<directive>]..." << std::endl;
			return 1;
		}
	}

	NanoPascalLexer lexer(in, directives);
	NanoPascalParser parser(lexer);
	parser.parse();

	return 0;
}
