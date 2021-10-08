#pragma once

#include "../Game/Field.hpp"

namespace model
{
	class BaseStrategy
	{
	public:
		BaseStrategy() {};
		~BaseStrategy() {};
		Move GetMove(Field*, Position pos, int target);
	};
}