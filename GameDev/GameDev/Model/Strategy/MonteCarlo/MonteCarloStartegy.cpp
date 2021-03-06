#include "MonteCarloStrategy.hpp"
#include "MonteCarloNode.hpp"
#include <algorithm>
#include <random>
#include "../../../View/Output.hpp"


#define NUM_THREADS 8

using namespace model;

bool model::MonteCarloStrategy::CheckTime(std::chrono::time_point<std::chrono::system_clock> start)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() < ALGORYTM_TIME;
}

MonteCarloNode* model::MonteCarloStrategy::GetNextToExplore(MonteCarloNode* node)
{
	if (node->childs.empty())
	{
		auto moves = m_game.GetUsefullMoves(m_game.GetCurrentPlayerId());

		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(moves), std::end(moves), rng);

		for (auto move : moves)
		{
			node->childs.push_back(new MonteCarloNode(node,new Move(move)));
		}
	}

	if (node->has_unvisited)
	{
		auto unexplored = std::find_if(node->childs.begin(), node->childs.end(),
			[](MonteCarloNode* c) { return c->games == 0; });

		if (unexplored == node->childs.end())
		{
			node->has_unvisited = false;

			auto best_move = FindBest(node);
			m_game.MakeMove(*best_move->move);
			return GetNextToExplore(best_move);
		}

		m_game.MakeMove(*(*unexplored)->move);

		return *unexplored;
	}
	else
	{
		auto best_move = FindBest(node);
		m_game.MakeMove(*best_move->move);
		return GetNextToExplore(best_move);
	}
}

double model::MonteCarloStrategy::CalculateUCB(MonteCarloNode* node)
{
	double a = node->win / node->games;
	if (node->depth % 2 == 0)
	{
		a = 1 - a;
	}
	return a + 1.4142 * sqrt(std::log10(node->parent->games) / node->games);
}

MonteCarloNode* model::MonteCarloStrategy::FindBest(MonteCarloNode* node)
{
	double best = -1;
	MonteCarloNode* best_node = nullptr;
	for (auto child : node->childs)
	{
		if (child->games > 0)
		{
			auto t = CalculateUCB(child);
			if (t > best)
			{
				best = t;
				best_node = child;
			}
		}
	}

	return best_node;
}

MonteCarloNode* model::MonteCarloStrategy::BestMove(MonteCarloNode* node)
{
	double best = -1;
	MonteCarloNode* best_node = nullptr;
	for (auto child : node->childs)
	{
		if (child->games > 0)
		{
			auto t = child->win / child->games;
			if (t > best)
			{
				best = t;
				best_node = child;
			}
		}
	}
	return best_node;
}

int model::MonteCarloStrategy::Simulate()
{

	while (true)
	{
		auto move = GetRandomMove();
		m_game.MakeMove(move);

		int winner = m_game.CheckWin();
		if (winner != -1)
		{
			if (winner == m_target_player)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void model::MonteCarloStrategy::UpdateTree(MonteCarloNode* node, int win)
{
	auto temp = node;
	while (temp != nullptr)
	{
		temp->games++;

		temp->win += win;
		temp = temp->parent;
	}
}

Move model::MonteCarloStrategy::GetRandomMove()
{
	if (m_game.GetCurrentPlayer()->GetPartitionsAmount() == 0)
	{
		return m_game.GetShortesFigureMove();
	}
	if (rand() % 101 < 70)
	{
		return m_game.GetShortesFigureMove();
	}
	else
	{
		if (rand() % 2 == 0)
		{
			auto moves = m_game.GetPossibleVerticalPatrtitions();

			if (moves.empty())
			{
				return m_game.GetShortesFigureMove();
			}
			return Move(2,moves[rand() % moves.size()]);
		}
		else
		{
			auto moves = m_game.GetPossibleHorizontalPatrtitions();
			if (moves.empty())
			{
				return m_game.GetShortesFigureMove();
			}
			return Move(3, moves[rand() % moves.size()]);
		}
	}
}

Move MonteCarloStrategy::GetMove(Game* game, int target)
{
	auto start = std::chrono::system_clock::now();

	if (m_counter < 2)
	{
		m_counter++;

		return game->GetShortesFigureMove();
	}

	MonteCarloNode root;

	m_target_player = game->GetCurrentPlayerId();
	
	while (CheckTime(start))
	{
		m_game = game;
		auto node = GetNextToExplore(&root);
		UpdateTree(node, Simulate());
	}

	MonteCarloNode* best_node = BestMove(&root);

	if (game->GetCurrentPlayer()->GetPartitionsAmount() == 0)
	{
		return game->GetShortesFigureMove();
	}

	return *(best_node->move);
}

model::MonteCarloStrategy::~MonteCarloStrategy()
{
	delete &m_game;
}
