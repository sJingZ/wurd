#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
//#include <map>
using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
     currRow = 0;
     currCol = 0;
     // TODO
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
}

bool StudentTextEditor::load(std::string file) {
    ifstream infile(file);
    if (!infile)
    {
        // cerr << "Error: Cannot open" << file << "!" << endl;
        return false;
    }
    fileContent.clear();
    std::string s;
    while(getline(infile,s)){
        if (s[s.size() - 1] == '\r'){
            s.erase(s.size()-1);
        }
        fileContent.push_back(s);
    }
    currLine = fileContent.begin();
    currRow= 0;
    currCol = 0;
    return true; // TODO}
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile(file);
    if(!outfile){
        return false;
    }
    list<string>::iterator it = fileContent.begin();
    for(;it != fileContent.end();it++){
        outfile << *it << endl;
    }
    return true;
    // TODO
}

void StudentTextEditor::reset() {
    fileContent.clear();
	// TODO
}


//////// Check big o of getting size
void StudentTextEditor::move(Dir dir) {
    switch (dir) {
        case Dir::UP:{
            if(currRow > 0){
                currRow--;
                currLine--;
            }
            break;
        }
        case Dir::DOWN:{
            if(currRow < fileContent.size() - 1){
                currRow++;
                currLine++;
            }
            break;
        }
        case Dir::LEFT:{
            if(currCol <= 0 && currRow <= 0){break;} // cursor on first char of top line
            if (currCol <= 0){ // at first char of the line
                currRow--;
                currLine--;
                currCol = currLine->size();
            }
            else{currCol--;}
            break;
        }
        case Dir::RIGHT:{
            if(currCol >= currLine->size() && currRow >= fileContent.size() - 1){break;} // cursor on last char of bottom line
            if (currCol >= currLine->size()){ // at last char of the line
                currRow++;
                currLine++;
                currCol = 0;
            }
            else{currCol++;}
            break;
        }
        case Dir::HOME:{
            currCol = 0;
            break;
        }
        case Dir::END:{
            currCol = currLine->size();
            break;
        }
        default:
            break;
    }
    
    if(currRow < 0){
        currRow = 0;
        currLine = fileContent.begin();
    }
    else if(currRow > fileContent.size() - 1){
        currRow = fileContent.size() - 1;
        currLine = fileContent.end();
    }
    else if(currCol < 0){currCol = 0;}
    else if(currCol > currLine->size()){
        currCol = currLine->size();
    }
    // TODO
}

// delete current char
void StudentTextEditor::del() {
    if (currRow <= fileContent.size()-1 &&
        currCol <= currLine->size()-1){
        (*currLine) = currLine->substr(0, currCol) + currLine->substr(currCol+1);
    }
    else if (currRow <= fileContent.size()-1 &&
             currCol == currLine->size()){ //last char of each line
        list<string>::iterator nextLine = currLine;
        nextLine++;
        (*currLine) = *currLine + *(nextLine);
        fileContent.remove(*nextLine);
    }
	// TODO
}

void StudentTextEditor::backspace() {
    if (currCol > 0){
        (*currLine) = currLine->substr(0, currCol-1) + currLine->substr(currCol);
        currCol--;
    }
    else if(currCol == 0){
        list<string>::iterator prevLine = currLine;
        prevLine--;
        currCol = prevLine->size();
        (*prevLine) = *prevLine + *currLine;
        fileContent.erase(currLine);
        currLine = prevLine;
        currRow--;
    }
	// TODO
}

void StudentTextEditor::insert(char ch) {
    if (ch == '\t'){
        currLine->insert(currCol,"    ");
        currCol+=4;
        return;
    }
    else{
        currLine->insert(currCol, 1, ch);
        currCol++;
        return;
    }
}

void StudentTextEditor::enter() {
    string temp = currLine->substr(currCol);
    *(currLine) = currLine->substr(0, currCol);
    currLine++;
    
    // take the rest to save to fileContent[currRow+1]
    fileContent.insert(currLine, temp);
    currLine--; // get back to the newly added line
    
    // change current row and column
    currRow++;
    currCol = 0;
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = currRow; col = currCol; // TODO
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if (startRow < 0 || numRows < 0 || startRow > fileContent.size()){
        return -1;
    }
    lines.clear();
    list<string>::iterator temp;
    temp = currLine;
    int traverseRow = currRow < startRow? startRow - currRow: currRow - startRow;
    for (int k=0; k < traverseRow; k++){
        temp--;
    }
    int maxLines = numRows > fileContent.size() - startRow? fileContent.size() - startRow:numRows;
    for (int i=0; i < maxLines; i++){
        lines.push_back(*temp);
        temp++;
    }
	return maxLines+1; // TODO
}

void StudentTextEditor::undo() {
	// TODO
}
