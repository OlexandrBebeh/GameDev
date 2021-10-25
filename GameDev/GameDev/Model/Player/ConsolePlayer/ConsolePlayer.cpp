#include "ConsolePlayer.hpp"

using namespace model;

ConsolePlayer::ConsolePlayer() : Player()
{
	m_player_type = PlayerType::console_player;
};

ConsolePlayer::~ConsolePlayer()
{
}

bool ConsolePlayer::HasConsoleInput() const
{
	return true;
}
