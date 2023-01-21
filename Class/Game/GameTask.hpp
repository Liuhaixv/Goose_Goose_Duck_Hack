#pragma once
#include<../memory.hpp>
#include "../../Data/offsets.hpp"

extern Memory memory;
class GameTask {
public:
    int64_t taskId = NULL;//string
    int64_t taskDisplayName = NULL;//string

    GameTask(IN int64_t address) {
        if (address == NULL) {
            return;
        }

        this->address = address;
        this->update();
    }

    bool update(int64_t address) {
        if (address == NULL) {
            return false;
        }

        this->address = address;
        return this->update();
    }

private:
    int64_t address = NULL;

    bool update() {
        //TODO: memory.read<int64_t>(this->address, NULL) != NULL
        if (this->address == NULL) {
            return false;
        }

        this->taskId = memory.read_mem<int64_t>(this->address + Offsets::GameTask::taskId, NULL);
        this->taskDisplayName = memory.read_mem<int64_t>(this->address + Offsets::GameTask::taskDisplayName, NULL);
    }
};