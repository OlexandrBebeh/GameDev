#include "Output.hpp"

using namespace view;

void Output::ShowMessage(std::string str)
{
	std::cout << str << std::endl;
}

void Output::StartMenu()
{
	ShowMessage("Welcome to the Quoridor Console Game!");
	ShowMessage("Please, choice mod:");
	ShowMessage("1. player vs player");
	ShowMessage("2. player vs AI");
	ShowMessage("3. exit");

}

void view::Output::ShowHelp()
{
	ShowMessage("Commands:");
	ShowMessage("1-2-3 - move by figure to position (2,3)");
	ShowMessage("2-5-6 - set vertical partition on position (5,6)");
	ShowMessage("3-5-6 - set horizontal partition on position (5,6)");
	ShowMessage("positions - show field like positions");
	ShowMessage("moves - show possible moves");
	ShowMessage("help - help");
	ShowMessage("exit - end game");
}


void Output::ShowGameState(model::Game* game)
{
	ShowMessage("Field:");
	auto field = game->GetField();
	auto hor_part = game->GetHorizontalPatrtitions();
	auto vert_part = game->GetVerticalPatrtitions();
	auto crost_part = game->GetCrosstPatritions();

	const char hor_line = 196;
	const char sqr = 254;

	char ch;
	for (int i = 0; i < model::FIELD_SIZE; i++)
	{
		for (int j = 0; j < model::FIELD_SIZE; j++)
		{
			if (field[i][j] >= 0)
			{
				std::cout << field[i][j];

			}
			else
			{
				std::cout << sqr;
			}

			if (i < vert_part.size() && j < vert_part[i].size())
			{
				ch = (vert_part[i][j] == 1) ? '|' : ' ';
				std::cout << ch;
			}

		}

		std::cout << std::endl;

		for (int j = 0; j < model::FIELD_SIZE; j++)
		{
			if (i < hor_part.size() && j < hor_part[i].size())
			{
				ch = (hor_part[i][j] == 1) ? hor_line : ' ';
				std::cout << ch;

				if (i < crost_part.size() && j < crost_part[i].size())
				{
					if (crost_part[i][j] == 1)
						std::cout << '|';
					if (crost_part[i][j] == -1)
						std::cout << hor_line;
					if (crost_part[i][j] == 0)
						std::cout << ' ';
				}
				else
				{
					std::cout << ' ';
				}
			}
		}
		std::cout << std::endl;
	}
}

void view::Output::ShowPositions(model::Game* game)
{
	auto field = game->GetField();
	auto hor_part = game->GetHorizontalPatrtitions();
	auto vert_part = game->GetVerticalPatrtitions();
	auto crost_part = game->GetCrosstPatritions();

	auto possible_fig_moves = game->GetPossibleFigureMoves(game->GetCurrentPlayerId());

	const char hor_line = 196;
	const char sqr = 254;
	std::string green = "\x1B[32mTexting\033[0m";
	char ch;
	for (int i = 0; i < model::FIELD_SIZE; i++)
	{
		for (int j = 0; j < model::FIELD_SIZE; j++)
		{

			if (std::find(possible_fig_moves.begin(), 
				possible_fig_moves.end(), 
				model::Position{ i,j }) != possible_fig_moves.end())
			{
				std::cout << "\x1B[32m" << "(" << i << "," << j << ")" << "\033[0m";
			}
			else
			{
				std::cout << "(" << i << "," << j << ")";
			}

			if (i < vert_part.size() && j < vert_part[i].size())
			{
				ch = '|';
				std::cout << ch;
				if (vert_part[i][j] == 1 || vert_part[i][j] == -1)
				{
					std::cout << "\x1B[31m" << "<" << i << "," << j << ">" << "\033[0m";
				}
				else
				{
					std::cout << "<" << i << "," << j << ">";
				}
				std::cout << ch;
			}

		}

		std::cout << std::endl;

		for (int j = 0; j < model::FIELD_SIZE; j++)
		{
			if (i < hor_part.size() && j < hor_part[i].size())
			{
				if (hor_part[i][j] == 1)
				{
					std::cout << "\x1B[31m" << "<" << i << "," << j << ">" << "\033[0m";
				}
				else
				{
					std::cout << "<" << i << "," << j << ">";
				}
				std::cout << "       ";
			}
		}
		std::cout << std::endl;
	}
}

void view::Output::ShowPossibleMoves(model::Game* game)
{
	auto positions = game->GetPossibleFigureMoves(game->GetCurrentPlayerId());

	ShowMessage("Possible Figure moves:");
	for (auto& pos : positions)
	{
		std::cout << pos;
	}
	ShowMessage("");
	positions = game->GetPossibleVerticalPatrtitions();

	ShowMessage("Possible vertical partitions:");
	for (auto& pos : positions)
	{
		std::cout << pos;
	}

	ShowMessage("");

	positions = game->GetPossibleHorizontalPatrtitions();

	ShowMessage("Possible horizontal partitions:");
	for (auto& pos : positions)
	{
		std::cout << pos;
	}
	ShowMessage("");
}
