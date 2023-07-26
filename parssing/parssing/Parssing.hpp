#ifndef PARSSING_H__
# define PARSSING_H__

# define MAX_LINE 1000000000

# define C_BLUE "\e[38;2;20;200;235m"
# define C_PURPLE "\e[38;2;174;21;235m"
# define C_RED "\e[38;2;235;43;21m"
# define C_WHITE "\e[38;2;235;255;255m"
# include <iostream>
# include <fstream>
# include <istream>
# include <sstream>
# include <string>
# include <vector>
# include <ctime>

# include "../../shared/shared.hpp"

int parssing_part(int argc, char **argv, std::vector<s_server> &servers_list);

#endif