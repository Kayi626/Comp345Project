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

CommandProcessor& CommandProcessor::operator =(const CommandProcessor& comP) {
	if (this == &comP)
		return *this;
	for (list<Command*>::iterator it = lc.begin(); it != lc.end(); ++it) {
		Command* tempCom = new Command(**it);
		lc.push_back(tempCom);
	}

	return *this;
}

string CommandProcessor::getCommand() {
	string input = readCommand();
	saveCommand(input);
	return input;
}

bool CommandProcessor::validate(Command& com, string state) {
	bool status = true;
	
	if (std::regex_match(com.getCommand(), std::regex("loadmap <(.*)>")) && (state.compare("start") == 0 || state.compare("maploaded") == 0)) {
		com.saveEffect("maploaded");
	}
	else if ((com.getCommand().compare("validatemap") == 0) && (state.compare("maploaded") == 0)) {
		com.saveEffect("mapvalidated");
	}
	else if ((std::regex_match(com.getCommand(),std::regex("(addplayer <(.*)>"))) && (state.compare("mapvalidated") == 0 || state.compare("playersadded") == 0)) {
		com.saveEffect("playersadded");
	}
	else if ((com.getCommand().compare("gamestart") == 0) && (state.compare("playersadded") == 0)){
		com.saveEffect("assignreinforcement");
	}
	else if ((com.getCommand().compare("replay") == 0) && (state.compare("win") == 0)) {
		com.saveEffect("start");
	}
	else if ((com.getCommand().compare("quit") == 0) && (state.compare("win"))) {
		com.saveEffect("exit program");
	}
	else {

		string errorMessage = "Rejected: Command [ " + com.getCommand() + "] is invalid in State [ " + state + " ]";
		com.saveEffect(errorMessage);
		status = false;
	}

	return status;

}

string CommandProcessor::extractName() {

	const std::string s = this->getCommand();
	//Two patterns(addplayer + loadmap). It will extract the substring from <>.
	std::string string1 = "loadmap <";
	std::string string2 = "addplayer <";
	std::regex rgx("(" + string1 + "|" + string2 + ")(\\w+)>");
	std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
		return match[1];
	else
		return "";
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

vector<string> FileCommandProcessorAdapter::readCommand(string path) {
	
	return flr->readLineFromFile(path);

}

void FileCommandProcessorAdapter::saveCommand(vector<string> com) {
	//Store the input strings in the command objects and push the objects into the list of commands
	for (vector<string>::iterator it = com.begin(); it != com.end(); ++it) {
		Command* tempCom = new Command(*it, "");
		lc.push_back(tempCom);
	}

}


//********************************************************************************


//**************************FileReader********************************************
vector<string> FileLineReader::readLineFromFile(string path) {
	std::fstream in;
	in.open(path, ios::out);
	string line;
	vector<string> vec(10);

	//Read lines from the input file
	in >> line;
	while (!in.eof()) {
		vec.push_back(line);
		in >> line;
	}

	return vec;

}
//********************************************************************************