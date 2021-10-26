#include "Field.hpp"


using namespace model;

void Field::GameReset()
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

	for (int col = 0; col < PARTITION_SIZE; col++)
	{
		m_horizontal_partitions[col].resize(FIELD_SIZE);
	}
	m_crosst_partitions.clear();

	m_crosst_partitions.resize(PARTITION_SIZE);

	for (int col = 0; col < PARTITION_SIZE; col++)
	{
		m_crosst_partitions[col].resize(FIELD_SIZE);
	}

	m_possible_horizontal_partitions.clear();
	m_possible_vertical_partitions.clear();

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			m_possible_horizontal_partitions.push_back({ i,j });
			m_possible_vertical_partitions.push_back({ i,j });
		}
	}


	for (int col = 0; col < FIELD_SIZE; col++)
	{
		for (int row = 0; row < FIELD_SIZE; row++)
		{
			m_distances[row + col * FIELD_SIZE] = 1000000;
			m_prev_node[row + col * FIELD_SIZE] = { -1,-1 };
		}
	}
}


void Field::SetVerticalPartition(Position position)
{
	m_vertical_partitions[position.GetVertical()][position.GetHorizontal()] = 1;
	m_crosst_partitions[position.GetVertical()][position.GetHorizontal()] = 1;

	m_vertical_partitions[position.GetVertical() + 1][position.GetHorizontal()] = 1;
}

void Field::SetHorizontalPartition(Position position)
{
	m_horizontal_partitions[position.GetVertical()][position.GetHorizontal()] = 1;
	m_crosst_partitions[position.GetVertical()][position.GetHorizontal()] = -1;

	m_horizontal_partitions[position.GetVertical()][position.GetHorizontal() + 1] = 1;

}

std::vector<Position> Field::GetMovesToDirect(Position start, Position dir = { 0,0 })
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
					|| start.GetVertical() >= PARTITION_SIZE
					&& m_vertical_partitions[start.GetVertical()][start.GetHorizontal() + 1] == 1)
				{
					jump_blocked = true;
				}
			}
			else
			{
				if (start.GetHorizontal() - 2 < 0
					|| start.GetVertical() >= PARTITION_SIZE
					|| m_vertical_partitions[start.GetVertical()][start.GetHorizontal() - 2] == 1)
				{
					jump_blocked = true;
				}
			}

			int hor_dir2 = start.GetHorizontal() + 2 * dir.GetHorizontal();
			if (!jump_blocked)
			{
				if (start.GetVertical() <= PARTITION_SIZE)
					positions.emplace_back(start.GetVertical(), hor_dir2);
			}
			else
			{
				if (start.GetHorizontal() + dir.GetHorizontal() < FIELD_SIZE
					&& start.GetHorizontal() + dir.GetHorizontal() >= 0
					&& start.GetVertical() < PARTITION_SIZE
					&& m_horizontal_partitions[start.GetVertical()][start.GetHorizontal() + dir.GetHorizontal()] != 1)
				{
					positions.emplace_back(start.GetVertical() + 1, start.GetHorizontal() + dir.GetHorizontal());
				}

				if (start.GetHorizontal() + dir.GetHorizontal() < FIELD_SIZE
					&& start.GetHorizontal() + dir.GetHorizontal() >= 0
					&& start.GetVertical() - 1 >= 0
					&& start.GetVertical() < PARTITION_SIZE
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
					|| m_horizontal_partitions[start.GetVertical() - 2][start.GetHorizontal()] == 1)
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
					&& start.GetVertical() + dir.GetVertical() < PARTITION_SIZE
					&& start.GetVertical() + dir.GetVertical() >= 0
					&& m_vertical_partitions[start.GetVertical() + dir.GetVertical()][start.GetHorizontal()] != 1)
				{
					if (m_field[start.GetVertical() + dir.GetVertical()][start.GetHorizontal() + 1] == -1)
						positions.emplace_back(start.GetVertical() + dir.GetVertical(), start.GetHorizontal() + 1);
				}

				if (start.GetHorizontal() - 1 >= 0
					&& start.GetVertical() + dir.GetVertical() < PARTITION_SIZE
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
std::vector<Position> Field::GetPossibleFigureMoves(Position start)
{
	std::vector<Position> positions;

	for (auto dir : directions)
	{
		auto pos = GetMovesToDirect(start, dir);
		positions.insert(positions.end(), pos.begin(), pos.end());
	}

	return  positions;
}

void Field::UndoMove()
{
}

void Field::DeleteHorizontalPartition(Position pos)
{
	m_horizontal_partitions[pos.GetVertical()][pos.GetHorizontal()] = 0;
	m_crosst_partitions[pos.GetVertical()][pos.GetHorizontal()] = 0;
	m_horizontal_partitions[pos.GetVertical()][pos.GetHorizontal() + 1] = 0;
}

void Field::DeleteVerticalPartition(Position pos)
{
	m_vertical_partitions[pos.GetVertical()][pos.GetHorizontal()] = 0;
	m_crosst_partitions[pos.GetVertical()][pos.GetHorizontal()] = 0;
	m_vertical_partitions[pos.GetVertical() + 1][pos.GetHorizontal()] = 0;
}

bool Field::AStar(Position start, std::vector<Position>& path, int target)
{
	std::map<int, int> distances = m_distances;
	std::map<int, Position> prev_node = m_prev_node;

	auto cmp = [&](Position a, Position b)
	{

		return distances[a.GetHorizontal() + a.GetVertical() * FIELD_SIZE]
			< distances[b.GetHorizontal() + b.GetVertical() * FIELD_SIZE];
		
	};
	std::multiset<Position, decltype(cmp)> queue(cmp);
	
	distances[start.GetHorizontal() + start.GetVertical() * FIELD_SIZE] = 0;
	prev_node[start.GetHorizontal() + start.GetVertical() * FIELD_SIZE] = Position{ -1,-1 };

	queue.insert(start);
	while (queue.size() > 0)
	{
		auto position = *queue.begin();

		queue.erase(queue.begin());

		if (position.GetVertical() == target)
		{
			path = std::vector<Position>();
			auto p = position;
			while (prev_node[p.GetHorizontal() + p.GetVertical() * FIELD_SIZE] != Position(-1, -1))
			{
				path.push_back(p);
				p = prev_node[p.GetHorizontal() + p.GetVertical() * FIELD_SIZE];
			}

			return true;
		}

		int travelDist = abs(target - position.GetVertical());
		for (auto pos : GetPossibleFigureMoves(position))
		{
			int dist = abs(target - pos.GetVertical());
			if (dist < distances[pos.GetHorizontal() + pos.GetVertical() * FIELD_SIZE])
			{
				distances[pos.GetHorizontal() + pos.GetVertical() * FIELD_SIZE] = dist;
				prev_node[pos.GetHorizontal() + pos.GetVertical() * FIELD_SIZE] = position;
				queue.insert(pos);
			}
		}
	}

	return false;
}
