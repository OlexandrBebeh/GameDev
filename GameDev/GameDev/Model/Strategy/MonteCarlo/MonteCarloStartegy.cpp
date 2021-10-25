#include "MonteCarloStrategy.hpp"
#include "MonteCarloNode.hpp"


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

		for (auto move : moves)
		{
			node->childs.push_back(new MonteCarloNode(node, &move));
		}
	}

	auto unexplored = std::find_if(node->childs.begin(), node->childs.end(),
		[](MonteCarloNode *c) { return c->games == 0; });

	if (unexplored == node->childs.end())
	{
		node->all_visited = true;

	}
	return *unexplored;
}

Move MonteCarloStrategy::GetMove(Game* game, int target)
{
	auto start = std::chrono::system_clock::now();

	MonteCarloNode root;
	int counter = 0;


	while (CheckTime(start))
	{
		m_game = game;
		m_game.MakeMove({ 1,{7,4} });
		auto node = GetNextToExplore(&root);
		counter++;
	}
	return Move();
}