#include "LoggingObserver.h"

ILoggable::ILoggable() {};
ILoggable::~ILoggable() {};
Observer::Observer() {};
Observer::~Observer() {};
LogObserver::LogObserver() {};
LogObserver::~LogObserver() {};
Subject::Subject() {};
Subject::~Subject() {};


void LogObserver::Update(ILoggable *ilog) {
	//std::cout << ilog->stringToLog() << std::endl;
	std::ofstream logFile("gamelog.txt", std::ios::app);
	logFile << ilog->stringToLog() << std::endl;
	logFile.close();
};

void Subject::attach(Observer *o) {
	observers.push_back(o);
};
void Subject::detach(Observer *o) {
	observers.remove(o);
};
void Subject::notify(ILoggable *ilog) {
	for (auto i : observers)
		i->Update(ilog);
};
