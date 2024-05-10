#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<deque>
#include<regex>

#include"Time.h"
#include"Table.h"

bool isEmpty(std::deque<std::string>& userQueue, std::vector<Table>& tables)
{
	if (!userQueue.empty()) return false;
	for (Table& table : tables)
		if (table.getName() != "")
			return false;
	return true;
}

bool isInside(std::string name, std::deque<std::string>& userQueue, std::vector<Table>& tables)
{
	if (!(std::find(userQueue.begin(), userQueue.end(), name) == userQueue.end()))
		return true;
	for (Table& table : tables)
		if (table.getName() == name)
			return true;
	return false;
}

int main(int argc, char* argv[])
{	
	// We should have 2 args
	if (argc != 2) 
		return 1;

	// Open file and make sure it opened
	std::ifstream inputFile(argv[1]);
	//std::ifstream inputFile("test_file4.txt");
	if (!inputFile.good())
		return 1;

	// Header data
	int nTables{};
	Time tOpened{};
	Time tClosed{};
	int pricePerHour{};

	std::streampos eventListPos{};

	// Check format && read header data
	std::regex regexInt("^[0-9]+$");
	std::regex regexTime("^([01][0-9]|2[0-3]):[0-5][0-9]$");
	std::regex regexTime2("^([01][0-9]|2[0-3]):[0-5][0-9] ([01][0-9]|2[0-3]):[0-5][0-9]$");
	std::regex regexEvent("^([01][0-9]|2[0-3]):[0-5][0-9] [1-4] [a-z0-9_\\-]+( [1-9]([0-9]+)?)?$");
	try
	{
		// Check & read num of tables
		std::string line;
		if (!std::getline(inputFile, line)) throw (line);
		if (!std::regex_match(line, regexInt)) throw (line);
		nTables = std::stoi(line);

		// Check & read open/close time
		if (!std::getline(inputFile, line)) throw (line);
		if (!std::regex_match(line, regexTime2)) throw (line);
		tOpened.from_str(line.substr(0, 5));
		tClosed.from_str(line.substr(6, 5));

		// Check & read price
		if (!std::getline(inputFile, line)) throw (line);
		if (!std::regex_match(line, regexInt)) throw (line);
		pricePerHour = std::stoi(line);

		// Save for ease of future read
		eventListPos = inputFile.tellg();

		// Check events (time must not go backward)
		Time lastTime;
		while (std::getline(inputFile, line))
		{
			//std::istringstream iss(line);
			if (!std::regex_match(line, regexEvent)) throw (line);
			if (Time(line.substr(0, 5)) < lastTime) throw (line);
			lastTime.from_str(line.substr(0, 5));
		}
	}
	catch (std::string& line)
	{
		// If input error is caught, print last line & halt
		std::cout << line << std::endl;
		return 1;
	}

	// Print open time
	std::cout << tOpened.to_str() << std::endl;

	// Return file cursor to event list
	inputFile.clear();
	inputFile.seekg(eventListPos);

	// Vars for parsing
	std::deque<std::string> userQueue{};
	std::vector<Table> tables(nTables, pricePerHour);
	std::string eventTimeString{};
	Time eventTime{};
	int eventId{};
	std::string eventName{};
	int eventTableId{};
	bool lateComersLeft = false;

	// Parse lines one by one
	std::string line;
	while (std::getline(inputFile, line))
	{
		// Use sstream for ease of convertion
		std::istringstream iss(line);
		iss >> eventTimeString;
		eventTime.from_str(eventTimeString);
		iss >> eventId;
		iss >> eventName;
		if (!iss.eof())
			iss >> eventTableId;
		
		// Kick everyone out first, then finish parsing
		if (tClosed < eventTime)
		{
			lateComersLeft = true;
			break;
		}

		// Copy next event line to console
		std::cout << line << std::endl;

		// Do what user wishes
		try 
		{
			switch (eventId)
			{
			// User arrived
			case 1:
				if (isInside(eventName, userQueue, tables)) throw(std::string("YouShallNotPass"));
				if (eventTime < tOpened) throw(std::string("NotOpenYet"));
				userQueue.push_back(eventName);
				break;
			
			// User took the table
			case 2:
				if (tables[eventTableId - 1].isBusy()) throw(std::string("PlaceIsBusy"));
				if (!isInside(eventName, userQueue, tables)) throw(std::string("ClientUnknown"));
				for (Table& table : tables)
					if (table.getName() == eventName)
						table.cashOut(eventTime);
				tables[eventTableId - 1].take(eventName, eventTime);
				if (std::find(userQueue.begin(), userQueue.end(), eventName) != userQueue.end())
					userQueue.erase(std::find(userQueue.begin(), userQueue.end(), eventName));
				break;
			
			// User's waiting
			case 3:
				for (Table& table : tables)
					if (!table.isBusy())
						throw(std::string("ICanWaitNoLonger!"));
				if (userQueue.size() > nTables)
				{
					userQueue.erase(std::find(userQueue.begin(), userQueue.end(), eventName));
					std::cout << eventTimeString << " 11 " << eventName << std::endl;
				}
				break;

			// User left;
			case 4:
				if (!isInside(eventName, userQueue, tables)) throw(std::string("ClientUnknown"));
				for (int i = 0; i < nTables; i++)
					if (tables[i].getName() == eventName)
					{
						tables[i].cashOut(eventTime);
						if (!userQueue.empty())
						{
							std::cout << eventTimeString << " 12 " << userQueue.front() << " "
								<< i + 1 << std::endl;
							tables[i].take(userQueue.front(), eventTime);
							userQueue.pop_front();
						}
					}
				if (std::find(userQueue.begin(), userQueue.end(), eventName) != userQueue.end())
					userQueue.erase(std::find(userQueue.begin(), userQueue.end(), eventName));
				break;
			}
		}
		catch (std::string& error)
		{
			std::cout << eventTimeString << " 13 " << error << std::endl;
		}

	}

	// Get everybody out in alphabetic order (no need to delete tho, just print)
	if (!isEmpty(userQueue, tables))
	{
		std::vector<std::string> names(userQueue.size());
		int queueSize = userQueue.size();
		for (int i = 0; i < queueSize; i++)
		{
			names[i] = userQueue.front();
			userQueue.pop_front();
		}
		for (int i = 0; i < nTables; i++)
		{
			if (tables[i].getName() != "")
			{
				names.push_back(tables[i].getName());
				tables[i].cashOut(tClosed);
			}
		}
		std::sort(names.begin(), names.end());
		for (int i = 0; i < names.size(); i++)
			std::cout << tClosed.to_str() << " 11 " << names[i] << std::endl;
	}
	
	// Turn around everybody who comes late
	if (lateComersLeft)
	{
		do
		{
			// Copy next event line to console
			std::cout << line << std::endl;

			// Use sstream for ease of convertion
			std::istringstream iss(line);
			iss >> eventTimeString;
			iss >> eventId;

			if (eventId == 1)
				std::cout << eventTimeString << " 13 NotOpenYet" << std::endl;

		} while (std::getline(inputFile, line));
	}

	// Print close time
	std::cout << tClosed.to_str() << std::endl;

	// Print table sums
	for (int i = 0; i < nTables; i++)
	{
		std::cout << i + 1 << " " << tables[i].getSum() << " " 
			<< tables[i].getTotalTime().to_str() << std::endl;
	}

	return 0;
}