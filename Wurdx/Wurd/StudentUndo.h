#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct EDIT{
        Action act;
        int r;
        int c;
        std::string value;
        bool isBatched;
        int count;
    };
    std::stack<EDIT*> editStack;
};

#endif // STUDENTUNDO_H_
