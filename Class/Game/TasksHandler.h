#pragma once
#include <vector>
#include <string>

class TasksHandler
{
public:
    int64_t address = NULL;

    int tasksNum = 0;
    std::vector<GameTask> assignedTasks;// 保存所有tasks的string
    std::vector<std::string> assignedTasks_string;//保存所有tasks的std::string

    bool update();
    void reset();//TODO
private:

};

