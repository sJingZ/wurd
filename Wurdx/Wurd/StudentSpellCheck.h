#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    class TNode{
    public:
        char value;
//        std::vector<TNode*> children;
        TNode* children[27];
        bool isEnd;
        TNode(){
            for (int i=0; i<27; i++){
                children[i] = nullptr;
            }
            isEnd = false;
        }
    };
    TNode* root;
    void breakUpSentence(const std::string line, std::vector<SpellCheck::Position>& wordPos);
};

#endif  // STUDENTSPELLCHECK_H_
