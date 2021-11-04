#include "AIPlayer.hpp"
#include "../../Game/Game.hpp"

using namespace model;

AIPlayer::AIPlayer() : Player()
{
	m_player_type = PlayerType::ai_player;
};

AIPlayer::~AIPlayer()
{
	delete strategy;
}

bool AIPlayer::HasAILogic() const
{
	return true;
}

Move AIPlayer::GetMove(Game* game)
{
	return strategy->GetMove(game, m_player_win_row);
}
