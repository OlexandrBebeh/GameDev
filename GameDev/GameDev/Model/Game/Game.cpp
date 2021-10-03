#include "Game.hpp"
#include "../../View/Output.hpp"

using namespace model;

Game::Game()
{
	m_players_amount = 0;

	Test();

	GameReset();
}

Game::~Game()
{
}

void Game::GameReset()
{
	m_field.clear();

	m_field.resize(FIELD_SIZE);

	for (int col = 0; col < FIELD_SIZE; col++)
	{
		m_field[col].resize(FIELD_SIZE);
		for (int row = 0; row < FIELD_SIZE; row++)
		{
			m_field[col][row] = -1;
		}
	}

	m_horizontal_partitions.clear();

	m_horizontal_partitions.resize(PARTITION_SIZE);

	m_vertical_partitions.clear();

	m_vertical_partitions.resize(FIELD_SIZE);

	for (int col = 0; col < FIELD_SIZE; col++)
	{
		m_vertical_partitions[col].resize(PARTITION_SIZE);
	}

	for (int col = 0; col < FIELD_SIZE; col++)
	{
		m_horizontal_partitions[col].resize(FIELD_SIZE);
	}
}

void Game::AddPlayer(Player* player)
{
	std::unique_ptr<Player> unique(player);
	m_players[m_players_amount] = std::move(unique);
	m_players_amount++;
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

void Game::MakeMove(model::Move move)
{
	switch (move.first)
	{
	case 0:
		MakeFigureMove(move.second);
	case 1:
		SetVerticalPartition(move.second);
	case 2:
		SetHorizontalPartition(move.second);
	default:
		throw std::exception{ "Game::MakeMove - wrong move" };
	}

	m_history.emplace_back(m_current_player, move);
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
		m_field[position.GetHorizontal()][position.GetVertical()] = m_current_player;
		GetCurrentPlayer()->SetPosition(position);
	}
	else
	{
		throw std::exception{"Wrong Move"};
	}
}
 
std::vector<Position> Game::GetPossibleFigureMoves(int player_id)
{
	//auto player_pos = m_players[player_id]->GetPosition();
	Position player_pos{1, 7};

	std::vector<Position> positions;
	std::vector<Position> directions;
	directions.push_back({ 1,0 });
	directions.push_back({ -1,0 });
	directions.push_back({ 0,1 });
	directions.push_back({ 0,-1 });

	for (auto dir : directions)
	{
		auto pos = GetMovesToDirect(player_pos, dir);
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


void Game::SetVerticalPartition(Position position)
{
	if (GetCurrentPlayer()->GetPartitionsAmount() > 0)
	{
		throw std::exception{ "Wrong Move" };
	}

	auto posible_ways = GetPossibleVerrticalPartitions(m_current_player);

	auto it = std::find(posible_ways.begin(), posible_ways.end(), position);

	if (it != posible_ways.end())
	{
		m_vertical_partitions[position.GetHorizontal()][position.GetVertical()] = 1;
		m_horizontal_partitions[position.GetHorizontal()][position.GetVertical()] = -1;

		m_vertical_partitions[position.GetHorizontal()][position.GetVertical() + 1] = 1;

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else
	{
		throw std::exception{ "Wrong Move" };
	}
}

void Game::SetHorizontalPartition(Position position)
{
	if (GetCurrentPlayer()->GetPartitionsAmount() > 0)
	{
		throw std::exception{ "Wrong Move" };
	}

	auto posible_ways = GetPossibleHorizontalPartitions(m_current_player);

	auto it = std::find(posible_ways.begin(), posible_ways.end(), position);

	if (it != posible_ways.end())
	{
		m_horizontal_partitions[position.GetHorizontal()][position.GetVertical()] = 1;
		m_vertical_partitions[position.GetHorizontal()][position.GetVertical()] = -1;

		m_horizontal_partitions[position.GetHorizontal() + 1][position.GetVertical()] = 1;

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else
	{
		throw std::exception{ "Wrong Move" };
	}
}


std::vector<Position> Game::GetPossibleVerrticalPartitions(int player_id)
{
	std::vector<Position> positions;

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			if (m_vertical_partitions[i][j] == 0 && m_vertical_partitions[i + 1][j] == 0)
			{
				
				 positions.push_back({ i,j });
				
			}
		}
	}

	return  positions;
}

std::vector<Position> Game::GetPossibleHorizontalPartitions(int player_id)
{
	std::vector<Position> positions;

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			if (m_vertical_partitions[i][j] == 0 && m_vertical_partitions[i][j + 1] == 0)
			{
				positions.push_back({ i,j });
			}
		}
	}

	return  positions;
}

std::vector<Position> Game::GetMovesToDirect(Position start, Position dir = { 0,0 })
{
	std::vector<Position> positions;

	if (dir.GetHorizontal() != 0)
	{
		int hor_dir = start.GetHorizontal() + dir.GetHorizontal();
		if (hor_dir < 0 || hor_dir >= FIELD_SIZE)
		{
			return positions;
		}
		if (dir.GetHorizontal() > 0)
		{
			if (m_vertical_partitions[start.GetVertical()][start.GetHorizontal()] == 1)
			{
				return positions;
			}
		}
		else
		{
			if (start.GetHorizontal() > 0 
				&& m_vertical_partitions[start.GetVertical()][start.GetHorizontal() - 1] == 1)
			{
				return positions;
			}
		}

		if (m_field[start.GetVertical()][hor_dir] == -1)
		{
			positions.emplace_back(start.GetVertical(), hor_dir);
		}
		else
		{
			bool jump_blocked = false;

			if (dir.GetHorizontal() > 0)
			{
				if (start.GetHorizontal() + 1 >= PARTITION_SIZE 
					|| m_vertical_partitions[start.GetVertical()][start.GetHorizontal() + 1] == 1)
				{
					jump_blocked = true;
				}
			}
			else
			{
				if (start.GetHorizontal() - 2 < 0 
					|| m_vertical_partitions[start.GetVertical()][start.GetHorizontal() - 2] == 1)
				{
					jump_blocked = true;
				}
			}

			int hor_dir2 = start.GetHorizontal() + 2 * dir.GetHorizontal();
			if (!jump_blocked)
			{
				positions.emplace_back(start.GetVertical(), hor_dir2);
			}
			else
			{
				if (start.GetHorizontal() + dir.GetHorizontal() < FIELD_SIZE
					&& start.GetHorizontal() + dir.GetHorizontal() >= 0
					&& m_horizontal_partitions[start.GetVertical()][start.GetHorizontal() + dir.GetHorizontal()] != 1)
				{
					positions.emplace_back(start.GetVertical() + 1, start.GetHorizontal() + dir.GetHorizontal());
				}

				if (start.GetHorizontal() + dir.GetHorizontal() < FIELD_SIZE
					&& start.GetHorizontal() + dir.GetHorizontal() >= 0
					&& start.GetVertical() - 1 >= 0
					&& m_horizontal_partitions[start.GetVertical() - 1][start.GetHorizontal() + dir.GetHorizontal()] != 1)
				{
					positions.emplace_back(start.GetVertical() - 1, start.GetHorizontal() + dir.GetHorizontal());
				}
			}
		}
	}
	else if (dir.GetVertical() != 0)
	{
		int vert_dir = start.GetVertical() + dir.GetVertical();
		if (vert_dir < 0 || vert_dir >= FIELD_SIZE)
		{
			return positions;
		}
		if (dir.GetVertical() > 0)
		{
			if (m_horizontal_partitions[start.GetVertical()][start.GetHorizontal()] == 1)
			{
				return positions;
			}
		}
		else
		{
			if (start.GetVertical() > 0
				&& m_horizontal_partitions[start.GetVertical() - 1][start.GetHorizontal()] == 1)
			{
				return positions;
			}
		}

		if (m_field[vert_dir][start.GetHorizontal()] == -1)
		{
			positions.emplace_back(vert_dir, start.GetHorizontal());
		}
		else
		{
			bool jump_blocked = false;

			if (dir.GetVertical() > 0)
			{
				if (start.GetVertical() + 1 >= PARTITION_SIZE
					|| m_horizontal_partitions[start.GetVertical() + 1][start.GetHorizontal()] == 1)
				{
					jump_blocked = true;
				}
			}
			else
			{
				if (start.GetVertical() - 2 < 0
					|| m_horizontal_partitions[start.GetVertical() - 2][start.GetHorizontal() ] == 1)
				{
					jump_blocked = true;
				}
			}

			int vert_dir2 = start.GetVertical() + 2 * dir.GetVertical();
			if (!jump_blocked)
			{
				positions.emplace_back(vert_dir2, start.GetHorizontal());
			}
			else
			{
				if (start.GetHorizontal() + 1 < FIELD_SIZE
					&& start.GetVertical() + dir.GetVertical() < FIELD_SIZE
					&& start.GetVertical() + dir.GetVertical() >= 0
					&& m_vertical_partitions[start.GetVertical() + dir.GetVertical()][start.GetHorizontal()] != 1)
				{
					if (m_field[start.GetVertical() + dir.GetVertical()][start.GetHorizontal() + 1] == -1)
					positions.emplace_back(start.GetVertical() + vert_dir, start.GetHorizontal() + 1);
				}

				if (start.GetHorizontal() - 1 >= 0
					&& start.GetVertical() + dir.GetVertical() < FIELD_SIZE
					&& start.GetVertical() + dir.GetVertical() >= 0
					&& m_vertical_partitions[start.GetVertical() + dir.GetVertical()][start.GetHorizontal() - 1] != 1)
				{
					if (m_field[start.GetVertical() + dir.GetVertical()][start.GetHorizontal() - 1] == -1)
					positions.emplace_back(start.GetVertical() + dir.GetVertical(), start.GetHorizontal() - 1);
				}
			}
		}
	}

	return positions;

}

void Game::Test()
{
	m_field =
	{
		{-1,-1,-1,-1,-1,-1,-1, 1, 2},
		{-1, 1,-1,-1,-1,-1,-1, 1,-1},
		{-1, 2,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1, 1,-1,-1,-1,-1,-1,-1,-1},
		{-1, 0,-1,-1,-1,-1,-1,-1,-1},
		{-1, 1,-1,-1,-1,-1,-1,-1,-1}
	};

	m_horizontal_partitions =
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
	};
	m_vertical_partitions =
	{
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
	};

	auto output = view::Output();

	output.ShowGameState(this);


	auto dir = GetMovesToDirect({ 7,1 }, { -1,0 });

	if (dir[0] == Position{ 5,1 })
	{
		std::cout << "";
	}

	dir = GetMovesToDirect({ 1,1 }, { 1,0 });

	dir = GetMovesToDirect({ 1,7 }, { 0,1 });

	dir = GetMovesToDirect({ 7,1 }, {  1,0 });

	GetPossibleFigureMoves(0);

}
