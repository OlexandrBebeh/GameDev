#pragma once
#include <utility>

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

	protected:
		int horizontal;
		int vertical;
	};

	using Move = std::pair<int, Position>;
}