#include "MonteCarloStrategy.hpp"
#include "MonteCarloNode.hpp"
#include <algorithm>
#include <random>

using namespace model;

bool model::MonteCarloStrategy::CheckTime(std::chrono::time_point<std::chrono::system_clock> start)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() < ALGORYTM_TIME;
}

MonteCarloNode* model::MonteCarloStrategy::GetNextToExplore(MonteCarloNode* node)
{
	if (node->childs.empty())
	{
		auto moves = m_game.GetPossibleMoves();

		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(moves), std::end(moves), rng);

		for (auto move : moves)
		{
			node->childs.push_back(new MonteCarloNode(node, &move));
		}
	}

	if (node->has_unvisited)
	{
		auto unexplored = std::find_if(node->childs.begin(), node->childs.end(),
			[](MonteCarloNode* c) { return c->games == 0; });

		if (unexplored == node->childs.end())
		{
			node->has_unvisited = false;

			return GetNextToExplore(FindBest(node));
		}

		return *unexplored;
	}
	else
	{
		return GetNextToExplore(FindBest(node));
	}
}

double model::MonteCarloStrategy::CalculateUCB(MonteCarloNode* node)
{
	return node->win/node->games + pow(2 * (std::log(node->parent->games) / node->games), 0.5);
}

MonteCarloNode* model::MonteCarloStrategy::FindBest(MonteCarloNode* node)
{
	double best = 0;
	MonteCarloNode* best_node = nullptr;
	for (auto child : node->childs)
	{
		auto t = CalculateUCB(child);
		if (t > best)
		{
			best = t;
			best_node = child;
		}
	}

	return best_node;
}

Move MonteCarloStrategy::GetMove(Game* game, int target)
{
	auto start = std::chrono::system_clock::now();

	MonteCarloNode root;
	int counter = 0;


	while (CheckTime(start))
	{
		m_game = game;
		auto node = GetNextToExplore(&root);
		counter++;
	}
	return Move();
}