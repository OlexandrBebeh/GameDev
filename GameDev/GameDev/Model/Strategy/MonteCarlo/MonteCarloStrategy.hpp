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

		int m_target_player;

		const double ALGORYTM_TIME = 2000.0;

		bool CheckTime(std::chrono::time_point<std::chrono::system_clock> start);

		MonteCarloNode* GetNextToExplore(MonteCarloNode* node);

		double CalculateUCB(MonteCarloNode* node);

		MonteCarloNode* FindBest(MonteCarloNode* node);

		int Simulate();

		void UpdateTree(MonteCarloNode* , int);

		Move GetRandomMove();

	public:
		Move GetMove(Game* game, int target);


	};
}
