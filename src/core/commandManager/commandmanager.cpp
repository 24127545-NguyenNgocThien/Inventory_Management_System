#include "commandmanager.h"

CommandManager::CommandManager() {}


bool CommandManager::ExecuteCommand(Command* cmd)
{
    bool res = cmd->Execute();
    if(res)
    {
        undoStack.push(cmd);
        while(!redoStack.empty())
        {
            redoStack.pop();
        }

        return true;
    }

    return false;
}

CommandManager::~CommandManager()
{
    while(!undoStack.empty())
    {
        delete undoStack.top();
        undoStack.pop();
    }
    while(!redoStack.empty())
    {
        delete redoStack.top();
        redoStack.pop();
    }
}

bool CommandManager::Undo()
{
    if(!undoStack.empty())
    {
        Command* cmd = undoStack.top();
        undoStack.pop();
        cmd->Undo();

        redoStack.push(cmd);

        return true;
    }
    return false;
}

bool CommandManager::Redo()
{
    if(!redoStack.empty())
    {
        Command* cmd = redoStack.top();
        redoStack.pop();
        cmd->Execute();

        undoStack.push(cmd);

        return true;
    }
    return false;
}
