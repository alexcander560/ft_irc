#include "general.hpp"

/* Общие функции для удобной работы */
/* P.S. Драконы */
/* P.P.S В debug, добавил if (DEBUG), потому что везде прописывать это условие невыносимо */

void	message(string line)
{
	cout << line << endl;
}

void	warning(string line)
{
	cout << YELLOW"WARNING: " << line << std::endl << DEFAULT;
}

void	fatal(string line)
{
	cout << RED"FATAL ERROR: " << line << endl << DEFAULT;
	exit(EXIT_FAILURE);
}

void	debug(string line)
{
	if (DEBUG)
		cout << "DEBUG: " << line << endl;
}