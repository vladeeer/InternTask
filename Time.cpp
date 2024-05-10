#include "Time.h"

Time::Time()
	:hours(), mins()
{
}

Time::Time(const Time& t)
{
	this->hours = t.hours;
	this->mins = t.mins;
}

Time::Time(int hours, int mins)
{
	this->hours = hours;
	this->mins = mins;
}

Time::Time(std::string s)
{
	this->hours = std::stoi(s.substr(0, 2));
	this->mins = std::stoi(s.substr(3, 2));
}

std::string Time::to_str() const
{
	return ((this->hours < 10) ? "0" : "") + std::to_string(this->hours) + ":"
		+ ((this->mins < 10) ? "0" : "") + std::to_string(this->mins);
}

void Time::from_str(std::string s)
{
	this->hours = std::stoi(s.substr(0, 2));
	this->mins = std::stoi(s.substr(3, 2));
}

bool Time::operator<(const Time& t) const
{
	return (t.hours > this->hours) || (t.hours == this->hours && t.mins > this->mins);
}

const Time& Time::operator-(const Time& t) const
{
	Time res{};
	res.hours = this->hours - t.hours;
	if (this->mins >= t.mins)
		res.mins = this->mins - t.mins;
	else
	{
		res.hours -= 1;
		res.mins = 60 + this->mins - t.mins;
	}
	return res;
}

const Time& Time::operator+(const Time& t) const
{
	Time res{};
	res.hours = this->hours + t.hours;
	res.mins = this->mins + t.mins;
	if (res.mins >= 60)
	{
		res.mins -= 60;
		res.hours += 1;
	}
	return res;
}