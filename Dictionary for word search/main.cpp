#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

#include "Dictionary.h"

void addWord(Dictionary &dict)
{
	std::string word, definition;

	std::cout << "Enter a word and definition to add to the dictionary. Word: ";
	std::cin >> word;
	std::cout << "Definition: ";
	std::cin.ignore();
	std::getline(std::cin, definition);
	dict.addWord(word, definition);
	std::cout << "The word has been successfully added to the dictionary" << std::endl;
}

void findWord(Dictionary &dict)
{
	std::string word;

	std::cout << "Enter a word to search in the dictionary: ";
	std::cin >> word;
	dict.findWord(word);
}

void help()
{
	std::cout << "List of available functions: "
		<< "\nAdd - add a word to the dictionary (enter the word and its description)"
		<< "\nFind - find the definition of a word (word input)"
		<< "\nShow - show all word in the dictionary"
		<< "\nHelp - a list of all commands" << std::endl;
}

void showDictionaty(Dictionary &dict)
{

	for (const auto word : dict.getDictionary())
	{
		std::cout << word.first << ": " << word.second << std::endl;
	}
}

enum FunctionType {
	add_,
	find_,
	help_,
	show_,
	exit_,
	UNKNOWN
};

int main()
{
	Dictionary dict;
	dict.loadFromJsonFile();
	//dict.addWord("example", "a representative form or pattern");
	//dict.findWord("example");
	std::string request;

	std::unordered_map<std::string, FunctionType> functionMap = {
			{"add", add_},
			{"Add", add_},
			{"find", find_},
			{"Find", find_},
			{"help", help_},
			{"Help", help_},
			{"show", show_},
			{"Show", show_},
			{"exit", exit_},
			{"Exit", exit_}
	};

	do
	{
		std::cin >> request;
		FunctionType funcType = functionMap.count(request) ? functionMap[request] : UNKNOWN;

		switch (funcType)
		{
			case add_:
				addWord(dict);
				break;
			case find_:
				findWord(dict);
				break;
			case help_:
				help();
				break;
			case show_:
				showDictionaty(dict);
				break;
			case exit_:
				std::cout << "Exiting the program!" << std::endl;
				break;
			default:
				std::cout << "Invalid function!" << std::endl;
				break;
		}

	} while (request != "exit");

	dict.saveToJsonFile();
	std::cout << "The dictionary has been successfully saved" << std::endl;

	return 0;
}
