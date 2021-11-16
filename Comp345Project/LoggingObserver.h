#pragma once
#include <string>
#include <list>
#include <fstream>
#include <iostream>

class ILoggable {
public:
	ILoggable();
	virtual ~ILoggable();
	virtual std::string stringToLog() = 0;
};

class Observer {
public:
	Observer();
	virtual ~Observer();
	virtual void Update(ILoggable *) = 0;
};

class LogObserver: public Observer {
public:
	LogObserver();
	~LogObserver();
	void Update(ILoggable *ilog);
};

class Subject {
protected:
	std::list<Observer *> observers;
public:
	Subject();
	~Subject();
	void attach(Observer *o);
	void detach(Observer *o);
	void notify(ILoggable *ilog);
};
