#include "Controller.hpp"

using namespace controller;

void Controller::Start()
{
	m_input = std::make_shared<view::Input>();
	m_output = std::make_shared<view::Output>();
	m_quoridor = std::make_shared<model::Game>();

	m_output->StartMenu();

	std::string str = m_input->GetString();

	while (true)
	{
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


	while (true)
	{
		
	}

};