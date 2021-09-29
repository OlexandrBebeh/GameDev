#pragma once
#include <string>
#include "../Model/DataTypes/Position.hpp"
#include <utility>

namespace controller
{
	class Parser
	{
		using Move = std::pair<int, model::Position>;
	public:
		Parser() {};
		~Parser() {};

		static Move ParseMove(std::string str);
	};
}