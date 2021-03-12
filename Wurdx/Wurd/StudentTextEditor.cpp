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
     undoCalled = false;
     // TODO
}

StudentTextEditor::~StudentTextEditor()
{
    fileContent.clear();
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
        if (!s.empty() && s[s.size() - 1] == '\r'){
            s.pop_back();
        }
        fileContent.push_back(s);
    }
    if(!fileContent.empty()){currLine = fileContent.begin();}
    else{currLine == fileContent.end();}
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
    if (fileContent.empty()){return;}
    if (currRow <= fileContent.size()-1 &&
        currCol <= currLine->size()-1){
        char temp = (*currLine)[currCol];
        (*currLine) = currLine->substr(0, currCol) + currLine->substr(currCol+1);
        if(!undoCalled){getUndo()->submit(Undo::Action::DELETE, currRow, currCol, temp);}
    }
    else if (currRow < fileContent.size()-1 &&
             currCol == currLine->size()){ //last char of each line
        list<string>::iterator nextLine = currLine;
        nextLine++;
        (*currLine) = *currLine + *(nextLine);
        fileContent.remove(*nextLine);
        if(!undoCalled){getUndo()->submit(Undo::Action::JOIN, currRow, currCol, '.');}
    }

	// TODO
}

void StudentTextEditor::backspace() {
    if (fileContent.empty()){return;}
    if (currCol > 0){
        char temp = (*currLine)[currCol-1];
        (*currLine) = currLine->substr(0, currCol-1) + currLine->substr(currCol);
        currCol--;
        if(!undoCalled){getUndo()->submit(Undo::Action::DELETE, currRow, currCol, temp);}
    }
    else if(currCol == 0 &&
            currRow > 0){ // first char of line
        list<string>::iterator prevLine = currLine;
        prevLine--;
        currCol = prevLine->size();
        (*prevLine) = *prevLine + *currLine;
        fileContent.erase(currLine);
        currLine = prevLine;
        currRow--;
        if(!undoCalled){getUndo()->submit(Undo::Action::JOIN, currRow, currCol, '.');}
    }
	// TODO
}

void StudentTextEditor::insert(char ch) {
    if (ch == '\t'){
        if(!fileContent.empty()){
            currLine->insert(currCol,"    ");}
        else{
            currLine->insert(currCol,"    ");
            currCol+=4;
        }
    }
    else{
        if(!fileContent.empty()){
            currLine->insert(currCol, 1, ch);}
        else{
            fileContent.push_back(string(1, ch));
            currLine = fileContent.begin();
        }
        currCol++;
    }
    if(!undoCalled){
        getUndo()->submit(Undo::Action::INSERT, currRow, currCol, ch);
    }
    return;
}

void StudentTextEditor::enter() {
    if (!fileContent.empty()){
        string temp = currLine->substr(currCol);
        *(currLine) = currLine->substr(0, currCol);
        currLine++;
        
        // take the rest to save to fileContent[currRow+1]
        fileContent.insert(currLine, temp);
        currLine--; // get back to the newly added line
        
        // change current row and column
        currRow++;
        currCol = 0;
    }
    else{
        fileContent.push_back(" ");
        fileContent.push_back(" ");
        currLine = fileContent.begin();
        currLine++;
        currRow = 1;
        currRow = 0;
    }
    if(!undoCalled){
        getUndo()->submit(Undo::Action::SPLIT, currRow, currCol, '.');
    }
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
    
    
    //// Do we need to check for the case where startRow is below currRow?
    if (currRow < startRow){
        for (int k=0; k < startRow - currRow; k++){
            temp--;
        }
    }
    else {
        for (int k=0; k < currRow - startRow; k++){
            temp--;
        }
    }
    
    int maxLines = numRows > fileContent.size() - startRow? fileContent.size() - startRow:numRows;
    for (int i=0; i < maxLines; i++){
        lines.push_back(*temp);
        temp++;
    }
	return maxLines+1; // TODO
}

void StudentTextEditor::undo() {
    int R = -100;
    int C = -100;
    int count = 0;
    string text = "";
    undoCalled = true;
//    Undo::Action act = getUndo()->get(R, C, count, text);
//    if (R > currRow){
//        for (int i=0; i<R-currRow; i++){currLine++;}
//    }
//    else{
//        for(int i=0; i < currRow - R; i++){currLine--;}
//    }
//    currCol = C;
//    currRow = R;
    switch (getUndo()->get(R, C, count, text)) {
        case Undo::Action::INSERT:{
            if (R > currRow){
                for (int i=0; i<R-currRow; i++){currLine++;}
            }
            else{
                for(int i=0; i < currRow - R; i++){currLine--;}
            }
            currCol = C;
            currRow = R;
            currLine->insert(currCol, text);
            break;
        }
        case Undo::Action::DELETE:{
            if (R > currRow){
                for (int i=0; i<R-currRow; i++){
                    currLine++;
                }
            }
            else{
                for(int i=0; i < currRow - R; i++){
                    currLine--;
                }
            }
            currCol = C;
            currRow = R;
            if(text[0] == '\t'){
                for (int i=0; i < count*4; i++) {backspace();}
            }
            else{
                for(int i=0;i<count;i++){backspace();}
            }
            break;
        }
        case Undo::Action::SPLIT:{
            if (R > currRow){
                for (int i=0; i<R-currRow; i++){
                    currLine++;
                }
            }
            else{
                for(int i=0; i < currRow - R; i++){
                    currLine--;
                }
            }
            currCol = C;
            currRow = R;
            string temp = currLine->substr(currCol);
            *(currLine) = currLine->substr(0, currCol);
            currLine++;
            
            // take the rest to save to fileContent[currRow+1]
            fileContent.insert(currLine, temp);
            currLine--; // get back to the newly added line
            currLine--;
            break;
        }
        case Undo::Action::JOIN:
        default:
            break;
    }
    undoCalled = false;
	// TODO
}
