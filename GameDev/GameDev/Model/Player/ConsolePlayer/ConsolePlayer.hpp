#pragma once

#include "../Player.hpp"

namespace model
{
	class ConsolePlayer : public Player
	{
	public:
		ConsolePlayer();
		~ConsolePlayer();
		bool HasConsoleInput() const override;
	private:


	};
}