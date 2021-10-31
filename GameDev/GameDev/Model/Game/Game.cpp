#include "Game.hpp"
#include "../../View/Output.hpp"
#include <set>
#include "../../View/Output.hpp"

using namespace model;

void model::Game::CleanPositions(std::vector<Position>& shortest)
{
	for (int i = 0; shortest.begin() + i != shortest.end(); i++)
	{
		if (shortest[i].GetHorizontal() < 0
			|| shortest[i].GetVertical() < 0
			|| shortest[i].GetHorizontal() >= PARTITION_SIZE
			|| shortest[i].GetVertical() >= PARTITION_SIZE)
		{
			shortest.erase(shortest.begin() + i);
			i--;
		}
	}
}

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
		this->m_players.emplace(player.first, std::make_shared<Player>(*player.second.get()));
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

	throw std::exception{ "Game::MakeMove - wrong move" };

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
			
			m_field->RemoveVerticalPartitionFromAvailable(move.second);
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
			m_field->RemoveHorizontalPartitionFromAvailable(move.second);
			m_field->m_history.emplace_back(m_current_player, Move(3, move.second));
		}

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else
	{
		throw std::exception{ "Game::MakeMove - wrong move" };
	}

	UpdatePossiblePartitions();

	NextPlayer();
}

void model::Game::MakeTrustMove(model::Move move)
{
	if (move.first == 1)										{
		MakeFigureMove(move.second);
	}
	else if (move.first == 2)
	{
		m_field->SetVerticalPartition(move.second);
		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else if (move.first == 3)
	{

		m_field->SetHorizontalPartition(move.second);

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else
	{
		throw std::exception{ "Game::MakeMove - wrong move" };
	}

	NextPlayer();
}

void model::Game::UpdatePossiblePartitions()
{
	std::vector<Position> vert_part = m_field->m_not_blocked_vertical_partitions;
	std::vector<Position> hor_part = m_field->m_not_blocked_horizontal_partitions;

	for (auto& player : m_players)
	{
		std::vector<Position> shortest;
		m_field->AStar(player.second->GetPosition(),
			shortest,
			player.second->GetPlayerWinRow());

		shortest.push_back(player.second->GetPosition());
		std::vector<Position> may_block_vert;
		std::vector<Position> may_block_hor;

		for (auto b = shortest.begin(), 
			c = shortest.begin() + 1; 
			c != shortest.end(); 
			b++, c++)
		{
			auto temp = *c - *b;

			if (temp.GetVertical() == 1 && temp.GetHorizontal() == 0)
			{
				may_block_hor.push_back(*b);
				may_block_hor.push_back(*b + Position{ 0, -1 });
			}
			else if (temp.GetHorizontal() == 1 && temp.GetVertical() == 0)
			{
				may_block_vert.push_back(*b);
				may_block_vert.push_back(*b + Position{ -1, 0 });
			}
			else if (temp.GetVertical() == -1 && temp.GetHorizontal() == 0)
			{
				may_block_hor.push_back(*b + Position{ -1, 0 });
				may_block_hor.push_back(*b + Position{ -1, -1 });
			}
			else if (temp.GetHorizontal() == -1 && temp.GetVertical() == 0)
			{
				may_block_vert.push_back(*b + Position{  0, -1 });
				may_block_vert.push_back(*b + Position{ -1, -1 });
			}
			else if (temp.GetVertical() == 2)
			{
				may_block_hor.push_back(*b);
				may_block_hor.push_back(*b + Position{ 1, 0 });
				may_block_hor.push_back(*b + Position{ 0, -1 });
				may_block_hor.push_back(*b + Position{ 1, -1 });
			}
			else if (temp.GetHorizontal() == 2)
			{
				may_block_vert.push_back(*b);
				may_block_vert.push_back(*b + Position{ 0, 1 });
				may_block_hor.push_back(*b + Position{ -1, 0 });
				may_block_hor.push_back(*b + Position{ -1, 1 });
			}
			else if (temp.GetVertical() == -2)
			{
				may_block_hor.push_back(*b + Position{ -1, 0 });
				may_block_hor.push_back(*b + Position{ -1, -1 });
				may_block_hor.push_back(*b + Position{ -2, 0 });
				may_block_hor.push_back(*b + Position{ -2, -1 });
			}
			else if (temp.GetHorizontal() == -2)
			{
				may_block_vert.push_back(*b + Position{ -1, -2 });
				may_block_vert.push_back(*b + Position{ -1, -1 });
				may_block_vert.push_back(*b + Position{ 0, -2 });
				may_block_vert.push_back(*b + Position{ 0, -1 });
			}
			else if (temp.GetVertical() == 1 && temp.GetHorizontal() == 1)
			{
				may_block_vert.push_back(*b);
				may_block_vert.push_back(*b + Position{1,0});
				may_block_vert.push_back(*b + Position{ 0,-1 });

				may_block_hor.push_back(*b);
				may_block_hor.push_back(*b + Position{ 0, 1 });
				may_block_hor.push_back(*b + Position{ 0,-1 });
			}
			else if (temp.GetVertical() == -1 && temp.GetHorizontal() == 1)
			{
				may_block_vert.push_back(*b);
				may_block_vert.push_back(*b + Position{ -1, 0 });
				may_block_vert.push_back(*b + Position{ -2, 0 });

				may_block_hor.push_back(*b + Position{ -1, 0 });
				may_block_hor.push_back(*b + Position{ -1, 1 });
				may_block_hor.push_back(*b + Position{ -1, -1 });

			}
			else if (temp.GetVertical() == -1 && temp.GetHorizontal() == -1)
			{
				may_block_vert.push_back(*b + Position{ 0,-1 });
				may_block_vert.push_back(*b + Position{ -1,-1 });
				may_block_vert.push_back(*b + Position{ -2,-1 });

				may_block_hor.push_back(*b + Position{ -1,  0 });
				may_block_hor.push_back(*b + Position{ -1, -1 });
				may_block_hor.push_back(*b + Position{ -1, -2 });
			}
			else if (temp.GetVertical() == 1 && temp.GetHorizontal() == -1)
			{
				may_block_vert.push_back(*b + Position{ 0,-1 });
				may_block_vert.push_back(*b + Position{ 1,-1 });
				may_block_vert.push_back(*b + Position{ -1,-1 });
				
				may_block_hor.push_back(*b);
				may_block_hor.push_back(*b + Position{ 0, -1 });
				may_block_hor.push_back(*b + Position{ 0, -2 });
			}

		}

		CleanPositions(may_block_vert);
		CleanPositions(may_block_hor);

		for (auto& par : may_block_hor)
		{
			auto cross = std::find(hor_part.begin(),
				hor_part.end(),
				par);
			if (cross != hor_part.end())
			{
				std::vector<Position> temp;
				m_field->SetHorizontalPartition(par);
				bool is_valid = m_field->AStar(player.second->GetPosition(),
					temp,
					player.second->GetPlayerWinRow());
				if (!is_valid)
				{
					hor_part.erase(cross);
				}
				m_field->DeleteHorizontalPartition(par);
			}
		}
		for (auto& par : may_block_vert)
		{
			auto cross = std::find(vert_part.begin(),
				vert_part.end(),
				par);
			if (cross != vert_part.end())
			{
				std::vector<Position> temp;
				m_field->SetVerticalPartition(par);
				bool is_valid = m_field->AStar(player.second->GetPosition(),
					temp,
					player.second->GetPlayerWinRow());
				if (!is_valid)
				{
					vert_part.erase(cross);
				}
				m_field->DeleteVerticalPartition(par);
			}
		}
	}
	m_field->m_possible_horizontal_partitions = hor_part;
	m_field->m_possible_vertical_partitions = vert_part;

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

	for (auto& player : m_players)
	{
		std::vector<Position> shortest;
		m_field->AStar(player.second->GetPosition(),
			shortest,
			player.second->GetPlayerWinRow());
		shortest.push_back(player.second->GetPosition());
		
		for (int i = 0; shortest.begin() + i != shortest.end(); i++)
		{
			if (shortest[i].GetHorizontal() == PARTITION_SIZE || shortest[i].GetVertical() == PARTITION_SIZE)
			{
				shortest.erase(shortest.begin() + i);
				i--;
			}
		}

		for (auto& par : shortest)
		{
			auto cross = std::find(m_field->m_not_blocked_vertical_partitions.begin(), 
									m_field->m_not_blocked_vertical_partitions.end(), 
									par);
			if (cross != shortest.end())
			{
				std::vector<Position> temp;


				m_field->SetVerticalPartition(par);
				bool is_valid = m_field->AStar(player.second->GetPosition(),
					temp,
					player.second->GetPlayerWinRow());
				if (!is_valid)
				{
					remove(positions.begin(), positions.end(), par);
				}
				m_field->DeleteVerticalPartition(par);
			}
			else
			{

			}
		}

	}

	return  positions;
}

std::vector<Position> Game::CheckPossibleHorizontalPartitions()
{
	std::vector<Position> positions = m_field->m_not_blocked_horizontal_partitions;


	for (auto& player : m_players)
	{
		std::vector<Position> shortest;

		m_field->AStar(player.second->GetPosition(),
			shortest,
			player.second->GetPlayerWinRow());

		shortest.push_back(player.second->GetPosition());

		for (int i = 0; shortest.begin()+i != shortest.end(); i++)
		{
			if (shortest[i].GetHorizontal() == PARTITION_SIZE || shortest[i].GetVertical() == PARTITION_SIZE)
			{
				shortest.erase(shortest.begin() + i);
				i--;
			}
		}
		for (auto& par : shortest)
		{
			auto cross = std::find(shortest.begin(), shortest.end(), par);
			if (cross != shortest.end())
			{
				std::vector<Position> temp;
				m_field->m_horizontal_partitions[par.GetVertical()][par.GetHorizontal()] = 1;
				m_field->m_crosst_partitions[par.GetVertical()][par.GetHorizontal()] = -1;
				m_field->m_horizontal_partitions[par.GetVertical()][par.GetHorizontal() + 1] = 1;
				bool is_valid = m_field->AStar(player.second->GetPosition(),
					temp,
					player.second->GetPlayerWinRow());
				if (!is_valid || temp.empty())
				{
					remove(positions.begin(),positions.end(),par);
				}
				m_field->DeleteHorizontalPartition(par);
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
	//m_field->m_field =
	//{
	//	{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1, 1,-1, 0,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	//		{-1,-1,-1,-1,-1,-1,-1,-1,-1,}
	//};

	//m_field->m_horizontal_partitions =
	//{
	//			{0,1,1,0,1,1,1,1,0},
	//		{0,0,1,1,0,1,1,0,0},
	//		{0,1,1,0,1,1,1,1,0},
	//		{0,0,0,1,1,0,0,0,0},
	//		{0,0,1,1,0,0,0,0,0},
	//		{1,1,0,1,1,1,1,1,1},
	//		{0,0,0,0,1,1,0,0,0},
	//		{0,0,0,0,1,1,0,0,0},
	//};
	//m_field->m_vertical_partitions =
	//{
	//	{1,0,0,0,0,0,0,0},
	//		{1,1,0,0,0,0,0,1},
	//		{1,1,1,0,0,0,0,1},
	//		{1,1,1,0,0,1,0,0},
	//		{1,1,0,0,0,1,1,1},
	//		{1,0,0,0,0,0,1,1},
	//		{0,0,0,0,0,1,0,0},
	//		{0,1,1,0,0,1,0,1},
	//		{0,1,1,0,0,0,0,1},
	//};
	//m_field->m_crosst_partitions =
	//{
	//		{1,-1,0,0,-1,0,-1,0,},
	//		{0,1,-1,0,0,-1,0,1,},
	//		{1,-1,1,0,-1,0,-1,0,},
	//		{0,1,0,-1,0,1,0,0,},
	//		{1,0,-1,0,0,0,1,1,},
	//		{-1,0,0,-1,0,-1,0,-1,},
	//		{0,0,0,0,-1,1,0,0,},
	//		{0,1,1,0,-1,0,0,1,}
	//};

	m_field->m_field =
	{
		{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, 0, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, 1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1,},
	};

	m_field->m_horizontal_partitions =
	{
			{1, 1, 0, 0, 1, 1, 0, 1, 1,},
			{1, 1, 0, 0, 0, 0, 1, 1, 0,},
			{1, 1, 0, 1, 1, 0, 1, 1, 0,},
			{0, 1, 1, 0, 0, 1, 1, 0, 0,},
			{1, 1, 0, 0, 0, 1, 1, 1, 1,},
			{0, 1, 1, 1, 1, 0, 1, 1, 0,},
			{0, 1, 1, 0, 1, 1, 1, 1, 0,},
			{0, 0, 1, 1, 1, 1, 0, 0, 0,},

	};
	m_field->m_vertical_partitions =
	{
		{0, 1, 1, 0, 0, 0, 1, 0,},
			{0, 1, 1, 1, 0, 1, 1, 0,},
			{0, 0, 0, 1, 1, 1, 0, 0,},
			{1, 0, 1, 0, 1, 0, 1, 1,},
			{1, 1, 1, 1, 0, 0, 1, 1,},
			{0, 1, 1, 1, 0, 1, 0, 0,},
			{1, 0, 1, 1, 0, 1, 0, 1,},
			{1, 0, 0, 1, 0, 1, 1, 1,},
			{0, 0, 0, 0, 0, 1, 1, 0,},
	};
	m_field->m_crosst_partitions =
	{
			 {-1, 1, 1, 0, -1, 0, 1, -1,},
			{-1, 0, 0, 1, 0, 1, -1, 0,},
			{-1, 0, 0, -1, 1, 0, -1, 0,},
			{1, -1, 1, 0, 0, -1, 1, 1,},
			{-1, 1, -1, 1, 0, -1, 0, -1,},
			{0, -1, 1, -1, 0, 1, -1, 0,},
			{1, -1, 0, 1, -1, 0, -1, 1,},
			{0, 0, -1, 0, -1, 1, 1, 0,},

	};

		
	//auto v = view::Output();

	//v.ShowGameState(this);

	Position player_pos{4, 2};
	std::vector< Position> posi;
	auto t = m_field->AStar(player_pos, posi,8);
	posi.size();

	ResetGame();
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

Move model::Game::GetShortesFigureMove()
{
	std::vector<Position> positions;
	m_field->AStar(GetCurrentPlayer()->GetPosition(),
					positions,
					GetCurrentPlayer()->GetPlayerWinRow());

	if (positions.empty())
	{
		positions = GetPossibleFigureMoves(m_current_player);
	}

	return Move(1,positions.back());
}
