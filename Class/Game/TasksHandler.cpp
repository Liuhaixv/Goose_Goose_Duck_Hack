#include "TasksHandler.h"
#include "../../memory.hpp"
#include"../../Data/offsets.hpp"

extern Memory memory;
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
    int entries_Addr = memory.read_mem<int64_t>(assignedTasks_Addr + 0x18, NULL);

    if (entries_Addr == NULL) {
        return false;
    }

    if (count > 0) {
        //System.string类型
        std::vector<int64_t> tasksStr;
        int64_t p_Entry = entries_Addr = 0x28;
        for (int i = 0; i < count; i++, entries_Addr+=0x18) {
            //获取entries中的GameTask对象

            //System.String
            int64_t str_Addr = memory.read_mem<int64_t>(entries_Addr, NULL);
            int64_t gameTask_Addr = memory.read_mem<int64_t>(entries_Addr, NULL);
            //TODO: 创建GameTask类，并将GameTask存入assigendTasks中
        }
    }
    else {
        this->assignedTasks.clear();
        this->assignedTasks_string.clear();
    }
}