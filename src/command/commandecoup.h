#pragma once

class CommandeCoup {
public:
    virtual void executer() = 0;
    virtual void annuler() = 0;
    virtual ~CommandeCoup() = default;
};