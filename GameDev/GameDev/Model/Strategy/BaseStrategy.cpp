#include "BaseStrategy.hpp"
#include "../Game/Game.hpp"

using namespace model;

Move BaseStrategy::GetMove(Game* game, int target)
{
	Move a;
	a.first = rand() % 3 + 1;
	if (a.first == 1)
	{
		auto moves = game->GetPossibleFigureMoves(game->GetCurrentPlayerId());
		a.second = moves[rand() % moves.size()];
	}
	else if (a.first == 2)
	{
		a.second = game->GetPossibleVerticalPatrtitions()[rand() % game->GetPossibleVerticalPatrtitions().size()];
	}
	else
	{
		a.second = game->GetPossibleHorizontalPatrtitions()[rand() % game->GetPossibleHorizontalPatrtitions().size()];

	}

	return a;
}
