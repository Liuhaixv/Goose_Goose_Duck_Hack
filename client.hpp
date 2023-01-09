#pragma once

#include "memory.hpp"
#include "Struct/HackSettings.hpp"

class Client
{
public:
    HackSettings* hackSettings = nullptr;

	Client(Memory memory)
	{
		this->memory = &memory;
	}

    Client(Memory memory, HackSettings hackSettings)
    {
        this->memory = &memory;
        this->hackSettings = &hackSettings;
    }

    //enable noclip to disable collider, which makes you able to walk through any obstacle like walls and tables
    //穿墙模式
    //static void noclip(PlayerController* localPlayer, bool enable = true) {
    //
    //}

private:
	Memory* memory = 0;
};
