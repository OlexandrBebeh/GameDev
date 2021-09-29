#pragma once
#include "../View/Input.hpp"
#include "../View/Output.hpp"
#include "../Model/Game/Game.hpp"
#include "../Model/Player/PlayerFactory.hpp"
#include "../Model/Player/AIPlayer/AIPlayer.hpp"
#include "../Model/Player/ConsolePlayer/ConsolePlayer.hpp"


namespace controller
{
	class Controller
	{
	public:
		Controller() {};
		~Controller() {};

		void Prepare();

	protected:
		void Start();

		std::shared_ptr<view::Output> m_output;
		std::shared_ptr<view::Input> m_input;
		std::shared_ptr<model::Game> m_quoridor;

		model::PlayerFactory m_player_factory;
	};
}