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
	this->lc = list<Command*>();
}

void CommandProcessor::init(const list<Command*>& lc) {
	for (Command* cmd : lc) {
		Command* tempCom = new Command(*cmd);
		for (auto obs : this->observers)
			tempCom->attach(obs);
		this->lc.push_back(tempCom);
	}
}

CommandProcessor::CommandProcessor(const list<Command*>& lc) {
	this->init(lc);
}

CommandProcessor::CommandProcessor(const CommandProcessor& comP) {
	this->init(comP.lc);
}

CommandProcessor::~CommandProcessor() {
	for (Command* element : lc) {
		delete element;
	}
}

CommandProcessor& CommandProcessor::operator =(const CommandProcessor& comP) {
	if (this == &comP)
		return *this;
	for (Command* cmd : this->lc) delete cmd;
	this->lc.clear();
	this->init(comP.lc);
	return *this;
}

ostream& operator<<(ostream& ost, const Command& com) {

	ost << "Command: " << com.originCommand << "; Effect: " << com.effect << endl;
	return ost;

}
ostream& operator<<(ostream& ost, const CommandProcessor& comP) {
	int counter = 1;
	ost << endl << comP.lc.front()->getOriginalCommand();
	for (Command* com : comP.lc) {
		ost << counter << ". " << *com << endl;
	}
	return ost;
}

string CommandProcessor::readCommand() {
	string input;
std:getline(std::cin, input);
	/*If the input is in the format of "-console commands", it will extract 'commands' automatically.
	if (std::regex_match(input, std::regex("-console\\s+(.*)"))){
		input = extractLineArgumentCommand(input);
	}
	//If the input in the format of "-file <filename>", it will do nothing(pass "-file <filename> " without extracting)*/
	return input;
}
Command* CommandProcessor::saveCommand(string str) {
	//Add the newly allocated command object into the list of command
	if (str == "") return nullptr;
	Command* tempCom = new Command(str, "");
	for (auto obs : this->observers)
		tempCom->attach(obs);
	/*Check if the argument string is in the format of "-file <filename>"
	if (std::regex_match(str, std::regex("-file\\s+<(.*)>"))) {
		tempCom->saveEffect("FileCommand");
	}*/
	lc.push_back(tempCom);
	notify(this);
	return tempCom;
}
//Validate the current command given a state and record the effect into the command object
bool CommandProcessor::validate(Command& com, int state) {
	string FirstArg = (com.getArgs())[0];
	switch (state) {
	case 1:
		return FirstArg == "loadmap";
	case 2: {
		return (FirstArg.compare("loadmap") == 0 || FirstArg.compare("validatemap") == 0);
	}
	case 3: {
		return (FirstArg.compare("addplayer") == 0);
	}
	case 4: {
		return (FirstArg.compare("addplayer") == 0 || FirstArg.compare("start") == 0);
	}
	case 6: {
		if (FirstArg.compare("endissueorder") == 0) {
			return true;
		}
		if (FirstArg.compare("issueorder") == 0) {
			if (!isNumber((com.getArgs())[1]) || !isNumber((com.getArgs())[2])) return false;
			if ((com.getArgs())[1].compare("2") != 0  || (com.getArgs())[1].compare("3") != 0){
				//if this command is not blockedade order or bomb order, testing the 3nd argument
				if (!isNumber((com.getArgs())[3])) return false;
				if ((com.getArgs())[1].compare("0") != 0) {
					//if this command is not a deploy order, testing the 4rd argument.
					if (!isNumber((com.getArgs())[4])) return false;
				}
			}
			return true;
		}
		return false;

	}
	case 8: {
		return (FirstArg.compare("replay") == 0 || FirstArg.compare("quit") == 0);
	}
	default:
		return false;
	}
}
bool CommandProcessor::isNumber(string &input) {
	return std::all_of(input.begin(), input.end(), ::isdigit); 
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


std::string CommandProcessor::stringToLog() { return "Command: [" + lc.back()->getOriginalCommand() + "]" ; };


//*********************************************************************************

//************************************Command**************************************
void Command::saveEffect(string effect) {
	this->effect = effect;
	notify(this);
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


std::string Command::stringToLog() { return "Command's Effect: [" + effect + "]"; };

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
	//No commands read from the . txt file
	//Store the input strings in the command objects and push the objects into the list of commands
	return this->CommandProcessor::saveCommand(com);
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
		Command *tempCom = new Command(s, "");
		for (auto obs : this->observers)
			tempCom->attach(obs);
		lc.push_back(tempCom);
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