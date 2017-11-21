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
    // Put your code in here...
    std::string::iterator it_T = inputText.begin();

    //this is the map we'll save the distribution to
    MarkovMap this_map;

    MarkovState input_string;
    //as it is a Markov chain, we'll constantly add up the character vector, than randomly pick a
    //character from the character vector
    //for the length of state_sized string map, increment the number of next character coming
    for(;it_T < inputText.end() - stateSize; ++it_T){
        input_string.clear();
        for(int i = 0; i < stateSize; ++i){
            input_string.push_back(*(it_T + i));
        }
        this_map[input_string].push_back(*(it_T + stateSize));
    }

    //this will be the random state_size string
    MarkovMap::iterator to_begin = this_map.begin();
    int beginning_number = rand()%(int)(inputText.end() - inputText.begin() - stateSize);

    for(int i = 0; i < beginning_number; ++i){
        to_begin++;
    }

    //returns the random markovState
    //to_begin->first;
    //next_input(this_map[to_begin->first]);
    auto c = selectRandom(this_map[to_begin->first]);

    MarkovState output(to_begin->first);
    MarkovState temp_str = output;
    for(int i = 0; i < outputSize - stateSize; ++i){
        temp_str.push_back(*selectRandom(this_map[temp_str]));
        output.push_back(temp_str.back());
        temp_str.erase(temp_str.begin());
    }
    std::cout << output << std::endl;
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
