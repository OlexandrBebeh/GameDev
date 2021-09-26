#pragma once
#include "../DataTypes/Position.hpp"

namespace model
{
	enum class PlayerType
	{
		no_player = -1,
		console_player = 0,
		ai_player = 1
	};
	class Player
	{
	public:
		Player();
		~Player();

		void SetPosition(Position p);
		void SetPartitionsAmount(int p);
		Position GetPosition() const;
		int GetPartitionsAmount() const;

		PlayerType GetPlayerType() const;

	protected:
		Position m_pos;

		int m_partitions_amount{10};

		PlayerType m_player_type{ PlayerType::no_player };
	};
}