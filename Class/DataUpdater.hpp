#pragma once

#include<list>

#include"../memory.hpp"
#include"PlayerController.hpp"

/// <summary>
/// Thread functions that update data.
/// </summary>
class DataUpdater {
public:
	DataUpdater(Memory memory) {
		this->memory = &memory;
	}

	int validPlayersNum = 0;

	/// <summary>
	/// 更新玩家信息的线程函数
	/// Thread that keeps updating data of PlayerControllers using RPM
	/// </summary>
	/// <param name="playerControllers"></param>
	void playerControllerUpdater(std::list<PlayerController> *playerControllers) {
		validPlayersNum = 0;

		while (true) {
			updatePlayerController(playerControllers);
			Sleep(1000);
		}
	}
private:
	Memory* memory;

	/// <summary>
	/// 更新玩家PlayerController信息
	/// </summary>
	/// <param name="playerIndex">玩家的索引，范围为[0,15],当index为0时为LocalPlayer</param>
	/// <param name="playerController">要更新的PlayerController</param>
	void updatePlayerController(std::list<PlayerController>* playerControllers) {
		int validPlayers = 0;

		//遍历所有PlayerController
		std::list<PlayerController>::iterator iterator = playerControllers->begin();
		for (int i = 0; iterator != playerControllers->end(); ++iterator, ++i) {
			std::vector<int64_t> offsets = Offsets::GameAssembly::playerControllerByIndex(i);
			int64_t playerControllerAddr = memory->FindPointer( memory->gameAssemblyBaseAddress, offsets);

			//更新玩家信息
			bool isPlayerValid = playerControllerAddr != NULL;

			//有效玩家信息，读取内存并更新
			if (isPlayerValid) {
				(*iterator).update(playerControllerAddr);
				//更新玩家坐标
				(*iterator).updatePosition(playerControllerAddr);

				validPlayers++;
			}
			else {
				//无效玩家信息，且没有重置数据
				if ((*iterator).valid) {
					(*iterator).markAsInvalidPlayer();
					(*iterator).reset();

					//Local player not valid, no need to read further data
					if (i == 0) {
						return;
					}
				}
				else {
					//无效玩家信息，已重置过数据
				}
			}
		}

		//更新有效玩家信息
		this->validPlayersNum = validPlayers;
	}
};