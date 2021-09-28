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

Player::Move Player::GetMove() const
{

}