#include "Input.hpp"
#include <string>

using namespace view;

std::string Input::GetString()
{
	std::string str;

	std::getline(std::cin, str);


	return str;
}


