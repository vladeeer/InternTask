#pragma once
#include<string>
#include"Time.h"

class Table
{
public:
	Table(int tablePrice);
	Table(const Table& table);

private:
	std::string name;
	Time timeTaken;
	int sum;
	int tablePrice;
	Time totalTime;

public:
	std::string getName() const;
	int getSum() const;
	Time getTotalTime() const;
	bool isBusy() const;
	void take(std::string name, Time t);
	void cashOut(Time curTime);
};

