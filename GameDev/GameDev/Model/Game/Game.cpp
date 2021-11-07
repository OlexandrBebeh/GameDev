#include "Game.hpp"
#include "../DataTypes/MoveProvider.hpp"
#include <set>

using namespace model;

void model::Game::CleanPositions(std::vector<Position>& shortest)
{
	for (int i = 0; shortest.begin() + i != shortest.end(); i++)
	{
		if (shortest[i].GetX() < 0
			|| shortest[i].GetY() < 0
			|| shortest[i].GetX() >= PARTITION_SIZE
			|| shortest[i].GetY() >= PARTITION_SIZE)
		{
			shortest.erase(shortest.begin() + i);
			i--;
		}
	}
}

Game::Game()
{
	m_players_amount = 0;
	m_player_start_pos.push_back({ FIELD_SIZE - FIELD_SIZE,PARTITION_SIZE / 2 });
	m_player_start_pos.push_back({ FIELD_SIZE - 1,PARTITION_SIZE / 2 });
	m_field = std::make_shared<Field>();
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
		player->SetPlayerWinRow(abs(pos.GetY() - PARTITION_SIZE));
		player->SetStartPosition(pos);
		player->SetField(m_field);

		m_field->m_field[pos.GetY()][pos.GetX()] = m_players_amount;

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

void model::Game::SetPossibleVerticalPatrtitions(std::vector<Position> v)
{
	m_field->m_possible_vertical_partitions = v;
}

void model::Game::SetPossibleHorizontalPatrtitions(std::vector<Position> v)
{
	m_field->m_possible_horizontal_partitions = v;
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
	if (move.first == 1)										
	{
		MakeFigureMove(move.second);
	}
	else if (move.first == 2)
	{
		m_field->SetVerticalPartition(move.second);
		m_field->RemoveVerticalPartitionFromAvailable(move.second);

		GetCurrentPlayer()->SetPartitionsAmount(GetCurrentPlayer()->GetPartitionsAmount() - 1);
	}
	else if (move.first == 3)
	{

		m_field->SetHorizontalPartition(move.second);
		m_field->RemoveHorizontalPartitionFromAvailable(move.second);

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

		MoveProvider::GetPartitonsWhichBlockway(shortest, may_block_vert, may_block_hor);

		CleanPositions(may_block_vert);
		CleanPositions(may_block_hor);

		MoveProvider::GetPartitonsWhichBlockWay(m_field.get(),
			may_block_vert,
			vert_part,
			player.second->GetPosition(),
			player.second->GetPlayerWinRow(), false);
		
		MoveProvider::GetPartitonsWhichBlockWay(m_field.get(),
			may_block_hor,
			hor_part,
			player.second->GetPosition(),
			player.second->GetPlayerWinRow(),true);
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

	for (auto pos : m_field->GetPossibleFigureMoves(GetCurrentPlayer()->GetPosition()))
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

std::vector<Move> model::Game::GetUsefullMoves(int player)
{
	std::set<Move> moves;

	for (auto pos : m_field->GetPossibleFigureMoves(m_players[player]->GetPosition()))
	{
		moves.insert(Move(1, pos));
	}
	if (m_players[player]->GetPartitionsAmount())
	{
		
		int v = GetCurrentPlayer()->GetPosition().GetY();
		int h = GetCurrentPlayer()->GetPosition().GetX();
		
		MoveProvider::GetPartitionsArounPlayer({ v,h }, m_field.get(),moves);

		auto p = (GetCurrentPlayerId() + 1) % m_players.size();
		v = m_players[p]->GetPosition().GetY();
		h = m_players[p]->GetPosition().GetX();

		MoveProvider::GetPartitionsArounPlayer({ v,h }, m_field.get(), moves, 2);

		MoveProvider::GetPartitionsAroundPartitions(m_field.get(), moves, 2);
	}

	return std::vector<Move>(moves.begin(),moves.end());
}

void Game::MakeFigureMove(Position position)
{
	auto posible_ways = GetPossibleFigureMoves(m_current_player);

	auto it = std::find(posible_ways.begin(),posible_ways.end(), position);

	if (it != posible_ways.end())
	{
		m_field->m_field[m_players[m_current_player]->GetPosition().GetY()][m_players[m_current_player]->GetPosition().GetX()] = -1;
		m_field->m_field[position.GetY()][position.GetX()] = m_current_player;
		GetCurrentPlayer()->SetPosition(position);
	}
	else
	{
		throw std::exception{"Wrong Move"};
	}
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

int model::Game::CheckWin()
{
	int winner = -1;
	for (auto& player : m_players)
	{
		if (player.second->GetPosition().GetY() 
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
