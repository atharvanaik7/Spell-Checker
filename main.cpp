#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include "TimeInterval.h"

// initializing vectors and lists
std::vector <std::string> dictionaryTable;
std::vector <std::string> wordTable;
std::vector <std::string> fLetterTable;
int hashSize = 26;
std::list<std::string> *hashTable;
std::list<std::string> *hashTableTwo;

// loads lists with size of 26
void loadHashTable(int size) {
	hashTable = new std::list<std::string>[size];
	hashTableTwo = new std::list<std::string>[size];
}

// load dictionary from dictionary.txt and puts into a vector
void loadDictionary() {
	std::ifstream myfile;
	std::string line;
	myfile.open("Dictionary.txt"); // opens dictionary file
	while (!myfile.eof()) { // loops thorugh until it hits the end of the file
		std::getline(myfile, line); // gets each line
		dictionaryTable.push_back(line); // pushes into vector
	}
	myfile.close(); // closes file
}

// makes key depending on the word and the index specified
int makeKey(std::string word, int index) {
	char fLetter = word[index]; // gets character of the word at specified index
	int key = int(fLetter); // makes into ascii value
	return key; // returns key
}

 // hashes the key
int hash(int key) { 
	return key % hashSize; // returns hash 
}

// creates hash table
void createHashTable(std::list<std::string>* hashTab, std::vector<std::string>& table, int letterPos) {
	for (int i = 0; i < 26; i++) {
		hashTab[i].clear(); // clears each index of the hash table
	}
	for (int i = 0; i < table.size(); i++) { // goes through each word in the table given either the dictionary table or word table
		std::string word;
		word = table.at(i);
		int index = hash(makeKey(word, letterPos)); // hashes the current word in the loop to get index
		hashTab[index].push_back(word); // puts word into hash list at the index
	}
}

// evaluates the word and puts into vector of words that match the word given
void evaluateWord(int key, int keyTwo, std::list<std::string>* hashTab, std::list<std::string>* hashTabTwo, std::vector<std::string>& wordTable, std::vector<std::string>& fLetterTable) { 
	int index = hash(key); // hashes the user input word
	for (auto x : hashTab[index]) { // using the hash table at the index of the user input word
		wordTable.push_back(x);  // pushes all words that start with the same first letter as the user input word
		fLetterTable.push_back(x); // pushes all words that start with the same first letter as the user input word
	}
	createHashTable(hashTabTwo, wordTable, 1); // creates second hash table for second letter of the words that have the same first letter as user input word
	wordTable.clear(); // clears word table
	int indexTwo = hash(keyTwo); // gets second letter hash value of user input word
	for (auto x : hashTabTwo[indexTwo]) { // using the second hash table at the index of the user input word
		wordTable.push_back(x); // pushes all words that start with the same first two letters as the user input word
	}
	hashTabTwo[indexTwo].clear(); // clears every word at the second letter index in the second hash table
}

// returns true if the word is found in the the word vector, false if not
bool findWord(std::string word, std::vector<std::string>& wordTable) {
	for (int i = 0; i < wordTable.size(); i++) { // loops through each word that has the same first two letter
		if (word == wordTable.at(i)) { // if the word is found
			return true; // returns true
		}
	}
	return false; // if not returns false
}

// checks word if word exists, and if the word is misspelled
void checkWord(std::string word, bool wordVal, std::vector<std::string>& wordTable, std::vector<std::string>& fLetterTable) {
	if (wordVal == 1) { // if the word exists
		std::cout << "True" << std::endl; // returns true
		for (int i = 0; i < wordTable.size(); i++) {
			std::cout << wordTable.at(i) << std::endl; // prints all words that start with the same first two letters
		}
		return;
	}
	else if (wordVal == 0 && fLetterTable.size() != 0) { // if the word is misspelled
		int wordLength = word.length(); // gets word length of the user input
		std::vector <std::string> filtered;
		std::vector <int> countVals;
		int count = 0;
		for (int i = 0; i < fLetterTable.size(); i++) { 
			std::string currWord = fLetterTable.at(i);
			if (currWord.length() >= (wordLength - 1) && currWord.length() <= (wordLength + 1)) { // checks for all words that have length of the user input +/- 1
				filtered.push_back(currWord); // puts those words into a filtered vector
			}
		}
		if (filtered.size() == 1) { // if the size of the vector is 1 meaning only one word is left
			std::string ans;
			std::cout << "Do you mean: " << filtered.at(0) << std::endl; // asks user if the suggested word is correct
			std::cout << "yes or no: ";
			std::cin >> ans;
			if (ans == "yes") { // if yes
				std::cout << "True" << std::endl; // returns true
				std::string correctWord = filtered.at(0);
				wordTable.clear();
				int key = makeKey(correctWord, 0);
				int keyTwo = makeKey(correctWord, 1);
				evaluateWord(key, keyTwo, hashTable, hashTableTwo, wordTable, fLetterTable); // gets all the words that start with the same first two letters as the suggested word
				for (int j = 0; j < wordTable.size(); j++) {
					std::cout << wordTable.at(j) << std::endl; // prints all of them out
				}
			}
			else if (ans == "no") { // if no
				std::cout << "False" << std::endl; // returns false
			}
		}
		else { // if the filtered vector has more than one word in the the vector

			// for loop gets each word in the filtered list
			// then checks if any letters match with the user input word
			// if they do there will be a count that increases if there is a word that matches
			for (int i = 0; i < filtered.size(); i++) {
				std::string wordTest = filtered.at(i);
				for (int j = 0; j < wordLength; j++) {
					for (int k = 1; k < wordTest.length(); k++) {
						if (word[j] == wordTest[k]) {
							count++;
						}
					}
				}
				countVals.push_back(count); // the counts for all words will be put into the vector
				count = 0; // reset count
			}
			if (countVals.size() < 1) { // if the count vector size is 0 meaning no resemblance was found
				std::cout << "False" << std::endl; // returns false
				return;
			}
			int maxElement = *std::max_element(countVals.begin(), countVals.end());
			if (maxElement < 1) { // if the max count value was less than 1 meaning little to no resemblance
				std::cout << "False" << std::endl; // returns false
				return;
			}

			// if there are multiple words with resemblance with the user input word
			int maxIndex = std::max_element(countVals.begin(), countVals.end()) - countVals.begin(); // gets the index where the max value occurs
			std::cout << "Do you mean: " << filtered.at(maxIndex) << std::endl; // recommends the word with the most resemblance
			std::string ans;
			std::cout << "yes or no: ";
			std::cin >> ans;
			if (ans == "yes") { // if yes
				std::cout << "True" << std::endl; // returns true
				std::string correctWord = filtered.at(maxIndex);
				wordTable.clear();
				int key = makeKey(correctWord, 0);
				int keyTwo = makeKey(correctWord, 1);
				evaluateWord(key, keyTwo, hashTable, hashTableTwo, wordTable, fLetterTable); // gets all the words that start with the same first two letters as the suggested word
				for (int j = 0; j < wordTable.size(); j++) {
					std::cout << wordTable.at(j) << std::endl; // prints all of them out
				}
			}
			else if (ans == "no") { // if no
				std::cout << "False" << std::endl; // returns false
			}
		}
		return;
	}
}

// clears all vectors
void clearTable() {
	wordTable.clear();
	fLetterTable.clear();
}

int main() {
	int size = 26;
	bool running = true;
	loadDictionary(); // loads dictionary
	loadHashTable(size); // loads hash table with size of 26
	createHashTable(hashTable, dictionaryTable, 0); // creates hashtable with the first letter

	while (true) {
		std::string word;
		std::cout << "Enter Word: ";
		std::cin >> word; // user input
		if (word == "QUIT") { // if "QUIT" is entered the program ends
			break;
		}
		int key = makeKey(word, 0); // gets ascii value of the first letter of the user input word
		int keyTwo = makeKey(word, 1); // gets ascii value of the second letter of the user input word
		evaluateWord(key, keyTwo, hashTable, hashTableTwo, wordTable, fLetterTable); // evaluates the user input word
		bool wordVal = findWord(word, wordTable); // returns true or false depending on if the program finds the word
		checkWord(word, wordVal, wordTable, fLetterTable); // checks if the word exists or misspelled, then will perform correct tasks accordingly
		clearTable(); // clears tables
	}
}