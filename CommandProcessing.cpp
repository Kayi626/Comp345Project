#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <regex>
using namespace std;

#include "CommandProcessing.h"

//*******************************CommandProcessor***********************************
CommandProcessor::CommandProcessor() {
	lc = list<Command*>();
}

CommandProcessor::CommandProcessor(const list<Command*>& lc) {
	
	for (list<Command*>::iterator it = this->lc.begin(); it != this->lc.end(); ++it) {
		this->lc.push_back(new Command(**it));
	}
}

CommandProcessor::CommandProcessor(const CommandProcessor& comP) {
	list<Command*> tempL = comP.lc;
	for (list<Command*>::iterator it = tempL.begin(); it != tempL.end(); ++it) {
		Command* tempCom = new Command(**it);
		lc.push_back(tempCom);
	}
	
}

CommandProcessor::~CommandProcessor() {
	for (Command* element:lc) {
		delete element;
	}
}

CommandProcessor& CommandProcessor::operator =(const CommandProcessor& comP) {
	if (this == &comP)
		return *this;
	list<Command*> tempL = comP.lc;
	for (list<Command*>::iterator it = tempL.begin(); it != tempL.end(); ++it) {
		Command* tempCom = new Command(**it);
		lc.push_back(tempCom);
	}

	return *this;
}

ostream& operator<<(ostream& ost, const Command& com) {

	ost << "Command: " << com.originCommand << "; Effect: " << com.effect << endl;
	return ost;

}
ostream& operator<<(ostream& ost, const CommandProcessor& comP){
	int counter = 1;
	ost << endl <<comP.lc.front()->getOriginalCommand();
	for (Command* com : comP.lc) {
		ost << counter << ". " << *com << endl;
	}
	return ost;
}

//Validate the current command given a state and record the effect into the command object
bool CommandProcessor::validate(Command& com, int state) {
	bool status = true;
	string FirstArg = (com.getArgs())[0];
	switch (state)
	{
	case 1: {
		return (FirstArg.compare("loadmap") == 0);
	}
	case 2: {
		return (FirstArg.compare("loadmap") == 0 || FirstArg.compare("validatemap") == 0);
	}
	case 3: {
		return (FirstArg.compare("addplayer") == 0 );
	}
	case 4: {
		return (FirstArg.compare("addplayer") == 0 || FirstArg.compare("start") == 0);
	}
	case 6: {
		return (FirstArg.compare("issueorder") == 0 || FirstArg.compare("endissueorder") == 0);
	}
	case 8: {
		return (FirstArg.compare("replay") == 0 || FirstArg.compare("quit") == 0);
	}
	default:
		return false;
	}


	
	return status;

}

//Used to extract the string in <>
// REMOVED when working on part2&3. 

//string CommandProcessor::extractName(Command& com) {
//
//	const std::string s = com.getOriginalCommand();
//	//Two patterns(addplayer + loadmap). It will extract the substring from <>.
//	std::string string1 = "loadmap <";
//	std::string string2 = "addplayer <";
//	std::regex rgx("(" + string1 + "|" + string2 + ")(.*)>");
//	std::smatch match;
//
//	if (std::regex_search(s.begin(), s.end(), match, rgx))
//		return match[2];
//	else
//		return "";
//}

void CommandProcessor::clearMemory() {
	for (Command* com : lc) {
		delete com;
	}
}


Command* CommandProcessor::getCommand() {
	string input = readCommand();
	Command* com = saveCommand(input);
	return com;
}



//*********************************************************************************

//************************************Command**************************************
void Command::saveEffect(string effect) {
	this->effect = effect;
}
Command::Command() {
	this->originCommand = "";
	this->SaveStringCommandInToArgs("");
	effect = "";
}
Command::Command(string com,string effect) {
	this->originCommand = com;
	this->SaveStringCommandInToArgs(this->originCommand);
	this->effect = effect;
}
Command::Command(const Command& com) {
	this->originCommand = com.originCommand;
	this->SaveStringCommandInToArgs(this->originCommand);
	this->effect = com.effect;
}
Command& Command::operator =(const Command& com) {
	if (this == &com)
		return *this;
    
	this->originCommand = com.originCommand;
	this->SaveStringCommandInToArgs(this->originCommand);
	this->effect = com.effect;

	return *this;
}

void Command::SaveStringCommandInToArgs(const string& command) {
	int findIndex,currentStartIndex = 0;
	findIndex = command.find(" ");
	if (findIndex == string::npos) {
		//if there's no " " in command - which means it's a one argument command
		args.push_back(command);
	}
	else {
		bool notReachEnd = true;
		while (notReachEnd) {
			//if we find a " ", telling this command have mutliple lines.
			args.push_back(command.substr(currentStartIndex, findIndex - currentStartIndex));
			//push the first argument in. update the index counter.
			currentStartIndex = findIndex + 1;
			findIndex = command.find(" ", currentStartIndex);
			//stop when reach the end - no more arguments will be added and stop.
			if (findIndex == string::npos) notReachEnd = false;
		}
		args.push_back(command.substr(currentStartIndex, findIndex - currentStartIndex));

	}
	while (args.size() < 4) {
		args.push_back("");
		//add arguments to the vector and make sure the command have at least 4 arguments.
	}
}

vector<string> Command::getArgs() {
	return args;
}
string Command::getOriginalCommand() {
	return originCommand;
}

string Command::getEffect() {
	return effect;
}
//********************************************************************************


//**********************FileCommandProcessorAdapter*******************************

FileCommandProcessorAdapter::FileCommandProcessorAdapter(): flr(){}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader* target) {
	flr = target;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcomP): CommandProcessor(fcomP){

	flr = new FileLineReader(*(fcomP.flr));

}
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
}

Command* FileCommandProcessorAdapter::getCommand() {
	return  saveCommand();
}

//It returns a command object with the command extracted from the saved file. If the flr object of FileCommandProcessorAdapter reaches 
//the end of the file or the saved file is empty, it will return a nullptr.
Command* FileCommandProcessorAdapter::saveCommand() {
	string com = readCommand();
	Command* tempCom = nullptr;
	//No commands read from the . txt file
	//Store the input strings in the command objects and push the objects into the list of commands
	if (com.compare("") != 0) {
		tempCom = new Command(com, "");
		lc.push_back(tempCom);
	}
	return tempCom;
}

string FileCommandProcessorAdapter::readCommand() {
	string inputFromFile = flr->readLineFromFile();
	if (inputFromFile.compare("")==0) {
		//if there's no command line from the file - ask the user to enter command instead:
		string input;
		std:getline(std::cin, input);
		return input;
	}
	return inputFromFile;
}



vector<string> FileCommandProcessorAdapter::readAllCommands() {
	vector<string> vec(5);
	string line;
	line = flr->readLineFromFile();
	if (line.compare("") != 0) {
		vec.push_back(line);
	}
	while (!flr->input.eof()) {
		line = flr->readLineFromFile();	
		vec.push_back(line);
	}
	return vec;

}
list<Command*> FileCommandProcessorAdapter::saveAllCommands() {
	vector<string> vec = readAllCommands();
	for (string s : vec) {
		lc.push_back(new Command(s,""));
	}
	return lc;
}




//********************************************************************************


//**************************FileReader********************************************
FileLineReader::FileLineReader() {
	filePath = "";
}

FileLineReader::FileLineReader(const FileLineReader& flr) {
	this->filePath = string(flr.filePath);
}
FileLineReader::FileLineReader(string path):filePath(path) {
	input.open(path, ios::in);
	if (!input.is_open())
		throw std::exception("file not found");
}
FileLineReader::~FileLineReader() {
	input.close();
}

string FileLineReader::readLineFromFile() {
	string line="";
	//Read lines from the input file
	if (!input.eof()) {
		getline(input,line);
		return line;
	}

	return line;

}
//********************************************************************************