#include "helpers.h"

bool HelperFunctions::EraseWordFromString(std::string& mainWord, std::string wordToLookFor) {
    auto iter = mainWord.find(wordToLookFor);
    
    bool foundAny = false;
    if(iter != std::string::npos){
        foundAny = true;
    }
    while (iter != std::string::npos) {
        
        mainWord.erase(iter, wordToLookFor.length());
        
        iter = mainWord.find(wordToLookFor, iter);
    }
    return foundAny;
}

