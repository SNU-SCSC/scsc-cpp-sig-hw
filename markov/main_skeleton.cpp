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
    // Put your code in here...
}

int main() {
    std::srand(std::time(0));

    std::string modeStr;
    MarkovMode mode;

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
