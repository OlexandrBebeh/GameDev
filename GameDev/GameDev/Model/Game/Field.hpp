#pragma once
#include <vector>
#include <map>
#include <set>
#include "../DataTypes/Position.hpp"

namespace model
{

	constexpr int FIELD_SIZE = 9;
	constexpr int PARTITION_SIZE = 8;

	class Field
	{

	public:
		Field() 
		{
			directions.push_back({ 1,0 });
			directions.push_back({ -1,0 });
			directions.push_back({ 0,1 });
			directions.push_back({ 0,-1 });

			GameReset();
		};
		void GameReset();

		bool AStar(Position start, std::vector<Position>& path, int target);

		std::vector<Position> GetMovesToDirect(Position, Position);
		std::vector<Position> GetPossibleFigureMoves(Position);

		void UndoMove();

		void DeleteHorizontalPartition(Position pos);
		void DeleteVerticalPartition(Position pos);

		void SetVerticalPartition(Position);
		void SetHorizontalPartition(Position);

		std::vector<std::vector<int>> m_field;

		std::vector<std::vector<int>> m_vertical_partitions;

		std::vector<std::vector<int>> m_horizontal_partitions;

		std::vector<std::vector<int>> m_crosst_partitions;

		std::vector<std::pair<int, Move>> m_history;

		std::vector<Position> directions;

		std::vector<Position> m_possible_vertical_partitions;

		std::vector<Position> m_possible_horizontal_partitions;

		std::map<int, int> m_distances;
		std::map<int, Position> m_prev_node;
	};
}