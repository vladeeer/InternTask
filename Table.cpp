#include "Table.h"

Table::Table(int tablePrice)
	:name(), timeTaken(), sum(), totalTime()
{
	this->tablePrice = tablePrice;
}

Table::Table(const Table& table)
{
	this->name = table.name;
	this->timeTaken = table.timeTaken;
	this->sum = table.sum;
	this->tablePrice = table.tablePrice;
	this->totalTime = table.totalTime;
}

std::string Table::getName() const
{
	return this->name;
}

int Table::getSum() const
{
	return this->sum;
}

Time Table::getTotalTime() const
{
	return this->totalTime;
}

bool Table::isBusy() const
{
	return !(this->name.empty());
}

void Table::take(std::string name, Time t)
{
	this->name = name;
	this->timeTaken = t;
}

void Table::cashOut(Time curTime)
{
	Time timeBusy = curTime - this->timeTaken;
	this->sum += (timeBusy.hours + ((timeBusy.mins > 0) ? 1 : 0)) * this->tablePrice;
	this->name = "";
	this->totalTime = this->totalTime + timeBusy;
}
