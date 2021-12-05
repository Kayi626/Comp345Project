#pragma once
#include <string>
#include <list>
#include <fstream>
#include <iostream>

// Generic abstract observer in obsever pattern
class ILoggable {
public:
	ILoggable();
	virtual ~ILoggable();
	// a pure virtual method, will have important content.
	virtual std::string stringToLog() = 0;
};

// Generic abstract observer in obsever pattern
class Observer {
public:
	Observer();
	virtual ~Observer();
	virtual void Update(ILoggable *) = 0;
};

// Implemented Observer: responsible to print ILoggable content into the gamelog.txt
class LogObserver: public Observer {
public:
	LogObserver();
	~LogObserver();
	// print ILoggable content into file
	void Update(ILoggable *ilog);
};

// Generic Subject in obsever pattern
class Subject {
protected:
	std::list<Observer *> observers;
public:
	Subject();
	~Subject();
	// attach a observer to a list of observer: one subject could have muliple target to notify
	void attach(Observer *o);
	void detach(Observer *o);
	// notify observers to update with provided ILoggable content
	void notify(ILoggable *ilog);
};
