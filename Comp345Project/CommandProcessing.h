#pragma once

#include <iostream>
#include <cstring>
#include<list>
#include<vector>
#include <regex>

using namespace std;

class Command {

public:
	
	//Constructors
	Command();
	Command(string com, string effect = "");
	Command(const Command& com);
	Command& operator =(const Command& com);
	friend ostream& operator<<(ostream& ost, const Command& com);
	string getCommand();
	string getEffect();
	void saveEffect(string effect);
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
	CommandProcessor(const CommandProcessor& comP);
	virtual ~CommandProcessor();
	CommandProcessor& operator =(const CommandProcessor& comP);
	virtual  Command* getCommand();
    virtual bool validate(Command& com, string state);
	friend ostream& operator<<(ostream& ost, const CommandProcessor& comP);
    
	string extractName(Command& com);
	void clearMemory();
	

private:
	string readCommand() {
		string input;
	    std:getline(std::cin,input);
		/*If the input is in the format of "-console commands", it will extract 'commands' automatically.
		if (std::regex_match(input, std::regex("-console\\s+(.*)"))){
			input = extractLineArgumentCommand(input);
		}
		//If the input in the format of "-file <filename>", it will do nothing(pass "-file <filename> " without extracting)*/
		return input;
	}
	Command* saveCommand(string str) {
		//Add the newly allocated command object into the list of command
		Command* tempCom = new Command(str, "");
		/*Check if the argument string is in the format of "-file <filename>"
		if (std::regex_match(str, std::regex("-file\\s+<(.*)>"))) {
			tempCom->saveEffect("FileCommand");
		}*/
		lc.push_back(tempCom);
		return tempCom;
	}


};




//Adapter of CommandProcessor

class FileLineReader {
public:
	FileLineReader();
	FileLineReader(string path);
	FileLineReader(const FileLineReader& flr);
	~FileLineReader();
	string readLineFromFile();
	std::fstream input;
private:
	string filePath;	

};
//Filecommandprocessor is derived from commandprocessor
class FileCommandProcessorAdapter : public CommandProcessor {
public:
	FileCommandProcessorAdapter();
	FileCommandProcessorAdapter(FileLineReader* flr);
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcomP);
	~FileCommandProcessorAdapter();
	string readCommand();
	vector<string> readAllCommands();
	list<Command*> saveAllCommands();
	Command* saveCommand();
	Command* getCommand();

private:
	FileLineReader* flr;
};
