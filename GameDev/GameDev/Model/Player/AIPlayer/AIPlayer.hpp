#pragma once

#include "../Player.hpp"

namespace model
{
	class AIPlayer : public Player
	{
	public:
		AIPlayer();
		~AIPlayer();
		bool HasAILogic() const override;

	private:


	};
}