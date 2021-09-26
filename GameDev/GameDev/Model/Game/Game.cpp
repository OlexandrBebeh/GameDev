#include "Game.hpp"

using namespace model;

Game::Game()
{
	m_players_amount = 0;
}

Game::~Game()
{
}

void Game::AddPlayer(Player player)
{
	m_players[m_players_amount] = std::make_shared<Player>(player);
	m_players_amount++;
}

std::shared_ptr<Player> Game::GetNextPlayer() const
{
	auto ptr = m_players.find(m_current_player);

	if (ptr != m_players.end())
	{
		return ptr->second;
	}
}

std::vector<std::vector<int>> Game::GetField() const
{
	return m_field;
}

std::vector<std::vector<int>> Game::GetVerticalPatrtitions() const
{
	return m_vertical_partitions;
}
std::vector<std::vector<int>> Game::GetHorizontalPatrtitions() const
{
	return m_horizontal_partitions;
}