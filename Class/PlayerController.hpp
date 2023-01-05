#pragma once
#pragma warning(disable:4996)

#include"../utils.hpp"
#include"../Class/offsets.hpp"
#include"../Struct/Common.hpp"

#include<Windows.h>

class PlayerController {
public:
	PlayerController(Memory memory) {
		this->memory = &memory;
		this->utils = new Utils;
	}

	~PlayerController() {
		delete this->utils;
	}


	//Mark if the player is valid(false if no player's info valid)
	//标记当前玩家槽位是否有效(当玩家中途离开或该槽位无人占用导致内存中无有效玩家数据时为false)
	bool valid = false;

	DWORD_PTR ptrPlayerController = 0;
	bool isSilenced = false;
	bool isInfected = false;
	int playerRoleId = 0;
	bool isPlayerRoleSet = false;
	bool inVent = false;
	bool hasBomb = false;
	bool isGhost = false;
	bool isLocal = false;
	int invisibilityDistance = 0;
	bool isSpectator = false;
	bool isRemoteSpectating = false;
	std::string nickname = "";
	char roleName[64] = "";
	Vector3 pos{ 0.0f, 0.0f, 0.0f };

	static void printAllPlayersInfo(std::list<PlayerController> playerControllers, Utils* utils = nullptr) {
		std::list<PlayerController>::iterator iterator = playerControllers.begin();

		const char separator = ' ';
		const int nameWidth = 15;

		std::cout << std::left << std::setw(20) << std::setfill(separator) << (utils ? utils->str("Nickname", "玩家昵称") : "玩家昵称");
		std::cout << std::left << std::setw(10) << std::setfill(separator) << (utils ? utils->str("Rolename", "角色") : "角色");
		std::cout << std::left << std::setfill(separator) << (utils ? utils->str("Pos", "坐标") : "坐标");
		std::cout << '\n';

		for (int i = 0; iterator != playerControllers.end(); ++iterator, ++i) {
			if (!(*iterator).valid) {
				continue;
			}

			std::cout << std::left << std::setw(20) << std::setfill(separator) << (*iterator).nickname;
			std::cout << std::left << std::setw(10) << std::setfill(separator) << (*iterator).roleName;
			std::cout << std::left << std::setfill(separator) << "(" << (*iterator).pos.x << ", " << (*iterator).pos.y << ")";
			std::cout << '\n';
		}
		std::cout << std::endl;
	}

	void markAsInvalidPlayer() {
		this->valid = false;
	}

	void markAsValidPlayer() {
		this->valid = true;
	}

	void reset() {
		ptrPlayerController = 0;
		isSilenced = false;
		isInfected = false;
		playerRoleId = -1;
		isPlayerRoleSet = false;
		inVent = false;
		hasBomb = false;
		isGhost = false;
		isLocal = false;
		invisibilityDistance = 0;
		isSpectator = false;
		isRemoteSpectating = false;
		nickname = "";
		roleName[0] = '\0';
		pos = { 0.0f, 0.0f };
	}

	/// <summary>
	/// 更新玩家坐标信息<para/>
	/// Update player's position and returns true if data valid 
	/// </summary>
	/// <param name="PlayerController"></param>
	/// <returns></returns>
	bool updatePosition(int64_t PlayerController) {

		//无效玩家数据
		//invalid data
		if (PlayerController == NULL) {
			markAsInvalidPlayer();
			return false;
		}

		if (PlayerController == ptrPlayerController) {
			pos = memory->read_mem<Vector3>(PlayerController + Offsets::PlayerController::fl_position);
		}

		return true;
	}

	//https://blog.csdn.net/mercy_ps/article/details/81218608
	std::string wstring2string(std::wstring wstr) {
		std::string result;

		int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];

		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
		buffer[len] = '\0';

		result.append(buffer);
		delete[] buffer;
		return result;
	}

	/// <summary>
	/// 更新玩家数据<para/>
	/// Update player's data and returns true if data valid 
	/// </summary>
	/// <param name="PlayerController"></param>
	/// <returns>玩家数据是否有效</returns>
	bool update(int64_t PlayerController) {

		//无效玩家数据
		//invalid data
		if (PlayerController == NULL) {
			markAsInvalidPlayer();
			return false;
		}
		else {
			ptrPlayerController = PlayerController;
			markAsValidPlayer();
		}

		wchar_t tmpNick[42] = L"";

		int64_t addr_0 = memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_nickname);
		byte* addr = (byte*)memory->read_mem<int64_t>(addr_0 + 0x14);
		int64_t size = sizeof(wchar_t) * memory->read_mem<int>(memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_nickname) + 0x10) + 1;

		memcpy(tmpNick, &addr, size);

		tmpNick[size] = 0;

		int _size = 0;
		//for (int i = 0; i < 64; i++) {
		//	if (tmpNick[i] == 0) {
		//		break;
		//	}
		//	_size++;
		//}

		std::wstring string_to_convert = std::wstring(reinterpret_cast<wchar_t*>(tmpNick), size);

		nickname = wstring2string(string_to_convert);

		pos = memory->read_mem<Vector3>(PlayerController + Offsets::PlayerController::fl_position);

		isPlayerRoleSet = memory->read_mem<bool>(Offsets::PlayerController::fl_isPlayerRoleSet);
		isLocal = memory->read_mem<bool>(Offsets::PlayerController::fl_isLocal);
		inVent = memory->read_mem<bool>(Offsets::PlayerController::fl_inVent);
		hasBomb = memory->read_mem<bool>(Offsets::PlayerController::fl_hasBomb);
		isGhost = memory->read_mem<bool>(Offsets::PlayerController::fl_isGhost);
		isSpectator = memory->read_mem<bool>(Offsets::PlayerController::fl_isSpectator);
		invisibilityDistance = memory->read_mem<int>(Offsets::PlayerController::fl_invisibilityDistance);
		isRemoteSpectating = memory->read_mem<bool>(Offsets::PlayerController::fl_isRemoteSpectating);

		//strcat(roleName, utils.getRoleName(playerRoleId));

		if (isPlayerRoleSet) {
			playerRoleId = memory->read_mem<int>(memory->read_mem<int64_t>(PlayerController + Offsets::PlayerController::fl_playerRoleId) + 0x10);
			strcpy(roleName, utils->getRoleName(playerRoleId));
		}

		return true;
	}

private:

	Utils* utils;
	Memory* memory = 0;
};