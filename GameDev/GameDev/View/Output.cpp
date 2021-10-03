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


void Output::ShowGameState(model::Game* game)
{
	auto field = game->GetField();
	auto hor_part = game->GetHorizontalPatrtitions();
	auto vert_part = game->GetVerticalPatrtitions();
	auto crost_part = game->GetCrosstPatritions();

	const char hor_line = 196;
	const char sqr = 254;

	char ch;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
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

		for (int j = 0; j < FIELD_SIZE; j++)
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
