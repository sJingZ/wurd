#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>
//#include <map>
//#include <vector>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
    int currRow;
    int currCol;
//    std::map<int, std::string> fileContent;
//    std::vector<std::string> fileContent;
    std::list<std::string> fileContent;
    std::list<std::string>::iterator currLine;
    bool undoCalled;
};

#endif // STUDENTTEXTEDITOR_H_
