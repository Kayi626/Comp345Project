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
	virtual  Command& getCommand();
    virtual bool validate(Command& com, string state);
	

	//Used to extract names of either players or files
	string extractName(Command& com);

private:
	string readCommand() {
		string input;
		std:getline(std::cin,input);
		return input;
	}
	Command& saveCommand(string com) {
		//Add the newly allocated command object into the list of command
		Command* tempCom = new Command(com, "");
		lc.push_back(tempCom);
		return *tempCom;
	}


};




//Adapter of CommandProcessor

class FileLineReader {
public:
	FileLineReader();
	FileLineReader(string path);
	FileLineReader(const FileLineReader& flr);
	vector<string> readLineFromFile();
private:
	string filePath;

};
//Filecommandprocessor is derived from commandprocessor
class FileCommandProcessorAdapter : public CommandProcessor {
public:
	FileCommandProcessorAdapter();
	FileCommandProcessorAdapter(FileLineReader& flr);
	~FileCommandProcessorAdapter();
	vector<string> readCommand();
	void saveCommand();

private:
	FileLineReader* flr;
};
