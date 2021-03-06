#include "LoadingUpgrades.h"

void LoadingUpgrades::ReadFile(string fileName)
{
	ifstream myFile(fileName);

	if (!myFile.is_open())
	{
		cout << "Unable to open file." << endl;
		return;
	}

	while (myFile.good())
	{
		getline(myFile, tempString);
		string::size_type begin = tempString.find_first_not_of(" \f\t\v");

		// Skip empty lines
		if (begin == string::npos)
			continue;

		// Found option line <setting name>=value
		else if (string("<").find(tempString[begin]) != string::npos)
		{
			for (int i = 0; i < tempString.size(); i++)
			{
				if (tempString[i] == '=')
				{
					//Speed upgrades
					if (tempString.substr(0, i) == "<Speed>")
						speed_upgrade = stoi(tempString.substr(i + 1));
					//Speed multiplyer upgrades
					if (tempString.substr(0, i) == "<Speed_Multiplyer>")
						speed_multiplyer_upgrade = stoi(tempString.substr(i + 1));
				}
			}
		}
	}
}

void LoadingUpgrades::DeleteTextFile(string fileName)
{
	ofstream output;
	output.open(fileName);

	//Replace text with blank space
	output << endl;

	output.close();
}

void LoadingUpgrades::WriteFile(string fileName, string type, int number)
{
	ofstream output;
	output.open(fileName);

	if (type == "Speed")
	{
		output << "<Speed>=" << number << endl;
	}
	else if (type == "Speed_Multiplyer")
	{
		output << "<Speed_Multiplyer>=" << number << endl;
	}

	output.close();
}