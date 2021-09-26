#include "Input.hpp"

using namespace view;

std::string Input::GetString()
{
	std::string str;

	std::cin >> str;

	return str;
}

