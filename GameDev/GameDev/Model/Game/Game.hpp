#pragma once

#include <map>
#include <vector>
#include <iostream>
#include "../Player/Player.hpp"
#include "../DataTypes/Position.hpp"


constexpr int FIELD_SIZE = 9;
constexpr int PARTITION_SIZE = 8;

namespace model
{
	class Game
	{
	public:
		Game();
		~Game();
		void GameReset();
		Player* GetCurrentPlayer() const;
		void AddPlayer(Player* player);
		std::vector<std::vector<int>> GetField() const;
		std::vector<std::vector<int>> GetVerticalPatrtitions() const;
		std::vector<std::vector<int>> GetHorizontalPatrtitions() const;
		std::vector<std::vector<int>> GetCrosstPatritions() const;

		bool ValidField();
		bool Dijkstra(std::vector<Position>& path);

		void MakeFigureMove(Position);
		void SetVerticalPartition(Position);
		void SetHorizontalPartition(Position);

		void MakeMove(model::Move move);

		bool IsGameEnd();

		std::vector<Position> GetPossibleFigureMoves(int player_id);

		std::vector<Position> GetPossibleVerrticalPartitions(int player_id);
		std::vector<Position> GetPossibleHorizontalPartitions(int player_id);

		std::vector<Position> GetMovesToDirect(Position, Position);
		std::vector<Position> GetPossibleFigureMoves(Position);

		void NextPlayer();

		void Test();
	protected:
		std::map<int, std::unique_ptr<Player>> m_players;

		int m_players_amount{ 0 };

		int m_current_player{ 0 };

		std::vector<std::vector<int>> m_field;

		std::vector<std::vector<int>> m_vertical_partitions;

		std::vector<std::vector<int>> m_horizontal_partitions;

		std::vector<std::vector<int>> m_crosst_partitions;

		std::vector<std::pair<int,Move>> m_history;

	};
}