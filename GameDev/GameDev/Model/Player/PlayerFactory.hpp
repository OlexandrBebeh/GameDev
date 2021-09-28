#pragma once
#include "Player.hpp"
#include "ConsolePlayer/ConsolePlayer.hpp"
#include "AIPlayer/AIPlayer.hpp"
#include <stdexcept>

namespace model
{
	class PlayerFactory
	{
	public:
		PlayerFactory() {};
		~PlayerFactory() {};

		Player GetPlayer(int type)
		{
			switch (static_cast<PlayerType>(type))
			{
				case PlayerType::console_player:
					return new ConsolePlayer();
				case PlayerType::ai_player:
					return new AIPlayer();
				default:
					throw std::invalid_argument("received invalid type");
			}
		}
	};
}