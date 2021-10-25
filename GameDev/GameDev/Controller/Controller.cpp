#include "Controller.hpp"
#include "Parser.hpp"

#include <map>

using namespace controller;

void Controller::Prepare()
{
	m_input    = std::make_shared<view::Input>();
	m_output   = std::make_shared<view::Output>();
	m_quoridor = std::make_shared<model::Game>();

	while (true)
	{
		while (true)
		{
			m_output->StartMenu();

			std::string str = m_input->GetString();

			if (str == "1")
			{
				m_quoridor->AddPlayer(m_player_factory.GetPlayer(0));
				m_quoridor->AddPlayer(m_player_factory.GetPlayer(0));
				break;
			}
			else if (str == "2")
			{
				m_quoridor->AddPlayer(m_player_factory.GetPlayer(0));
				m_quoridor->AddPlayer(m_player_factory.GetPlayer(1));
				break;

			}
			else if (str == "3")
			{
				return;
			}
			else
			{
				m_output->ShowMessage("Invalid input. Try again.");
			}
		}

		Start();

		m_quoridor->ResetGame();
	}
};

void Controller::Start()
{
	m_output->ShowHelp();

	while (true)
	{
		if (m_quoridor->CheckWin() >= 0)
		{
			m_output->ShowMessage("Congrads player:");

			m_output->ShowMessage(std::to_string(m_quoridor->CheckWin()));
			
			return;
		}

		m_output->ShowGameState(m_quoridor.get());
		m_output->ShowPartitions(m_quoridor.get());

		auto cur_player = m_quoridor->GetCurrentPlayer();

		model::Move move;

		if (cur_player->HasConsoleInput())
		{
			m_output->ShowMessage("Please, enter your answer:");

			std::string input;
			input = m_input->GetString();

			if (input == "help")
			{
				m_output->ShowHelp();
				continue;
			}
			else if (input == "moves")
			{
				m_output->ShowPossibleMoves(m_quoridor.get());
				continue;
			}
			else if (input == "positions")
			{
				m_output->ShowPositions(m_quoridor.get());
				continue;
			}
			else if (input == "exit")
			{
				return;
			}
			
			move = Parser::ParseMove(input);
		}
		if (cur_player->HasAILogic())
		{
			move = cur_player->GetMove(m_quoridor.get());
		}

		auto moves = m_quoridor->GetPossibleMoves(m_quoridor->GetCurrentPlayerId());

		if (std::find(moves.begin(),moves.end(),move) != moves.end())
		{
			m_quoridor->MakeMove(move);
		}
		else
		{
			m_output->ShowMessage("Wrong move");
		}
	}
}

