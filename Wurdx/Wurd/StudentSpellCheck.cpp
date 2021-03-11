#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}


bool StudentSpellCheck::load(std::string dictionaryFile) {
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
            if(word[i] == '\''){index = 27;}
            if(!curr->children[index]){
                curr->children[index] = new TNode;
            }
            curr = curr->children[index];
        }
        curr->isEnd = true;
    }
    return true; // TODO
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    TNode *curr = root;
    for(int i=0; i<word.size(); i++){
        int index = word[i] - 'a';
        if(word[i] == '\''){index = 27;}
        if(!curr->children[index])
            return false;
        curr = curr->children[index];
    }
    return (curr && curr->isEnd);
//	return false; // TODO
}

void StudentSpellCheck::breakUpSentence(const std::string line, std::vector<SpellCheck::Position>& wordPos){
        int len = 0;
        for(int i=0;i<line.size();i++){
            if (isalpha(line[i])){
                len++;
            }
            else if(line[i] == '\''){
                if (i < line.size()-1 && isalpha(line[i+1])){
                    len++;
                }
                else if (i >= line.size()-1){ // reach the end
                    Position* newP = new Position;
                    newP->end = i-1;
                    newP->start = i-len;
                    len = 0;
                    return;
                }
                else{
                    Position* newP = new Position;
                    newP->end = i-1;
                    newP->start = i-len;
                    len = 0;
                }
            }
            else{
                Position* newP = new Position;
                newP->end = i-1;
                newP->start = i-len;
                len = 0;
            }
        }
    }


void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    std::vector<SpellCheck::Position> wordPositions;
    breakUpSentence(line, wordPositions);
	// TODO
}
