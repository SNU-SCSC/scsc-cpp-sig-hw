#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <chrono>

// Column Width Reference                                                     80                                    120   125    132 136
//                                                                             |                                      |     |      |   |
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

void printElapsedTime(void) {
    std::cout << "Elapsed: " << diffTime.count() << "s" << std::endl;
}

using std::cout; using std::cin; using std::endl;

using MarkovState = std::string;
using NextChars = std::vector<char>;
using MarkovMap = std::unordered_map<MarkovState, NextChars>;

void start(std::string inputText, int stateSize, int outputSize) {
    MarkovMap markovChain;

    /* Generate Markov Chain */
    timerStart();
    for(auto i = inputText.cbegin(); i != (inputText.cend()-stateSize); ++i)
    {
        // New Entry
        if(!markovChain.count(std::string(i,i+stateSize)))
            // Assign new vector to key
            markovChain[std::string(i,i+stateSize)]=NextChars();
        // Add following character to the chain
        markovChain[std::string(i,i+stateSize)].push_back(*(i+stateSize));
    }
    timerEnd();
    cout << "Created markov chain." << endl;
    printElapsedTime();
    cout << endl;

    /* Select rand. seed */
    timerStart();
    std::vector<MarkovState> seedList;
    for(const auto& i : markovChain) // Get vector of keys
        seedList.push_back(i.first);
    MarkovState seed = *selectRandom(seedList); // Select one
    timerEnd();
    cout << "Selected random seed as: " << seed << endl;
    printElapsedTime();
    cout << endl;

    /* Generate text */
    timerStart();
    std::string generatedText(seed); // initialize with seed
    // Generate text until reach desired size
    while(generatedText.size() < outputSize) 
        // Append string with randomly choosen char. from the markov chain
        generatedText+= *selectRandom(
        markovChain[generatedText.substr(generatedText.size()-stateSize)]
        );
    timerEnd();
    cout << "Finished generating text:" << endl;
    cout << generatedText << endl;
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
    istream.close();
    std::string originalText = buffer.str();
    cout << "Loaded original text.\n" << endl;

    std::string text;
    std::stringstream wordStream(originalText);
    while(wordStream) {
        std::string word;
        wordStream >> word;
        if(word.empty()) break; // End of VALID text.i.e. No more words left.
        text += word;
        text += " ";
    }
    while(text.back() == ' ') // Remove trailing space
        text.pop_back();
    cout << "Removed whitespace.\n" << endl;
    start(text, stateSize, outputSize); // Start processing

    return 0;
}
