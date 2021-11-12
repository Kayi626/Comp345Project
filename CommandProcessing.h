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

	vector<string> getArgs();
	//return the private member 'args'. which is the arguments from the command.
	string getOriginalCommand();

	string getEffect();
	void saveEffect(string effect);
private:
	//variables to store command and effect info
	vector<string> args;
	//the arguments from the command.
	string originCommand;
	string effect;
	void SaveStringCommandInToArgs(const string& com);

};
class CommandProcessor {

public:
	list<Command*> lc;
	CommandProcessor();
	CommandProcessor(const list<Command*>& lc);
	CommandProcessor(const CommandProcessor& comP);
	virtual ~CommandProcessor();
	CommandProcessor& operator =(const CommandProcessor& comP);
	friend ostream& operator<<(ostream& ost, const CommandProcessor& comP);
    
	void clearMemory();

	virtual Command* getCommand();
	virtual bool validate(Command& com, int state);
	/*	Commands & their available states: 
	* 
	*	[during startup]
	*	loadmap <mapfile>		start, maploaded
	*	validatemap				maploaded
	*	addplayer <playername>	mapvalidated, playersadded
	*	gamestart				playersadded
	*	
	*	[during player]
	*	issueorder <ordertype> [TargetTerrtory] [numberOfArmies] [FromTerrtory]		issue orders
	*	endissueorder		issue orders
	*	deleteorder <orderID>...			issue orders   //to-do? in Assignment3
	*	use_card <>...			issue orders   //to-do? in Assignment3
	*	replay					win
	*	quit					win
	*/


private:
	string readCommand() {
		//这里是要写在.h文件里的吗？检查时备忘：记得尝试搬到.cpp里
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
		//这里是要写在.h文件里的吗？检查时备忘：记得尝试搬到.cpp里
		// 
// 
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

	Command* getCommand();
	string readCommand();
	Command* saveCommand();

	vector<string> readAllCommands();
	list<Command*> saveAllCommands();
private:
	FileLineReader* flr;
};
