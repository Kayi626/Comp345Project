#pragma once

#include <iostream>
#include <cstring>
#include<list>
#include<vector>

using namespace std;

class Command {

public:
	void saveEffect(string effect);
	//Constructors
	Command();
	Command(string com, string effect = "");
	Command(const Command& com);
	Command& operator =(const Command& com);
	friend ostream& operator<<(ostream& ost, const Command& com);
	string getCommand();
	string getEffect();
private:
	//variables to store command and effect info
	string command;
	string effect;

};
class CommandProcessor {

public:
	list<Command*> lc;
	CommandProcessor();
	CommandProcessor(const list<Command*>& lc);
	virtual ~CommandProcessor();
	CommandProcessor& operator =(const CommandProcessor& comP);
	virtual string getCommand();
    virtual bool validate(Command& com, string state);
	

	//Used to extract names of either players or files
	string extractName();

private:
	string readCommand() {
		string input;
		std::cin >> input;
		return input;
	}
	void saveCommand(string com) {
		//Add the newly allocated command object into the list of command
		Command* tempCom = new Command(com, "");
		lc.push_back(tempCom);
	}


};




//Adapter of CommandProcessor

class FileLineReader {
public:
	vector<string> readLineFromFile(string path);

};
//Filecommandprocessor is derived from commandprocessor
class FileCommandProcessorAdapter : public CommandProcessor {
public:
	FileCommandProcessorAdapter();
	~FileCommandProcessorAdapter();
	vector<string> readCommand(string path);
	void saveCommand(vector<string> com);
private:
	FileLineReader* flr;
};
