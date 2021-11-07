#pragma once
#include "../Player/Player.hpp"
#include "../Game/Field.hpp"
#include <map>

namespace model
{
	class MoveProvider
	{
		static std::map<Position, std::vector<Position>> may_block_dir_vert;
		static std::map<Position, std::vector<Position>> may_block_dir_hor;

	public:
		static void GetPartitionsArounPlayer(Position player, Field* field, std::set<Move>& moves,int dist = 1)
		{
			for (int i = player.GetY() - dist; i < player.GetY() + dist; i++)
			{
				for (int j = player.GetX() - dist; j < player.GetX() + dist; j++)
				{
					auto pos = Position{ i,j };
					if (std::find(field->m_possible_vertical_partitions.begin(),
						field->m_possible_vertical_partitions.end(), pos)
						!= field->m_possible_vertical_partitions.end())
					{
						moves.insert(Move(2, pos));
					}
					if (std::find(field->m_possible_horizontal_partitions.begin(),
						field->m_possible_horizontal_partitions.end(), pos)
						!= field->m_possible_horizontal_partitions.end())
					{
						moves.insert(Move(3, pos));
					}
				}
			}
		}

		static void GetPartitionsAroundPartitions(Field* field, std::set<Move>& moves, int dist = 1)
		{
			for (int i = 0; i < PARTITION_SIZE; i++)
			{
				for (int j = 0; j < PARTITION_SIZE; j++)
				{
					if (field->m_crosst_partitions[i][j] != 0)
					{
						for (int k = i - dist; k < i + dist; k++)
						{
							for (int l = j - dist; l < j + dist; l++)
							{
								if (field->m_crosst_partitions[i][j] == 0)
								{
									auto pos = Position{ k,l };
									if (std::find(field->m_possible_vertical_partitions.begin(),
										field->m_possible_vertical_partitions.end(), pos)
										!= field->m_possible_vertical_partitions.end())
									{
										moves.insert(Move(2, pos));
									}
									if (std::find(field->m_possible_horizontal_partitions.begin(),
										field->m_possible_horizontal_partitions.end(), pos)
										!= field->m_possible_horizontal_partitions.end())
									{
										moves.insert(Move(3, pos));
									}
								}
							}
						}
					}
				}
			}
		}

		static void GetPartitonsWhichBlockway(const std::vector<Position>& shortest,
													std::vector<Position>& may_block_vert,
													std::vector<Position>& may_block_hor)
		{
			for (auto b = shortest.begin(),
				c = shortest.begin() + 1;
				c != shortest.end();
				b++, c++)
			{
				auto temp = *c - *b;

				if (temp.GetY() == 1 && temp.GetX() == 0)
				{
					may_block_hor.push_back(*b);
					may_block_hor.push_back(*b + Position{ 0, -1 });
				}
				else if (temp.GetX() == 1 && temp.GetY() == 0)
				{
					may_block_vert.push_back(*b);
					may_block_vert.push_back(*b + Position{ -1, 0 });
				}
				else if (temp.GetY() == -1 && temp.GetX() == 0)
				{
					may_block_hor.push_back(*b + Position{ -1, 0 });
					may_block_hor.push_back(*b + Position{ -1, -1 });
				}
				else if (temp.GetX() == -1 && temp.GetY() == 0)
				{
					may_block_vert.push_back(*b + Position{ 0, -1 });
					may_block_vert.push_back(*b + Position{ -1, -1 });
				}
				else if (temp.GetY() == 2)
				{
					may_block_hor.push_back(*b);
					may_block_hor.push_back(*b + Position{ 1, 0 });
					may_block_hor.push_back(*b + Position{ 0, -1 });
					may_block_hor.push_back(*b + Position{ 1, -1 });
				}
				else if (temp.GetX() == 2)
				{
					may_block_vert.push_back(*b);
					may_block_vert.push_back(*b + Position{ 0, 1 });
					may_block_vert.push_back(*b + Position{ -1, 0 });
					may_block_vert.push_back(*b + Position{ -1, 1 });
				}
				else if (temp.GetY() == -2)
				{
					may_block_hor.push_back(*b + Position{ -1, 0 });
					may_block_hor.push_back(*b + Position{ -1, -1 });
					may_block_hor.push_back(*b + Position{ -2, 0 });
					may_block_hor.push_back(*b + Position{ -2, -1 });
				}
				else if (temp.GetX() == -2)
				{
					may_block_vert.push_back(*b + Position{ -1, -2 });
					may_block_vert.push_back(*b + Position{ -1, -1 });
					may_block_vert.push_back(*b + Position{ 0, -2 });
					may_block_vert.push_back(*b + Position{ 0, -1 });
				}
				else if (temp.GetY() == 1 && temp.GetX() == 1)
				{
					may_block_vert.push_back(*b);
					may_block_vert.push_back(*b + Position{ 1, 0 });
					may_block_vert.push_back(*b + Position{ -1, 0 });

					may_block_hor.push_back(*b);
					may_block_hor.push_back(*b + Position{ 0, 1 });
					may_block_hor.push_back(*b + Position{ 0,-1 });
				}
				else if (temp.GetY() == -1 && temp.GetX() == 1)
				{
					may_block_vert.push_back(*b);
					may_block_vert.push_back(*b + Position{ -1, 0 });
					may_block_vert.push_back(*b + Position{ -2, 0 });

					may_block_hor.push_back(*b + Position{ -1, 0 });
					may_block_hor.push_back(*b + Position{ -1, 1 });
					may_block_hor.push_back(*b + Position{ -1, -1 });

				}
				else if (temp.GetY() == -1 && temp.GetX() == -1)
				{
					may_block_vert.push_back(*b + Position{ 0,-1 });
					may_block_vert.push_back(*b + Position{ -1,-1 });
					may_block_vert.push_back(*b + Position{ -2,-1 });

					may_block_hor.push_back(*b + Position{ -1,  0 });
					may_block_hor.push_back(*b + Position{ -1, -1 });
					may_block_hor.push_back(*b + Position{ -1, -2 });
				}
				else if (temp.GetY() == 1 && temp.GetX() == -1)
				{
					may_block_vert.push_back(*b + Position{ 0,-1 });
					may_block_vert.push_back(*b + Position{ 1,-1 });
					may_block_vert.push_back(*b + Position{ -1,-1 });

					may_block_hor.push_back(*b);
					may_block_hor.push_back(*b + Position{ 0, -1 });
					may_block_hor.push_back(*b + Position{ 0, -2 });
				}
			}
		}

		static void GetPartitonsWhichBlockWay(Field* field, 
												const std::vector<Position>& may_block,
												std::vector<Position>& possible_par,
												Position pos, 
												int target, 
												bool swth)
		{
			for (auto& par : may_block)
			{
				auto cross = std::find(possible_par.begin(),
					possible_par.end(),
					par);
				if (cross != possible_par.end())
				{
					std::vector<Position> temp;
					if (swth)
						field->SetHorizontalPartition(par);
					else
						field->SetVerticalPartition(par);

					bool is_valid = field->AStar(pos, temp, target);
					if (!is_valid)
					{
						possible_par.erase(cross);
					}
					if (swth)
						field->DeleteHorizontalPartition(par);
					else
						field->DeleteVerticalPartition(par);
				}
			}
		}
	};
}
