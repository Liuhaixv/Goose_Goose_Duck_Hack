#pragma once
#include <vector>
#include <string>
#include "GameTask.hpp"

class TasksHandler
{
public:
    int64_t address = NULL;

    int tasksNum = 0;
    std::vector<GameTask> assignedTasks;// 保存所有tasks的string


    bool update(int64_t address);
    void reset();//TODO
    void resetMemberFields();//TODO
private:
    bool update();
    bool validateAddress(int64_t address);//TODO
};
