#pragma once

#include "../Player.hpp"
#include "../../Strategy/BaseStrategy.hpp"

namespace model
{
	class AIPlayer : public Player
	{
	public:
		AIPlayer();
		~AIPlayer();
		bool HasAILogic() const override;
		Move GetMove() override;
	private:
		BaseStrategy strategy = BaseStrategy();

	};
}