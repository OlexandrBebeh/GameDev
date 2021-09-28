#include "Controller.hpp"
#include "Parser.hpp"

#include <map>

using namespace controller;

void Controller::Prepare()
{
	m_input = std::make_shared<view::Input>();
	m_output = std::make_shared<view::Output>();
	m_quoridor = std::make_shared<model::Game>();
	m_output->StartMenu();


	while (true)
	{
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
};

void Controller::Start()
{
	while (true)
	{
		auto cur_player = m_quoridor->GetNextPlayer();

		std::pair<int, model::Position> move;

		
		if (cur_player->HasConsoleInput())
		{
			m_input->GetString();
		}
		if (cur_player->HasAILogic())
		{
			Parser
		}
	}
}