#pragma once
#include "../BaseStrategy.hpp"
#include "../../Game/Game.hpp"
#include "./MonteCarloNode.hpp"
#include <ctime> 
#include <chrono>

namespace model
{
	class MonteCarloStrategy : public BaseStrategy
	{
		Game m_game;

		const double ALGORYTM_TIME = 2000.0;

		bool CheckTime(std::chrono::time_point<std::chrono::system_clock> start);

		MonteCarloNode* GetNextToExplore(MonteCarloNode* node);

		double CalculateUCB(MonteCarloNode* node);

		MonteCarloNode* FindBest(MonteCarloNode* node);

	public:
		Move GetMove(Game* game, int target);


	};
}
