#include "general.hpp"

/* Общие функции для удобной работы */
/* P.S. Драконы */

void	message(std::string line)
{
	std::cout << line << std::endl;
}

void	warning(std::string line)
{
	std::cout << "WARNING: " << line << std::endl;
}

void	fatal(std::string line)
{
	std::cout << "FATAL ERROR: " << line << std::endl;
	std::exit(EXIT_FAILURE);
}
