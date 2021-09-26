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
