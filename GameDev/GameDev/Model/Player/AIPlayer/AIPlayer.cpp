#include "AIPlayer.hpp"

using namespace model;

AIPlayer::AIPlayer() : Player()
{
	m_player_type = PlayerType::ai_player;
};

AIPlayer::~AIPlayer()
{
}

bool AIPlayer::HasAILogic() const
{
	return true;
}

Move AIPlayer::GetMove()
{
	return strategy.GetMove(m_field.get(),m_pos,m_player_win_row);
}
