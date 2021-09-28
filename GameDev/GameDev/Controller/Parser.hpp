#pragma once
#include <string>
#include "../Model/DataTypes/Position.hpp"

namespace controller
{
	class Parser
	{
		using Move = std::pair<int, model::Position>;
	public:
		Parser() {};
		~Parser() {};

		Move ParseMove(std::string str);
	};
}