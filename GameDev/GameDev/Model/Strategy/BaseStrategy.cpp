#include "BaseStrategy.hpp"

using namespace model;

Move BaseStrategy::GetMove(Field* field, Position pos, int target)
{
	Move a;
	a.first = rand() % 3 + 1;
	if (a.first == 1)
	{
		auto moves = field->GetPossibleFigureMoves(pos);
		a.second = moves[rand() % moves.size()];
	}
	else if (a.first == 2)
	{
		a.second = field->m_possible_vertical_partitions[rand() % field->m_possible_vertical_partitions.size()];
	}
	else
	{
		a.second = field->m_possible_horizontal_partitions[rand() % field->m_possible_horizontal_partitions.size()];

	}

	return a;
}
