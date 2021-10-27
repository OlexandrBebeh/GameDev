#include "MonteCarloStrategy.hpp"
#include "MonteCarloNode.hpp"
#include <algorithm>
#include <random>
#include "../../../View/Output.hpp"

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
	return node->win/node->games + pow(2 * (std::log(node->parent->games) / node->games), 0.5);
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

int model::MonteCarloStrategy::Simulate()
{
	int counter = 0;
	auto v = view::Output();

	while (true)
	{
		auto move = GetRandomMove();

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				std::cout << m_game.GetField()[i][j] << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << m_game.GetVerticalPatrtitions()[i][j] << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				std::cout << m_game.GetHorizontalPatrtitions()[i][j] << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << m_game.GetCrosstPatritions()[i][j] << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		m_game.MakeMove(move);
		int winner = m_game.CheckWin();
		counter++;
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
	if (rand() % 101 < 60)
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

	MonteCarloNode root;
	int counter = 0;
	m_target_player = game->GetCurrentPlayerId();

	while (counter < 100)
	{
		m_game = game;
		auto node = GetNextToExplore(&root);
		UpdateTree(node, Simulate());
		counter++;
	}
	auto move = *FindBest(&root)->move;
	return move;
}