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
	Test();
}

model::Game::Game(Game* game)
{
	this->m_field = std::make_shared<Field>(*game->m_field.get());

	for (auto player : game->m_players)
	{
		this->m_players.insert(player);
	}
	this->m_current_player = game->m_current_player;
	this->m_players_amount = game->m_players_amount;
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

		std::shared_ptr<Player> unique(player);
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

std::vector<Position> model::Game::GetPossibleVerticalPatrtitions() const
{
	return m_field->m_possible_vertical_partitions;
}

std::vector<Position> model::Game::GetPossibleHorizontalPatrtitions() const
{
	return m_field->m_possible_horizontal_partitions;
}


void Game::MakeMove(model::Move move)
{
	if (move.first == 1)
	{
		MakeFigureMove(move.second);
	}
	else if (move.first == 2)
	{
		
		auto it = std::find(m_field->m_possible_vertical_partitions.begin(),
			m_field->m_possible_vertical_partitions.end(),
							move.second);

		if (it != m_field->m_possible_vertical_partitions.end())
		{
			m_field->SetVerticalPartition(move.second);
			m_field->m_history.emplace_back(m_current_player, Move(2, move.second));
		}

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else if (move.first == 3)
	{

		auto it = std::find(m_field->m_possible_horizontal_partitions.begin(),
			m_field->m_possible_horizontal_partitions.end(),
			move.second);

		if (it != m_field->m_possible_horizontal_partitions.end())
		{
			m_field->SetHorizontalPartition(move.second);
			m_field->m_history.emplace_back(m_current_player, Move(3, move.second));
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

std::vector<Move> model::Game::GetPossibleMoves()
{
	std::vector<Move> moves;

	for (auto pos : m_field->GetPossibleFigureMoves(GetCurrentPlayer()->GetPosition()))
	{
		moves.emplace_back(1, pos);
	}
	for (auto pos : m_field->m_possible_vertical_partitions)
	{
		moves.emplace_back(2, pos);
	}
	for (auto pos : m_field->m_possible_horizontal_partitions)
	{
		moves.emplace_back(3, pos);
	}

	return moves;
}

std::vector<Move> model::Game::GetPossibleMoves(int player)
{
	std::vector<Move> moves;

	for (auto pos : m_field->GetPossibleFigureMoves(m_players[player]->GetPosition()))
	{
		moves.emplace_back(1, pos);
	}
	if (m_players[player]->GetPartitionsAmount())
	{
		for (auto pos : m_field->m_possible_vertical_partitions)
		{
			moves.emplace_back(2, pos);
		}
		for (auto pos : m_field->m_possible_horizontal_partitions)
		{
			moves.emplace_back(3, pos);
		}
	}

	return moves;
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

void model::Game::RevertMove()
{

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

int model::Game::CheckWin()
{
	int winner = -1;
	for (auto& player : m_players)
	{
		if (player.second->GetPosition().GetVertical() 
			== player.second->GetPlayerWinRow())
		{
			winner = player.first;
			break;
		}
	}
	return winner;
}

void model::Game::ResetGame()
{
	m_field->GameReset();

	m_players.clear();

	m_players_amount = 0;
	m_current_player = 0;
}
