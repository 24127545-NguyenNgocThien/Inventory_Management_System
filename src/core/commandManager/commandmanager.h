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
    ~CommandManager();

    bool ExecuteCommand(Command* cmd);
    bool Undo();
    bool Redo();
};

#endif // COMMANDMANAGER_H
