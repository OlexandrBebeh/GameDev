#pragma once
#include "../DataTypes/Position.hpp"
#include "../Game/Field.hpp"

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
		void SetStartPosition(Position p);
		Position GetStartPosition();

		void SetPartitionsAmount(int p);
		Position GetPosition() const;
		int GetPartitionsAmount() const;
		void SetPlayerWinRow(int row)
		{
			m_player_win_row = row;
		}
		void SetField(std::shared_ptr<Field> field)
		{
			m_field = field;
		}
		int GetPlayerWinRow() const
		{
			return m_player_win_row;
		}
		PlayerType GetPlayerType() const;

		virtual bool HasConsoleInput() const;
		virtual bool HasAILogic() const;

		Move GetMove() const;

	protected:
		Position m_pos;
		Position m_start_pos;
		int m_player_win_row;
		int m_partitions_amount{10};

		PlayerType m_player_type{ PlayerType::no_player };

		std::shared_ptr<Field> m_field;
	};
}