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

		const double ALGORYTM_TIME = 1900.0;

		bool CheckTime(std::chrono::time_point<std::chrono::system_clock> start);

		MonteCarloNode* GetNextToExplore(MonteCarloNode* node);


		double CalculateUCB(MonteCarloNode* node);

		MonteCarloNode* FindBest(MonteCarloNode* node);

		MonteCarloNode* BestMove(MonteCarloNode* node);

		int Simulate();

		void UpdateTree(MonteCarloNode* , int);

		Move GetRandomMove();

		int m_counter = 0;

	public:
		Move GetMove(Game* game, int target);

		MonteCarloStrategy() = default;
		~MonteCarloStrategy();
	};
}
