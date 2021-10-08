#pragma once

#include <map>
#include <iostream>
#include "../Player/Player.hpp"
#include "Field.hpp"


namespace model
{
	class Game
	{
	public:
		Game();
		~Game();
		Player* GetCurrentPlayer() const;
		void AddPlayer(Player* player);
		
		int GetCurrentPlayerId() const;

		void MakeFigureMove(Position);

		void MakeMove(model::Move move);
		std::vector<Position> GetPossibleFigureMoves(int player_id);

		std::vector<std::vector<int>> GetField() const;
		std::vector<std::vector<int>> GetVerticalPatrtitions() const;
		std::vector<std::vector<int>> GetHorizontalPatrtitions() const;
		std::vector<std::vector<int>> GetCrosstPatritions() const;
		std::vector<Position> GetPossibleVerticalPatrtitions() const;
		std::vector<Position> GetPossibleHorizontalPatrtitions() const;
		std::vector<Position> CheckPossibleVerrticalPartitions();
		std::vector<Position> CheckPossibleHorizontalPartitions();
		bool IsGameEnd();
		std::vector<Move> GetPossibleMoves();
		std::vector<Move> GetPossibleMoves(int player);
		void NextPlayer();

		void Test();

		int CheckWin();

		void ResetGame();
	protected:
		std::map<int, std::shared_ptr<Player>> m_players;

		std::shared_ptr<Field> m_field;

		int m_players_amount{ 0 };

		int m_current_player{ 0 };
		std::vector<Position> m_player_start_pos;
	};
}