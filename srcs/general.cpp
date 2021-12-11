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
	cout << YELLOW"WARNING: " << line << DEFAULT"" << std::endl;
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

// Определяет подходит ли строка под маску
int		check_mask(string str, string mask_base) {
	bool	res = true, flag = false;
	string	mask = "";
	int		index = 0, len_str = str.size(), len_mask;

	if (mask_base.find('*') == std::string::npos)
		return (str == mask_base);
	for (int i = 0; mask_base[i]; i++) {
		if (mask_base[i] == '*' && i == 0)
			mask += mask_base[i];
		else if (mask_base[i] == '*' && mask_base[i - 1] != '*')
			mask += mask_base[i];
		else if (mask_base[i] != '*')
			mask += mask_base[i];
	}
	len_mask = mask.size();
	for (int i = 0; mask[i]; i++) {
		if (mask[i] == '*' && (i != len_mask - 1)) {
			flag = false;
			i++;
			while (index < len_str) {
				if (mask[i] == str[index]) {
					flag = true;
					index++;
					break ;
				}
				index++;
			}
			if (flag == false)
				return (false);
		}
		else if (mask[i] != '*') {
			if (mask[i] != str[index]) {
				return (false);
			}
			index++;
		}
	}
	return(res);
}
