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
    std::string s;
//    int index = 0;
    while(getline(infile,s)){
        fileContent.push_back(s);
        
//        fileContent[index] = s;
//        fileContent.insert({index, s});
//        index++;
    }
    currLine = fileContent.begin();
    return true; // TODO}
}

bool StudentTextEditor::save(std::string file) {
	
    return false;  // TODO
}

void StudentTextEditor::reset() {
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

void StudentTextEditor::del() {
    if (currRow < fileContent.size()-1 &&
        currCol < currLine->size()-1){
        (*currLine) = currLine->substr(0, currCol) + currLine->substr(currCol+1);
    }
    else if (currRow < fileContent.size()-1 &&
             currCol == currLine->size()){
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
        (*prevLine) = *prevLine + *currLine;
        fileContent.remove(*currLine);
        currLine = prevLine;
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
