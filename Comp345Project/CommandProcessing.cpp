#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <vector>
#include <regex>
#include <sstream>
using namespace std;

#include "CommandProcessing.h"
#include "GameEngine.h"

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
		return (FirstArg.compare("addplayer") == 0 || FirstArg.compare("gamestart") == 0);
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


vector<vector<string>> CommandProcessor::validateTourna(Command& com, int state) {
	vector<vector<string>> tempvec;
	
	const string str = com.getOriginalCommand();

	if (state != 1) {
		return tempvec;
	}
	//Extract map files
	//The newly generated string is the list of map files.
	//The map files are represented normally by numbers(1-5).

	std::regex rgx("tournament -M\\s{1}(.+)\\s{1}-P\\s{1}(.+)\\s{1}-G\\s{1}(\\S+)\\s{1}-D\\s{1}(\\S+)\\s*");
	std::smatch match;
	if (std::regex_search(str.begin(), str.end(), match, rgx)) {

		int counter = 1;
		while (counter <= 4) {

			//std::cout << "match: " << match[counter] << '\n';
			//Validate all extracted strings
			//**********************************************
			//*Counter 1 : Mapfiles validated
			//*Counter 2 : Player strategies validated
			//*Counter 3 : Number of games validated
			//*Counter 4 : Max number of game runs validated
			//**********************************************


			//Counter 1
			if (counter == 1) {
				istringstream sst(match[counter]);
				string tempstr;
				vector<string> vec;
				tempvec.push_back(vec);
				while (getline(sst, tempstr, ' ')) {
					//The list of map files must contains number ranging from 1 to 50
					//Otherwise, it is an invalid command
					try {
						int i = stoi(tempstr);

						if (i < 1 || i >5) {
							tempvec.clear();
							return tempvec;
						}

						//Reject the repeated parameters
						if (std::count(tempvec[counter - 1].begin(), tempvec[counter - 1].end(), tempstr) == 0) {
							tempvec[counter - 1].push_back(tempstr);
						}

					}
					catch (const std::invalid_argument& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
					catch (const std::out_of_range& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
				}

				//Extra validation because 0 parameter cant be accepted.Otherwise invalid
				if (tempvec[counter - 1].size() == 0) {
					tempvec.clear();
					return tempvec;
					//cout<<"Invalid command"<<endl;
				}
			}
			//Counter 2
			else if (counter == 2) {
				istringstream sst(match[counter]);
				string tempstr;
				vector<string> vec;
				tempvec.push_back(vec);
				vector<string> p_strategy = { "A","B","N","C","H"};

				while (getline(sst, tempstr, ' ')) {
					//The player strategy must be contained in the vector shown above
					//Otherwise, it is an invalid command
					if (std::count(p_strategy.begin(), p_strategy.end(), tempstr)) {

						//Reject the repeated parameters
						if (std::count(tempvec[counter - 1].begin(), tempvec[counter - 1].end(), tempstr) == 0) {
							tempvec[counter - 1].push_back(tempstr);
						}

					}
					else {
						tempvec.clear();
						return tempvec;
						//cout<<"Invalid Command"<<endl;
					}
				}

				//Extra validation because 0 parameter cant be accepted.Otherwise invalid
				if (tempvec[counter - 1].size() == 0) {
					tempvec.clear();
					return tempvec;
					//cout<<"Invalid command"<<endl;
				}
			}
			//Counter 3 
			else if (counter == 3) {
				istringstream sst(match[counter]);
				string tempstr;
				vector<string> vec;
				tempvec.push_back(vec);
				while (getline(sst, tempstr, ' ')) {
					//The number of game runs on each map must be a number ranging from 1 to 5
					//Otherwise, it is an invalid command
					try {

						int i = stoi(tempstr);
						if (i < 1 || i >5) {
							tempvec.clear();
							return tempvec;
							//cout<<"invalid command";
						}

						tempvec[counter - 1].push_back(tempstr);

					}
					catch (const std::invalid_argument& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
					catch (const std::out_of_range& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
				}
				//Extra validation because only 1 parameter can be accepted.Otherwise invalid
				if (tempvec[counter - 1].size() != 1) {
					tempvec.clear();
					return tempvec;
					//cout<<"Invalid command"<<endl;
				}
			}
			else if (counter == 4) {
				istringstream sst(match[counter]);
				string tempstr;
				vector<string> vec;
				tempvec.push_back(vec);
				while (getline(sst, tempstr, ' ')) {
					//The max number of game runs must be a number ranging from 10 to 50
					//Otherwise, it is an invalid command
					try {

						int i = stoi(tempstr);
						if (i < 10 || i > 50) {
							tempvec.clear();
							return tempvec;
						}
						tempvec[counter - 1].push_back(tempstr);
					}
					catch (const std::invalid_argument& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
					catch (const std::out_of_range& e) {
						tempvec.clear();
						return tempvec;
						//std::cout << e.what() << "\n";
					}
				}
				//Extra validation because only 1 parameter can be accepted Otherwise invalid
				if (tempvec[counter - 1].size() != 1) {
					tempvec.clear();
					return tempvec;
					//cout<<"Invalid command"<<endl;
				}
			}

			//Update the counter
			counter++;

		}
	}
	//Nothing matched == Invalid command absolutely
	else {
		tempvec.clear();
		return tempvec;
		//cout<<"Invalid command!";
	}

	return tempvec;
}

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
string Command::extractName(string str) {

	const std::string s = str;

	std::string string2 = "addplayer <";
	std::regex rgx("addplayer\\s{1}(.*)\\s*");
	std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
		return match[1];
	else
		return "";
}
void Command::saveEffect(string effect) {
	string temp1 = "Command is successfully executed: ";
	string temp2 = this->getOriginalCommand();
	string temp3 = "";
	if (effect.compare("Invalid Command") == 0) {
		this->effect = "Invalid Command";
		notify(this);
		return;
	}
    if ((this->getArgs())[0].compare("gamestart") == 0) {
		temp3 = ". The game starts. ";
	}
	else if (std::regex_match(temp2,std::regex("addplayer\\s{1}(.*)\\s*"))) {
		temp3 = ".PlAYER "+extractName(temp2)+" have been successfully added. ";
	}
	else if ((this->getArgs())[0].compare("loadmap") == 0) {
		temp3 = ". Map has been successfully loaded. ";
	}
	else if ((this->getArgs())[0].compare("validatemap") == 0) {
		temp3 = ". Map has been successfully validated. ";
	}
	else if ((this->getArgs())[0].compare("endissueorder") == 0) {
		temp3 = ". Players have ended the state of issuing orders. ";
	}
	else if ((this->getArgs())[0].compare("issueorder") == 0) {
		temp3 = ".Players have successfully issued the orders . ";
	}
	else if ((this->getArgs())[0].compare("replay") == 0) {
		temp3 = ". The game re-starts. ";
	}
	else if ((this->getArgs())[0].compare("quit") == 0) {
		temp3 = ".The game quits .";
	}

   
	temp1.append(temp2);
	temp1.append(temp3);
    
	this->effect = temp1;

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