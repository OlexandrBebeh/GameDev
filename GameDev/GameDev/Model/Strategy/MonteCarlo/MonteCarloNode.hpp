#pragma once
#include "../../DataTypes/Position.hpp"
#include <vector>

namespace model
{
	class MonteCarloNode
	{
	public:
		int win{ 0 };
		double games{ 0 };
		int depth{ 0 };
		Move* move;

		MonteCarloNode* parent;

		bool has_unvisited{ true };

		std::vector<MonteCarloNode*> childs = std::vector<MonteCarloNode*>();

		MonteCarloNode(MonteCarloNode* parent, Move* move)
						: parent(parent), move(move) 
		{
			if (parent != nullptr)
			{
				depth = parent->depth + 1;
			}
		};

		MonteCarloNode()
			: parent(nullptr), move(nullptr) {};
	};
}