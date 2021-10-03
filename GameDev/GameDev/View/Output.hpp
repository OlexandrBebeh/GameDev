#pragma once

#include <iostream>
#include "../Model/Game/Game.hpp"

namespace view
{
	class Output
	{
	public:
		Output() {};
		~Output() {};

		void ShowMessage(std::string);
		void ShowGameState(model::Game*);

		void StartMenu();
	};
}