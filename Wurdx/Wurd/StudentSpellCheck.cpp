#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}


// set root to null in const.




StudentSpellCheck::~StudentSpellCheck() {
    delete root;
	// TODO
}


bool StudentSpellCheck::load(std::string dictionaryFile) {
    delete root;
    std::ifstream infile(dictionaryFile);
    if (!infile)
    {
        return false;
    }
    
    root = new TNode;
    TNode* curr = root;
    std::string word;
    
    while(getline(infile,word)){
        for(int i=0; i< word.size(); i++){
            int index = word[i] - 'a';
            if(word[i] == '\''){index = 26;}
            if(!curr->children[index]){
                curr->children[index] = new TNode;
                curr->children[index]->value = word[i];
            }
            curr = curr->children[index];
        }
        curr->isEnd = true;
        curr = root;
    }
    return true; // TODO
}

void StudentSpellCheck::traverse(std::vector<std::string>& suggestions, int countOfErrors, TNode* curr, std::string target, std::string& possibleWord, int height, int max){
    if (curr == nullptr){return;}
//    if (countOfErrors > 1){
//        possibleWord.pop_back();
//        return;}
    if (height == 0){return;}
    if(suggestions.size() > max){return;}

    
    if (curr->value != target[0]){
        countOfErrors++;
        possibleWord+=curr->value;
        height--;
        for(int i=0; i<27; i++){
            if (countOfErrors > 1){
                break;}
            if (height == 0)
                {break;}
            if (curr->children[i]){
                traverse(suggestions, countOfErrors, curr->children[i], target.substr(1), possibleWord, height, max);
            }
        }
    }
    else{
        possibleWord+= curr->value;
        height--;
        for(int i=0; i<27; i++){
            if (countOfErrors > 1){
                break;}
            if (height == 0)
                {break;}
            if (curr->children[i]){
                traverse(suggestions, countOfErrors, curr->children[i], target.substr(1), possibleWord, height, max);
            }
        }
    }
    
    if (height == 0 && curr->isEnd && countOfErrors <= 1){
        if(suggestions.size() > max){return;}
        else{suggestions.push_back(possibleWord);}
    }
    
    possibleWord.pop_back();
    
}

bool StudentSpellCheck::checkInDict(std::string& word){
    TNode *curr = root;
    int i = 0;
    while(i < word.size()){
        word[i] = tolower(word[i]);
        int index = word[i] - 'a';
        if(word[i] == '\''){index = 26;}
        if(!curr->children[index]){break;} // word is not found
        curr = curr->children[index];
        i++;
    }

    if (i == word.size() && curr && curr->isEnd){return true;}
    else{return false;}
}


bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    if(checkInDict(word)){return true;}
    else{
        TNode *curr = root;
        curr = root;
        for (int i=0; i < 27; i++){
            std::string str = "";
            int errors = 0;
            int h = word.size();
            traverse(suggestions, errors, curr->children[i], word, str, h, max_suggestions);
        }
        return false;
    }
}

void StudentSpellCheck::breakUpSentence(const std::string line, std::vector<SpellCheck::Position>& wordPos){
    int len = 0;
    int i = 0;
    while(i < line.size()){
        if(isalpha(line[i]) || line[i] == '\''){
            len++;
        }
        else{
            Position* newP = new Position;
            newP->end = i-1;
            newP->start = i-len;
            len = 0;
            wordPos.push_back(*newP);
        }
        i++;
    }
    if (isalpha(line[i-1]) || line[i-1] == '\''){
        Position* newP = new Position;
        newP->end = i-1;
        newP->start = i-len;
        len = 0;
        wordPos.push_back(*newP);
    }
}



void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    problems.clear();
    std::vector<Position> wordPositions;
    breakUpSentence(line, wordPositions);
    for(int i=0;i<wordPositions.size();i++){
        std::string word = line.substr(wordPositions[i].start,wordPositions[i].end - wordPositions[i].start+1);
        if(!checkInDict(word)){problems.push_back(wordPositions[i]);}
    }
	// TODO
}
