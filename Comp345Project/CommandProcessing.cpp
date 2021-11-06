#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
using namespace std;

#include "CommandProcessing.h"

//*******************************CommandProcessor***********************************
CommandProcessor::CommandProcessor() {
	lc = list<Command*>(10);
}


CommandProcessor::CommandProcessor(const list<Command*>& lc) {
	
	for (list<Command*>::iterator it = this->lc.begin(); it != this->lc.end(); ++it) {
		this->lc.push_back(new Command(**it));
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
	for (list<Command*>::iterator it = lc.begin(); it != lc.end(); ++it) {
		Command* tempCom = new Command(**it);
		lc.push_back(tempCom);
	}

	return *this;
}

Command* CommandProcessor::getCommand() {
	string input = readCommand();
	Command* com = saveCommand(input);
	return com;
}

ostream& operator<<(ostream& ost, const CommandProcessor& comP){
	int counter = 1;
	for (Command* com : comP.lc) {
		ost << counter << ". " << *com << endl;
	}
	return ost;
}

//Validate the current command given a state and record the effect into the command object
bool CommandProcessor::validate(Command& com, string state) {
	bool status = true;
	string input = com.getCommand();
	CommandProcessor comP=CommandProcessor();
	if (std::regex_match(input,std::regex("loadmap <(.*)>")) && (state.compare("start") == 0 || state.compare("maploaded") == 0)) {
		com.saveEffect("maploaded: "+comP.extractName(com));
	}
	else if ((input.compare("validatemap")==0) && (state.compare("maploaded") == 0)) {
		com.saveEffect("mapvalidated");
	}
	else if (std::regex_match(input, std::regex("addplayer <(.*)>")) && (state.compare("mapvalidated") == 0 || state.compare("playersadded") == 0)) {
		com.saveEffect("playersadded: "+comP.extractName(com));
	}
	else if ((input.compare("gamestart") == 0) && (state.compare("playersadded") == 0)){
		com.saveEffect("assignreinforcement");
	}
	else if ((input.compare("replay") == 0) && (state.compare("win") == 0)) {
		com.saveEffect("start");
	}
	else if ((input.compare("quit") == 0) && (state.compare("win"))) {
		com.saveEffect("exit program");
	}
	else {

		string errorMessage = "Rejected: Command [ " + com.getCommand() + "] is invalid in State [ " + state + " ]";
		com.saveEffect(errorMessage);
		status = false;
	}

	return status;

}

//Used to extract the string in <>
string CommandProcessor::extractName(Command& com) {

	const std::string s = com.getCommand();
	//Two patterns(addplayer + loadmap). It will extract the substring from <>.
	std::string string1 = "loadmap <";
	std::string string2 = "addplayer <";
	std::regex rgx("(" + string1 + "|" + string2 + ")(.*)>");
	std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
		return match[2];
	else
		return "";
}

void CommandProcessor::clearMemory() {

	for (Command* com : lc) {
		delete com;
	}
}

//*********************************************************************************

//************************************Command**************************************
void Command::saveEffect(string effect) {
	this->effect = effect;
}
Command::Command() {
	command = "";
	effect = "";
}
Command::Command(string com,string effect) {
	command = com;
	this->effect = effect;
}
Command::Command(const Command& com) {
	this->command = com.command;
	this->effect = com.effect;
}
Command& Command::operator =(const Command& com) {
	if (this == &com)
		return *this;
    
	this->command = com.command;
	this->effect = com.effect;

	return *this;
}

ostream& operator<<(ostream& ost, const Command& com) {

	ost << "Command: " << com.command << "; Effect: " << com.effect << endl;
	return ost;

}

string Command::getCommand() {
	return command;
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

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
}

string FileCommandProcessorAdapter::readCommand() {
	
	return flr->readLineFromFile();

}

//It returns a command object with the command extracted from the saved file. If the flr object of FileCommandProcessorAdapter reaches 
//the end of the file or the saved file is empty, it will return a nullptr.
Command* FileCommandProcessorAdapter::saveCommand() {
	string com = readCommand();
	Command* tempCom =nullptr;
	//No commands read from the . txt file
	//Store the input strings in the command objects and push the objects into the list of commands
	if (com.compare("") != 0) {
		tempCom= new Command(com, "");
		lc.push_back(tempCom);
	}
	return tempCom;
}

Command* FileCommandProcessorAdapter::getCommand() {
	
	return  saveCommand();
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
	}

	return line;

}
//********************************************************************************