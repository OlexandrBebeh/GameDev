#include "Parser.hpp"
#include <vector>

using namespace controller;

model::Move Parser::ParseMove(std::string str)
{
	Move a;
	std::string temp;
	std::vector<int> m_digits;
	for (int i = 0; i < str.size(); i++)
	{
		while (std::isdigit(str[i]) && i < str.size())
		{
			temp += str[i];
			i++;
		}
		if (!temp.empty())
		{
			m_digits.push_back(std::stoi(temp));
			temp.clear();
		}
	}

	if (m_digits.size() == 3)
	{
		a.first = m_digits[0];
		a.second = { m_digits[1],m_digits[2] };
	}
	else
	{
		throw std::invalid_argument("Parser::ParseMove::wrong amount of numbers");
	}

	return a;
}
