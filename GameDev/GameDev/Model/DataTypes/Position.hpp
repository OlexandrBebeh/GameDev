#pragma once
#include <utility>
#include <iostream>
#include <ostream>

namespace model
{

	class Position
	{
	public:
		Position(int _x, int _y) : y(_x), x(_y) {};
		Position() :  y(-1), x(-1) {};
		~Position() {};

		const int GetX() const
		{
			return x;
		}

		const int GetY() const
		{
			return y;
		}

		bool operator==(const Position& pos) const
		{
			 return pos.x == this->x
				&& pos.y == this->y;
		
		}

		bool operator!=(Position pos)
		{
			return pos.x != this->x
				|| pos.y != this->y;
		}
		 
		bool operator<(const Position& pos) const
		{
			return (pos.y * 9 + pos.x)
				< (this->y * 9 + this->x);
		}

		Position operator-(const Position& pos) const
		{
			return Position(this->y - pos.y,
				this->x - pos.x);
		}

		Position operator+(const Position& pos) const
		{
			return Position(this->y + pos.y,
				this->x + pos.x);
		}

		friend std::ostream& operator<<(std::ostream& os, const Position& pos)
		{
			os << "{" << pos.y << "," << pos.x << "}";
			return os;
		}

	protected:
		int y;
		int x;
	};

	using Move = std::pair<int, Position>;
}