#pragma once

#include "../Game/Field.hpp"

namespace model
{
	class Game;

	class BaseStrategy
	{
	public:
		BaseStrategy() {};
		~BaseStrategy() {};
		virtual Move GetMove(Game* game, int target);
	};
}