#pragma once

#include "../Player.hpp"
#include "../../Strategy/MonteCarlo/MonteCarloStrategy.hpp"

namespace model
{
	class AIPlayer : public Player
	{
	public:
		AIPlayer();
		~AIPlayer();
		bool HasAILogic() const override;
		Move GetMove(Game*) override;
	private:
		BaseStrategy *strategy = new MonteCarloStrategy();

	};
}