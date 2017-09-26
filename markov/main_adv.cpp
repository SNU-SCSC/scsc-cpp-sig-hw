#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <regex>

template<typename T>
auto selectRandom(const std::vector<T>& vec) {
    return std::next(vec.begin(), std::rand() % vec.size());
}

using std::cout; using std::cin; using std::endl;

using MarkovState = std::string;
using NextChars = std::vector<char>;
using MarkovMap = std::unordered_map<MarkovState, NextChars>;

void start(std::string inputText, int stateSize, int outputSize) {
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
    cout << "Created markov chain." << endl;

    MarkovMap::size_type randIndex = std::rand() % markovMap.size();
    auto randEntry = std::next(std::begin(markovMap), randIndex);
    MarkovState startSeed = randEntry->first;
    cout << "Selected random seed as: "
         << std::string(startSeed.begin(), startSeed.end()) << endl;

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

    cout << "Finished generating text: \n" << generatedText << endl;
}

enum MarkovMode { Char, Word };

int main(int argc, char* argv[]) {
    std::srand(std::time(0));

    std::string modeStr;
    MarkovMode mode;
    std::string filename;
    if (argc == 3) {
        modeStr = argv[1];
        filename = argv[2];
    }
    else {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
    }
    if (modeStr == "-c") {
        mode = MarkovMode::Char;
    }
    else if (modeStr == "-s") {
        mode = MarkovMode::Word;
    }
    else {
        std::cerr << "Invalid mode string." << std::endl;
        return 2;
    }

    std::ifstream istream(filename);

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

    switch(mode) {
        case MarkovMode::Char: {
            start(text, stateSize, outputSize);
            break;
        }
        case MarkovMode::Word: {
            std::istream_iterator<std::string> begin(wordStream);
            std::istream_iterator<std::string> end;
            std::vector<std::string> words(begin, end);
            cout << "Unimplemented..." << endl;
            break;
        }

    }

    return 0;
}