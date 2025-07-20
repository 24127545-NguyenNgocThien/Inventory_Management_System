#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "command.h"
#include <stack>

class CommandManager
{
private:
    std::stack<Command*> undoStack;
    std::stack<Command*> redoStack;

public:
    CommandManager();

    bool ExecuteCommand(Command* cmd);
    void Undo();
    void Redo();
};

#endif // COMMANDMANAGER_H
