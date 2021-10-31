#pragma once
#include <utility>
#include <iostream>
#include <ostream>

namespace model
{

	class Position
	{
	public:
		Position(int _x, int _y) : vertical(_x), horizontal(_y) {};
		Position() :  vertical(-1), horizontal(-1) {};
		~Position() {};

		const int GetHorizontal() const
		{
			return horizontal;
		}

		const int GetVertical() const
		{
			return vertical;
		}

		bool operator==(const Position& pos) const
		{
			 return pos.horizontal == this->horizontal
				&& pos.vertical == this->vertical;
		
		}

		bool operator!=(Position pos)
		{
			return pos.horizontal != this->horizontal
				|| pos.vertical != this->vertical;
		}
		 
		bool operator<(const Position& pos) const
		{
			return (pos.vertical * 9 + pos.horizontal)
				< (this->vertical * 9 + this->horizontal);
		}

		Position operator-(const Position& pos) const
		{
			return Position(this->vertical - pos.vertical,
				this->horizontal - pos.horizontal);
		}

		Position operator+(const Position& pos) const
		{
			return Position(this->vertical + pos.vertical,
				this->horizontal + pos.horizontal);
		}

		friend std::ostream& operator<<(std::ostream& os, const Position& pos)
		{
			os << "{" << pos.vertical << "," << pos.horizontal << "}";
			return os;
		}

	protected:
		int vertical;
		int horizontal;
	};

	using Move = std::pair<int, Position>;
}