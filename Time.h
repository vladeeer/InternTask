#pragma once
#include<string>

class Time
{
public:
	Time();
	Time(const Time& t);
	Time(int hours, int mins);
	Time(std::string s);

public:
	std::string to_str() const;
	void from_str(std::string s);

public:
	bool operator<(const Time& t) const;
	const Time& operator-(const Time& t) const;
	const Time& operator+(const Time& t) const;

public:
	int hours;
	int mins;
};

