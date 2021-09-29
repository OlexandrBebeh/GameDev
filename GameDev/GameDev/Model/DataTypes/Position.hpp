#pragma once
#include <utility>

namespace model
{

	class Position
	{
	public:
		Position(int _x, int _y) : x(_x), y(_y) {};
		Position() : x(-1), y(-1) {};
		~Position() {};

		int& GetX()
		{
			return x;
		}

		int& GetY()
		{
			return y;
		}

	protected:
		int x;
		int y;
	};

	using Move = std::pair<int, Position>;
}