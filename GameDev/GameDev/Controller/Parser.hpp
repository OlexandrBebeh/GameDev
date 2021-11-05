#pragma once
#include <string>
#include "../Model/DataTypes/Position.hpp"
#include <utility>
#include <map>

namespace controller
{
	class Parser
	{
		using Move = std::pair<int, model::Position>;
	public:
		Parser() 
		{
			char a = 'A';
			for (int i = 0; i < 9; i++, a++)
			{
				move_digits[a] = i;
			}
			a = 'S';
			for (int i = 0; i < 8; i++, a++)
			{
				wall_digits[a] = i;
			}
		};
		~Parser() {};

		Move ParseMove(std::string str);
		int GetFlag(std::string str);
		Move ParseBotMove(std::string str);
		std::string ToBotMove(Move move, model::Position pos);

		std::map<char, int> move_digits;
		std::map<char, int> wall_digits;

	};
}