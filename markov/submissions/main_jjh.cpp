#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <chrono>
template<typename T>
auto selectRandom(const std::vector<T>& vec) {
	return std::next(vec.begin(), std::rand() % vec.size());
}

decltype(std::chrono::high_resolution_clock::now()) curTime;
std::chrono::duration<double> diffTime;

void timerStart() {
	curTime = std::chrono::high_resolution_clock::now();
}

void timerEnd() {
	auto prevTime = curTime;
	curTime = std::chrono::high_resolution_clock::now();
	diffTime = curTime - prevTime;
}

void printElapsedTime() {
	std::cout << "Elapsed: " << diffTime.count() << "s" << std::endl;
}

using std::cout; using std::cin; using std::endl;

using MarkovState = std::string;
using NextChars = std::vector<char>;
using MarkovMap = std::unordered_map<MarkovState, NextChars>;
void CreateMarkov(MarkovMap &MkChain, int stateSize, const std::string &inputText) {
	decltype(inputText.size()) A = 0;
	decltype(inputText.size()) B = inputText.size() - stateSize;
	while (A<B) {
		MkChain[inputText.substr(A, stateSize)].push_back(inputText[A+stateSize]);
		++A;

	}
}
std::string randomSeed(const std::string &inputText, int stateSize) {
	auto point = inputText.begin() + std::rand() % (inputText.size() - stateSize);
	std::string randomSeed;
	for (int i = 0; i < stateSize; ++i) {
		randomSeed.push_back(*(point + i));
	}
	return randomSeed;
}
void generateText(int outputSize, std::string &Text, MarkovMap &MkChain,int stateSize) {
	std::deque<char> statenode;
	for (auto a : Text) {
		statenode.push_back(a);
	}
	while (Text.size() < outputSize) {
		std::string stnode(statenode.begin(), statenode.end());
		char C = *selectRandom(MkChain[stnode]);
		statenode.pop_front();
		statenode.push_back(C);
		Text.push_back(C);
	}
}
void start(const std::string &inputText, int stateSize, int outputSize) {
	// Put your code in here...
	MarkovMap MkChain;
	timerStart();
	CreateMarkov(MkChain, stateSize, inputText);
	timerEnd();
	cout << "Created Markov chain." << endl;
	printElapsedTime();
	timerStart();
	std::string Text = randomSeed(inputText, stateSize);
	timerEnd();
	cout << "selected random seed as: " << Text << endl;
	printElapsedTime();
	timerStart();
	generateText(outputSize, Text, MkChain, stateSize);
	timerEnd();
	cout <<"finished generating text: \n" << Text << endl;
	printElapsedTime();
}


int main() {
	std::srand(std::time(0));

	std::string filename;
	cout << "File name: ";
	cin >> filename;
	cout << endl;

	std::ifstream istream(filename);
	if (!istream) {
		cout << "Cannot read file!" << endl;
		return 1;
	}

	int stateSize;
	cout << "Markov chain length: ";
	cin >> stateSize;
	cout << endl;

	int outputSize;
	cout << "Output length: ";
	cin >> outputSize;
	cout << endl;

	std::stringstream buffer;
	buffer << istream.rdbuf();
	std::string originalText = buffer.str();
	cout << "Loaded original text." << endl;

	std::string text;
	std::stringstream wordStream(originalText);
	std::string word;
	while (wordStream) {
		wordStream >> word;
		text += word;
		text += " ";
	}
	cout << "Removed whitespace." << endl;

	start(text, stateSize, outputSize);
	return 0;
}