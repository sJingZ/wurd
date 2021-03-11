#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
//    if (!editStack.empty() && editStack.top()->act == action){
//        EDIT* topEdit = editStack.top();
//        if(topEdit->act == Action::INSERT &&
//           topEdit->r == row &&
//           topEdit->c == col-1){ // insert batching
//            topEdit->c = col;
//            topEdit->count++;
//            topEdit->value+= ch;
//        }
//        else if(topEdit->act == Action::DELETE &&
//                topEdit->r == row &&
//                topEdit->c == col-1){ // backspace batching wait but there is no way to check back vs. del
//                 topEdit->c = col;
//                 topEdit->count++;
//                 topEdit->value+= ch;
//        /// prob now: insert and delete has different ways of determining batching
//        }
//    }
//
//    // no batchings
//    else{
//        EDIT* lastEdit = new EDIT;
//        lastEdit->act = action;
//        lastEdit->r = row;
//        lastEdit->c = col;
//        lastEdit->value = ch;
//        lastEdit->count = 1;
//        editStack.push(lastEdit);
//    }
    
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

    
    
//    if(!editStack.empty() &&
//       lastEdit->act == editStack.top()->act &&
//       lastEdit->r == editStack.top()->r){ // check batching
//        switch (lastEdit->act) {
//            case Undo::Action::INSERT:
//                if(col == editStack.top()->c+1){
//                    editStack.top()->count++;
////                    lastEdit->isBatched = true;
//                    return;
//                }
//                break;
//            case Undo::Action::DELETE:{
////                if(col == editStack.top()->c ||
////                   col == editStack.top()->c-1){
////                    lastEdit->isBatched = true;
////                }
//                if (col == editStack.top()->c){
//                    editStack.top()->value += ch;
//                    return;
//                }
//                else if (col == editStack.top()->c-1){
//                    editStack.top()->c = col;
//                    editStack.top()->value.insert(0,1,ch);
//                    return;
//                }
//            }
//        }
//    }
//    else{lastEdit->isBatched = false;}
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

//            count++;
//            if(topEdit->isBatched){
//                get(row, col, count, text);
//            }
            
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
            
        default:
            break;
    }
    
    
    
    
    //    if(row == -100){ // the first time calling this function
//        row = topEdit->r;
//        col = topEdit->c;
////        text = topEdit->value;
////        count++;
//        editStack.pop();
//        if(topEdit->isBatched){
//            get(row, col, count, text);
//        }
////        if(!editStack.empty() &&
////           topEdit->act == editStack.top()->act &&
////           topEdit->r == editStack.top()->r){ // always check if the second one is batching
////            get(row, col, count, text);
////        }
////        switch (topEdit->act) {
////            case Undo::Action::INSERT:{
////                col = topEdit->c;
////                return Undo::Action::DELETE;
////                break;
////            }
////            case Undo::Action::DELETE:{
////                col = topEdit->c;
////                return Undo::Action::INSERT;
////                break;
////            }
////            default:
////                break;
////        }
//    }
    
    
    
    
    
    
    
    
//    switch (topEdit->act) {
//        case Undo::Action::INSERT:{
//            if(topEdit->c == col-1){ // this is the batching case
//                row = topEdit->r;
//                col = topEdit->c;
//                text += topEdit->value;
//                count++;
//                editStack.pop();
//                if(!editStack.empty() &&
//                   topEdit->act == editStack.top()->act &&
//                   topEdit->r == editStack.top()->r){ // check next one is batching
//                    get(row, col, count, text);
//                }
//            }
////            col = topEdit->c;
//            return Undo::Action::DELETE;
//            break;
//        }
//        case Undo::Action::DELETE:{
//            if(topEdit->c == col ||
//               topEdit->c == col+1){ // this is the batching case
//                row = topEdit->r;
//                col = topEdit->c;
//                text += topEdit->value;
//                count++;
//                editStack.pop();
//                if(!editStack.empty() &&
//                   topEdit->act == editStack.top()->act &&
//                   topEdit->r == editStack.top()->r){ // check next one is batching
//                    get(row, col, count, text);
//                }
//            }
//            col = topEdit->c;
//            return Undo::Action::INSERT;
//            break;
//        }
//        default:
//            break;
//    }
    
    
    
    
    
    

    return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
	// TODO
}
