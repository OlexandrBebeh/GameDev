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
	m_not_blocked_horizontal_partitions.clear();
	m_not_blocked_vertical_partitions.clear();

	for (int i = 0; i < PARTITION_SIZE; i++)
	{
		for (int j = 0; j < PARTITION_SIZE; j++)
		{
			m_not_blocked_horizontal_partitions.push_back({ i,j });
			m_not_blocked_vertical_partitions.push_back({ i,j });

		}
	}
}


void Field::SetVerticalPartition(Position position)
{
	m_vertical_partitions[position.GetY()][position.GetX()] = 1;
	m_crosst_partitions[position.GetY()][position.GetX()] = 1;

	m_vertical_partitions[position.GetY() + 1][position.GetX()] = 1;
}

void Field::RemoveVerticalPartitionFromAvailable(Position position)
{
	m_not_blocked_vertical_partitions.erase(find(m_not_blocked_vertical_partitions.begin(), m_not_blocked_vertical_partitions.end(), position));
	
	auto it = find(m_not_blocked_horizontal_partitions.begin(), m_not_blocked_horizontal_partitions.end(), position);
	if (it != m_not_blocked_horizontal_partitions.end())
		m_not_blocked_horizontal_partitions.erase(it);

	it = find(m_not_blocked_vertical_partitions.begin(), m_not_blocked_vertical_partitions.end(), Position{ position.GetY() + 1,position.GetX() });
	if (it != m_not_blocked_vertical_partitions.end())
		m_not_blocked_vertical_partitions.erase(it);

	it = find(m_not_blocked_vertical_partitions.begin(), m_not_blocked_vertical_partitions.end(), Position{ position.GetY() - 1,position.GetX() });
	if (it != m_not_blocked_vertical_partitions.end())
		m_not_blocked_vertical_partitions.erase(it);

}

void Field::SetHorizontalPartition(Position position)
{
	m_horizontal_partitions[position.GetY()][position.GetX()] = 1;
	m_crosst_partitions[position.GetY()][position.GetX()] = -1;

	m_horizontal_partitions[position.GetY()][position.GetX() + 1] = 1;
}

void model::Field::RemoveHorizontalPartitionFromAvailable(Position position)
{
	m_not_blocked_horizontal_partitions.erase(find(m_not_blocked_horizontal_partitions.begin(), m_not_blocked_horizontal_partitions.end(), position));

	auto it = find(m_not_blocked_vertical_partitions.begin(), m_not_blocked_vertical_partitions.end(), position);
	if (it != m_not_blocked_vertical_partitions.end())
		m_not_blocked_vertical_partitions.erase(it);

	it = find(m_not_blocked_horizontal_partitions.begin(), m_not_blocked_horizontal_partitions.end(), Position{ position.GetY(),position.GetX() + 1 });
	if (it != m_not_blocked_horizontal_partitions.end())
		m_not_blocked_horizontal_partitions.erase(it);
	
	it = find(m_not_blocked_horizontal_partitions.begin(), m_not_blocked_horizontal_partitions.end(), Position{ position.GetY(),position.GetX() - 1 });
	if (it != m_not_blocked_horizontal_partitions.end())
		m_not_blocked_horizontal_partitions.erase(it);
}

std::vector<Position> Field::GetMovesToDirect(Position start, Position dir = { 0,0 })
{
	std::vector<Position> positions;

	if (dir.GetX() != 0)
	{
		int hor_dir = start.GetX() + dir.GetX();
		if (hor_dir < 0 || hor_dir >= FIELD_SIZE)
		{
			return positions;
		}
		if (dir.GetX() > 0)
		{
			if (m_vertical_partitions[start.GetY()][start.GetX()] == 1)
			{
				return positions;
			}
		}
		else
		{
			if (start.GetX() > 0
				&& m_vertical_partitions[start.GetY()][start.GetX() - 1] == 1)
			{
				return positions;
			}
		}

		if (m_field[start.GetY()][hor_dir] == -1)
		{
			positions.emplace_back(start.GetY(), hor_dir);
		}
		else
		{
			bool jump_blocked = false;

			if (dir.GetX() > 0)
			{
				if (start.GetX() + 1 >= PARTITION_SIZE
					|| start.GetY() >= PARTITION_SIZE
					|| m_vertical_partitions[start.GetY()][start.GetX() + 1] == 1)
				{
					jump_blocked = true;
				}
			}
			else
			{
				if (start.GetX() - 2 < 0
					|| start.GetY() >= PARTITION_SIZE
					|| m_vertical_partitions[start.GetY()][start.GetX() - 2] == 1)
				{
					jump_blocked = true;
				}
			}

			int hor_dir2 = start.GetX() + 2 * dir.GetX();
			if (!jump_blocked)
			{
				if (start.GetY() <= PARTITION_SIZE)
					positions.emplace_back(start.GetY(), hor_dir2);
			}
			else
			{
				if (start.GetX() + dir.GetX() < FIELD_SIZE
					&& start.GetX() + dir.GetX() >= 0
					&& start.GetY() < PARTITION_SIZE
					&& m_horizontal_partitions[start.GetY()][start.GetX() + dir.GetX()] != 1)
				{
					positions.emplace_back(start.GetY() + 1, start.GetX() + dir.GetX());
				}

				if (start.GetX() + dir.GetX() < FIELD_SIZE
					&& start.GetX() + dir.GetX() >= 0
					&& start.GetY() - 1 >= 0
					&& start.GetY() < PARTITION_SIZE
					&& m_horizontal_partitions[start.GetY() - 1][start.GetX() + dir.GetX()] != 1)
				{
					positions.emplace_back(start.GetY() - 1, start.GetX() + dir.GetX());
				}
			}
		}
	}
	else if (dir.GetY() != 0)
	{
		int vert_dir = start.GetY() + dir.GetY();
		if (vert_dir < 0 || vert_dir >= FIELD_SIZE)
		{
			return positions;
		}
		if (dir.GetY() > 0)
		{
			if (m_horizontal_partitions[start.GetY()][start.GetX()] == 1)
			{
				return positions;
			}
		}
		else
		{
			if (start.GetY() > 0
				&& m_horizontal_partitions[start.GetY() - 1][start.GetX()] == 1)
			{
				return positions;
			}
		}

		if (m_field[vert_dir][start.GetX()] == -1)
		{
			positions.emplace_back(vert_dir, start.GetX());
		}
		else
		{
			bool jump_blocked = false;

			if (dir.GetY() > 0)
			{
				if (start.GetY() + 1 >= PARTITION_SIZE
					|| m_horizontal_partitions[start.GetY() + 1][start.GetX()] == 1)
				{
					jump_blocked = true;
				}
			}
			else
			{
				if (start.GetY() - 2 < 0
					|| m_horizontal_partitions[start.GetY() - 2][start.GetX()] == 1)
				{
					jump_blocked = true;
				}
			}

			int vert_dir2 = start.GetY() + 2 * dir.GetY();
			if (!jump_blocked)
			{
				positions.emplace_back(vert_dir2, start.GetX());
			}
			else
			{
				if (start.GetX() < PARTITION_SIZE
					&& start.GetY() + dir.GetY() <= PARTITION_SIZE
					&& start.GetY() + dir.GetY() >= 0
					&& m_vertical_partitions[start.GetY() + dir.GetY()][start.GetX()] != 1)
				{
					if (m_field[start.GetY() + dir.GetY()][start.GetX() + 1] == -1)
						positions.emplace_back(start.GetY() + dir.GetY(), start.GetX() + 1);
				}

				if (start.GetX() - 1 >= 0
					&& start.GetY() + dir.GetY() <= PARTITION_SIZE
					&& start.GetY() + dir.GetY() >= 0
					&& m_vertical_partitions[start.GetY() + dir.GetY()][start.GetX() - 1] != 1)
				{
					if (m_field[start.GetY() + dir.GetY()][start.GetX() - 1] == -1)
						positions.emplace_back(start.GetY() + dir.GetY(), start.GetX() - 1);
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
	m_horizontal_partitions[pos.GetY()][pos.GetX()] = 0;
	m_crosst_partitions[pos.GetY()][pos.GetX()] = 0;
	m_horizontal_partitions[pos.GetY()][pos.GetX() + 1] = 0;
}

void Field::DeleteVerticalPartition(Position pos)
{
	m_vertical_partitions[pos.GetY()][pos.GetX()] = 0;
	m_crosst_partitions[pos.GetY()][pos.GetX()] = 0;
	m_vertical_partitions[pos.GetY() + 1][pos.GetX()] = 0;
}

bool Field::AStar(Position start, std::vector<Position>& path, int target)
{
	std::map<int, int> distances = m_distances;
	std::map<int, Position> prev_node = m_prev_node;

	auto cmp = [&](Position a, Position b)
	{

		return distances[a.GetX() + a.GetY() * FIELD_SIZE]
			< distances[b.GetX() + b.GetY() * FIELD_SIZE];
		
	};
	std::multiset<Position, decltype(cmp)> queue(cmp);
	
	distances[start.GetX() + start.GetY() * FIELD_SIZE] = 0;
	prev_node[start.GetX() + start.GetY() * FIELD_SIZE] = Position{ -1,-1 };

	queue.insert(start);
	int player = m_field[start.GetY()][start.GetX()];
	while (queue.size() > 0)
	{
		auto position = *queue.begin();

		queue.erase(queue.begin());
		m_field[position.GetY()][position.GetX()] = player;
		if (position.GetY() == target)
		{
			path = std::vector<Position>();
			auto p = position;
			while (prev_node[p.GetX() + p.GetY() * FIELD_SIZE] != Position(-1, -1))
			{
				path.push_back(p);
				p = prev_node[p.GetX() + p.GetY() * FIELD_SIZE];
			}
			m_field[position.GetY()][position.GetX()] = -1;
			m_field[start.GetY()][start.GetX()] = player;
			return true;
		}

		int travelDist = abs(target - position.GetY());
		int travelWay = distances[position.GetX() + position.GetY() * FIELD_SIZE] - abs(target - position.GetY());
		for (auto pos : GetPossibleFigureMoves(position))
		{
			int dist = abs(target - pos.GetY()) + travelWay + 1;
			if (dist < distances[pos.GetX() + pos.GetY() * FIELD_SIZE])
			{
				distances[pos.GetX() + pos.GetY() * FIELD_SIZE] = dist;
				prev_node[pos.GetX() + pos.GetY() * FIELD_SIZE] = position;
				queue.insert(pos);
			}
		}

		m_field[position.GetY()][position.GetX()] = -1;
	}
	m_field[start.GetY()][start.GetX()] = player;

	return false;
}
