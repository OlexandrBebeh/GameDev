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

		int& GetHorizontal()
		{
			return horizontal;
		}

		int& GetVertical()
		{
			return vertical;
		}

		bool operator==(Position pos)
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

		friend std::ostream& operator<<(std::ostream& os, const Position& pos)
		{
			os << "{" << pos.vertical << "," << pos.horizontal << "}";
		}

	protected:
		int vertical;
		int horizontal;
	};

	using Move = std::pair<int, Position>;
}