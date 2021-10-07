#include "Game.hpp"
#include "../../View/Output.hpp"
#include <set>

using namespace model;

Game::Game()
{
	m_players_amount = 0;
	m_player_start_pos.push_back({ 0,4 });
	m_player_start_pos.push_back({ 8,4 });
	m_field = std::make_shared<Field>();
	m_field->GameReset();
	Test();
	
}

Game::~Game()
{
}

void Game::AddPlayer(Player* player)
{
	if (m_players_amount < m_player_start_pos.size())
	{
		auto pos = m_player_start_pos[m_players_amount];
		player->SetPlayerWinRow(abs(pos.GetVertical() - PARTITION_SIZE));
		player->SetStartPosition(pos);
		player->SetField(m_field);

		m_field->m_field[pos.GetVertical()][pos.GetHorizontal()] = m_players_amount;

		std::unique_ptr<Player> unique(player);
		m_players[m_players_amount] = std::move(unique);
		m_players_amount++;
	}
}

int model::Game::GetCurrentPlayerId() const
{
	return m_current_player;
}

Player* Game::GetCurrentPlayer() const
{
	auto ptr = m_players.find(m_current_player);

	if (ptr != m_players.end())
	{
		return ptr->second.get();
	}
}

std::vector<std::vector<int>> Game::GetField() const
{
	return m_field->m_field;
}

std::vector<std::vector<int>> Game::GetVerticalPatrtitions() const
{
	return m_field->m_vertical_partitions;
}

std::vector<std::vector<int>> Game::GetHorizontalPatrtitions() const
{
	return m_field->m_horizontal_partitions;
}
std::vector<std::vector<int>> Game::GetCrosstPatritions() const
{
	return m_field->m_crosst_partitions;
}


void Game::MakeMove(model::Move move)
{
	if (move.first == 1)
	{
		MakeFigureMove(move.second);
	}
	else if (move.first == 2 || move.first == 3)
	{
		if (GetCurrentPlayer()->GetPartitionsAmount() > 0)
		{
			throw std::exception{ "Wrong Move" };
		}
		if (move.first == 2)
		{
			auto it = std::find(m_field->m_possible_vertical_partitions.begin(),
				m_field->m_possible_vertical_partitions.end(),
								move.second);

			if (it != m_field->m_possible_vertical_partitions.end())
			{
				m_field->SetVerticalPartition(move.second);
				m_field->m_history.emplace_back(m_current_player, Move(2, move.second));
			}
		}
		else
		{

			auto it = std::find(m_field->m_possible_horizontal_partitions.begin(),
				m_field->m_possible_horizontal_partitions.end(),
								move.second);

			if (it != m_field->m_possible_horizontal_partitions.end())
			{
				m_field->SetHorizontalPartition(move.second);
				m_field->m_history.emplace_back(m_current_player, Move(3, move.second));
			}
				
		}

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else
	{
		throw std::exception{ "Game::MakeMove - wrong move" };
	}
	m_field->m_possible_horizontal_partitions = CheckPossibleHorizontalPartitions();
	m_field->m_possible_vertical_partitions = CheckPossibleVerrticalPartitions();

	NextPlayer();
}

bool Game::IsGameEnd()
{
	return false;
}

void Game::MakeFigureMove(Position position)
{
	auto posible_ways = GetPossibleFigureMoves(m_current_player);

	auto it = std::find(posible_ways.begin(),posible_ways.end(), position);

	if (it != posible_ways.end())
	{
		m_field->m_field[m_players[m_current_player]->GetPosition().GetVertical()][m_players[m_current_player]->GetPosition().GetHorizontal()] = -1;
		m_field->m_field[position.GetVertical()][position.GetHorizontal()] = m_current_player;
		GetCurrentPlayer()->SetPosition(position);
	}
	else
	{
		throw std::exception{"Wrong Move"};
	}

	m_field->m_history.emplace_back(m_current_player, Move(1, position));
}
 

std::vector<Position> Game::CheckPossibleVerrticalPartitions()
{
	std::vector<Position> positions;

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			if (m_field->m_vertical_partitions[i][j] == 0 
				&& m_field->m_vertical_partitions[i + 1][j] == 0)
			{
				if (m_field->m_crosst_partitions[i][j] == 0)
				{
					bool is_valid = true;
					for (auto& player : m_players)
					{
						m_field->SetVerticalPartition({ i,j });
						std::vector<Position> positions;
						is_valid = m_field->Dijkstra(player.second->GetPosition(),
							positions,
							player.second->GetPlayerWinRow());
						if (!is_valid)
						{
							break;
						}
					}
					if (is_valid)
					{
						positions.push_back({ i,j });
					}
					m_field->DeleteVerticalPartition({ i,j });
				}
			}
		}
	}

	return  positions;
}

std::vector<Position> Game::CheckPossibleHorizontalPartitions()
{
	std::vector<Position> positions;

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			if (m_field->m_horizontal_partitions[i][j] == 0 
				&& m_field->m_horizontal_partitions[i][j + 1] == 0)
			{
				if (m_field->m_crosst_partitions[i][j] == 0)
				{
					bool is_valid = true;
					for (auto& player : m_players)
					{
						m_field->SetHorizontalPartition({ i,j });
						std::vector<Position> positions;
						is_valid = m_field->Dijkstra(player.second->GetPosition(), 
													 positions,
													 player.second->GetPlayerWinRow());
						if (!is_valid)
						{
							break;
						}
					}
					if (is_valid)
					{
						positions.push_back({ i,j });
					}
					m_field->DeleteHorizontalPartition({ i,j });
				}
			}
		}
	}

	return  positions;
}


std::vector<Position> Game::GetPossibleFigureMoves(int player_id)
{
	auto player_pos = m_players[player_id]->GetPosition();

	std::vector<Position> positions;

	for (auto dir : m_field->directions)
	{
		auto pos = m_field->GetMovesToDirect(player_pos, dir);
		positions.insert(positions.end(), pos.begin(), pos.end());
	}

	return  positions;
}

void Game::NextPlayer()
{
	if (m_current_player == m_players.size() - 1)
	{
		m_current_player = 0;
	}
	else
	{
		m_current_player++;
	}
}

void Game::Test()
{
	//m_field =
	//{
	//	{-1,-1,-1,-1,-1,-1,-1, 1, 2},
	//	{-1, 1,-1,-1,-1,-1,-1, 1,-1},
	//	{-1, 2, 1,-1,-1,-1,-1,-1,-1},
	//	{-1,-1,-1,-1,-1,-1,-1,-1,-1},
	//	{-1,-1,-1,-1,-1,-1,-1,-1,-1},
	//	{-1,-1,-1,-1,-1,-1,-1,-1,-1},
	//	{-1, 1,-1,-1,-1,-1,-1,-1,-1},
	//	{-1, 0,-1,-1,-1,-1,-1,-1,-1},
	//	{-1, 1,-1,-1,-1,-1,-1,-1,-1}
	//};

	//m_horizontal_partitions =
	//{
	//	{0,0,0,0,0,0,0,0,0},
	//	{0,1,1,0,0,0,0,0,0},
	//	{0,1,1,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0,0},
	//	{1,1,1,1,1,1,0,1,1},
	//	{0,0,0,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0,0},
	//};
	//m_vertical_partitions =
	//{
	//	{1,1,0,0,0,0,0,0},
	//	{1,1,0,0,0,0,0,0},
	//	{1,0,0,0,0,0,0,0},
	//	{1,0,0,0,0,0,0,0},
	//	{0,0,0,0,0,1,0,0},
	//	{0,0,0,0,1,1,0,0},
	//	{0,0,0,0,1,0,0,0},
	//	{0,0,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0},
	//};
	//m_crosst_partitions =
	//{
	//	{1,1,0,0,0,0,0,0},
	//	{0,-1,0,0,0,0,0,0},
	//	{1,-1,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0},
	//	{-1, 0,-1, 0,-1, 1, 0,-1},
	//	{0,0,0,0,1,0,0,0},
	//	{0,0,0,0,0,0,0,0},
	//	{0,0,0,0,0,0,0,0}
	//};

}
