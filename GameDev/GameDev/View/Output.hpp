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
		void ShowPositions(model::Game*);
		void ShowPossibleMoves(model::Game*);
		void ShowPartitions(model::Game*);
		void StartMenu();
		void ShowHelp();

	};
}