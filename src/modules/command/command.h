#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
    virtual ~Command() = default;

    virtual bool Execute() = 0;
    virtual void Undo() = 0;
};

#endif // COMMAND_H
