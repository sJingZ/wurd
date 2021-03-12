#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if(!editStack.empty() &&
       action == editStack.top()->act &&
       row == editStack.top()->r){ // check batching
        switch (action) {
            case Undo::Action::INSERT:{
                if(col == editStack.top()->c+1){
                    editStack.top()->c = col;
                    editStack.top()->count++;
                    return;
                }
                break;
            }
            case Undo::Action::DELETE:{
                if (col == editStack.top()->c){
                    editStack.top()->value += ch;
                    return;}
                else if (col == editStack.top()->c-1){
                    editStack.top()->c = col;
                    editStack.top()->value.insert(0,1,ch);
                    return;}
            }
            default:
                break;
        }
    }
    
    EDIT* lastEdit = new EDIT;
    lastEdit->act = action;
    lastEdit->r = row;
    lastEdit->c = col;
    lastEdit->value = ch;
    lastEdit->count = 1;
    editStack.push(lastEdit);

    // TODO
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if(editStack.empty()){return Action::ERROR;}
    
    EDIT* topEdit = editStack.top();
    
    switch (topEdit->act) {
        case Undo::Action::INSERT:{
            row = topEdit->r;
            col = topEdit->c;
            count = topEdit->count;
            editStack.pop();
            
            return Undo::Action::DELETE;
            break;
        }
        case Undo::Action::DELETE:{
            row = topEdit->r;
            col = topEdit->c;
            text = topEdit->value;
            editStack.pop();
            return Undo::Action::INSERT;
            break;
        }
        case Undo::Action::JOIN:{
            row = topEdit->r;
            col = topEdit->c;
            editStack.pop();
            return Undo::Action::SPLIT;
        }
        case Undo::Action::SPLIT:{
            row = topEdit->r;
            col = topEdit->c;
            editStack.pop();
            return Undo::Action::JOIN;
        }
        default:
            break;
    }
    
    return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
    for (int i=0; i<editStack.size(); i++){
        editStack.pop();
    }
	// TODO
}
