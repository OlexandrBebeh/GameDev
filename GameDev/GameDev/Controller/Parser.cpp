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

int controller::Parser::GetFlag(std::string str)
{
	if (str == "white")
	{
		return 1;
	}
	if (str == "black")
	{
		return 2;
	}
	return 3;
}

model::Move controller::Parser::ParseBotMove(std::string str)
{
	std::string delimiter = " ";
	str += " ";
	size_t pos = 0;
	std::string token;
	std::vector<std::string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos) 
	{
		token = str.substr(0, pos);
		tokens.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	Move a;
	if (tokens[0] == "move")
	{
		a.first = 1;
		a.second = { tokens[1][1] - '1',move_digits[tokens[1][0]] };
	}
	else if (tokens[0] == "wall")
	{
		if (tokens[1][2] == 'v')
			a.first = 2;
		else if (tokens[1][2] == 'h')
			a.first = 3;

		a.second = { tokens[1][1] - '1',wall_digits[tokens[1][0]] };
	}

	return a;
}

std::string controller::Parser::ToBotMove(Move move)
{
	std::string str;
	if (move.first == 1)
	{
		char a = 'A';
		str += "move ";
		str += a + move.second.GetHorizontal();
		str += '1' + (move.second.GetVertical());
	}
	else
	{
		str += "wall ";
		char a = 'S';
		str += a + move.second.GetHorizontal();
		str += '1' + move.second.GetVertical();
		if (move.first == 2)
		{
			str += 'v';
		}
		else
		{
			str += 'h';

		}
	}
	return str;
}
