#pragma once

#include <iostream>

namespace view
{
	class Output
	{
	public:
		Output() {};
		~Output() {};

		void ShowMessage(std::string);

		void StartMenu();
	};
}