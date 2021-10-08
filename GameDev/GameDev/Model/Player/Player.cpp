#include "Player.hpp"

using namespace model;

Player::Player()
{
}

Player::~Player()
{
}

void Player::SetPosition(Position p)
{
	m_pos = p;
}

void Player::SetPartitionsAmount(int p)
{
	m_partitions_amount = p;
}

Position Player::GetPosition() const
{
	return m_pos;
}

int Player::GetPartitionsAmount() const
{
	return m_partitions_amount;
}

PlayerType Player::GetPlayerType() const
{
	return m_player_type;
}

bool Player::HasConsoleInput() const
{
	return false;
}

bool Player::HasAILogic() const
{
	return false;
}

Move Player::GetMove()
{
	Move a;
	return a;
}

void Player::SetStartPosition(Position p)
{
	m_start_pos = p;
	m_pos = m_start_pos;
}

Position Player::GetStartPosition()
{
	return m_start_pos;
}