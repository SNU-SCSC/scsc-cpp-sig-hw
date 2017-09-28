#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <queue>
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

void start(std::string inputText, int stateSize, int outputSize) {
    timerStart();

    MarkovMap markovMap;

    std::vector<char> markovKey(inputText.begin(), inputText.begin() + stateSize);
    for (auto it = inputText.begin() + stateSize;
         it != inputText.end() - stateSize;
         ++it) {
        char nextChar = *it;

        MarkovState markovState(markovKey.begin(), markovKey.end());
        markovMap[markovState].push_back(nextChar);

        markovKey.erase(markovKey.begin());
        markovKey.push_back(nextChar);
    }
    timerEnd();
    cout << "Created markov chain." << endl;
    printElapsedTime();

    timerStart();
    MarkovMap::size_type randIndex = std::rand() % markovMap.size();
    auto randEntry = std::next(std::begin(markovMap), randIndex);
    MarkovState startSeed = randEntry->first;
    timerEnd();
    cout << "Selected random seed as: "
         << std::string(startSeed.begin(), startSeed.end()) << endl;
    printElapsedTime();

    timerStart();
    std::string generatedText(startSeed.begin(), startSeed.end());

    std::vector<char> currentKey(startSeed.begin(), startSeed.end());
    for (int i = 0; i < outputSize; i++) {
        MarkovState markovState(currentKey.begin(), currentKey.end());

        auto nextCharIt = selectRandom(markovMap[markovState]);
        char nextChar = *nextCharIt;
        generatedText += nextChar;

        currentKey.erase(currentKey.begin());
        currentKey.push_back(nextChar);
    }

    timerEnd();
    cout << "Finished generating text: \n" << generatedText << endl;
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
    while(wordStream) {
        wordStream >> word;
        text += word;
        text += " ";
    }
    cout << "Removed whitespace." << endl;

    start(text, stateSize, outputSize);

    return 0;
}
