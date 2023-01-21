#include "TasksHandler.h"
#include "../../memory.hpp"
#include"../../Data/offsets.hpp"

extern Memory memory;

//Public

bool TasksHandler::update(int64_t address) {
    if (address == NULL) {
        return false;
    }

    if (!validateAddress(address)) {
        return false;
    }

    if (address != this->address) {
        reset();
        this->address = address;
    }

    return update();
}

void TasksHandler::reset() {
    this->address = NULL;

    this->resetMemberFields();
}

void TasksHandler::resetMemberFields() {
    this->tasksNum = 0;
    this->assignedTasks.clear();
}


//Private

bool TasksHandler::update() {
    if (address == NULL) {
        return false;
    }

    int64_t assignedTasks_Addr = memory.read_mem<int64_t>(this->address + Offsets::TasksHandler::assignedTasks, NULL);

    if (assignedTasks_Addr == NULL) {
        return false;
    }

    //任务数量
    int count = memory.read_mem<int>(assignedTasks_Addr + 0x20, 0);
    int64_t entries_Addr = memory.read_mem<int64_t>(assignedTasks_Addr + 0x18, NULL);

    if (entries_Addr == NULL) {
        return false;
    }

    if (count > 0) {
        //System.string类型
        std::vector<int64_t> tasksStr;
        int64_t p_Entry = entries_Addr + 0x28;
        for (int i = 0; i < count; i++, p_Entry += 0x18) {
            //获取entries中的GameTask对象

            //System.String
            int64_t str_Addr = memory.read_mem<int64_t>(p_Entry, NULL);
            int64_t gameTask_Addr = memory.read_mem<int64_t>(p_Entry + 0x8, NULL);

            if (i >= this->assignedTasks.size()) {
                //空间不足，扩容vector
                this->assignedTasks.push_back(GameTask(gameTask_Addr));
            }
            else {
                this->assignedTasks[i].update(gameTask_Addr);
            }
        }
    }
    else {
        this->assignedTasks.clear();
    }

    this->tasksNum = count;
}

//TODO:
bool TasksHandler::validateAddress(int64_t address) {
    return true;
}