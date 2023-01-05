#pragma once

#include "memory.hpp"
class Client
{
public:


	Client(Memory memory)
	{
		this->memory = &memory;
	}

	/*
	int get_crosshair_status()
	{

		DWORD offsetToGameBaseAdress = 0x004FB4D4;
		int crosshairStatusOffsets[] = { 0x8, 0x24, 0x2c, 0x8, 0x8, 0x1494, 0x8 };

		DWORD crosshairStatusAddress = NULL;

		crosshairStatusAddress = memory->FindPointer(7, memory->gameAssemblyBaseAddress + offsetToGameBaseAdress, crosshairStatusOffsets);

		int crosshair_status = memory->read_mem<int>(crosshairStatusAddress);
		return crosshair_status;
	}

	// use simulate mouse，may be replaced with writing memory method
	void shoot()
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); //按下左键
		Sleep(20);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //松开左键
	}

	Vector3 get_camera_angles()
	{
		DWORD offsetToGameBaseAdress = 0x4fb4d4;
		int offsets[] = { 0x8, 0x24, 0x2c, 0x8, 0x94, 0x3c };

		DWORD camera_angles_addr = memory->FindPointer(6, memory->gameAssemblyBaseAddress + offsetToGameBaseAdress, offsets);

		return memory->read_mem<Vector3>(camera_angles_addr);
	}

	//超大视野 0 -2.2 0.0000001
	void set_camera_angles(Vector3 angles)
	{

		DWORD offsetToGameBaseAdress = 0x4fb4d4;
		int offsets[] = { 0x8, 0x24, 0x2c, 0x8, 0x94, 0x3c };

		DWORD camera_angles_addr = memory->FindPointer(6, memory->gameAssemblyBaseAddress + offsetToGameBaseAdress, offsets);

		memory->write_mem(camera_angles_addr, angles);
	}
	*/

private:
	Memory* memory = 0;
};