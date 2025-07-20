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

void CommandManager::Undo()
{
    if(!undoStack.empty())
    {
        Command* cmd = undoStack.top();
        undoStack.pop();
        cmd->Undo();

        redoStack.push(cmd);

        delete cmd;
    }
}

void CommandManager::Redo()
{
    if(!redoStack.empty())
    {
        Command* cmd = redoStack.top();
        redoStack.pop();
        cmd->Execute();

        undoStack.push(cmd);

        delete cmd;
    }
}
