#pragma once

#include<vector>

namespace Offsets {
	namespace PlayerController {
		constexpr int64_t fn_updateRVA = 0xE12760; // AOBScan too.
		constexpr int64_t fl_isSilenced = 0xC1; // Boolean
		constexpr int64_t fl_isInfected = 0xC3; // Boolean
		constexpr int64_t fl_playerRoleId = 0xE8; // point64_ter, [instance+playerRoleId] + 0x10, int64_t
		constexpr int64_t fl_isPlayerRoleSet = 0xF0; // Boolean
		constexpr int64_t fl_inVent = 0xF1; // Boolean
		constexpr int64_t fl_facingRight = 0x130; // Boolean, can make "Moon walk".
		constexpr int64_t fl_hasBomb = 0x134; // Boolean
		constexpr int64_t fl_isGhost = 0x188; // Boolean
		constexpr int64_t fl_isLocal = 0x1C0; // Boolean
		constexpr int64_t fl_nickname = 0x1D0; // point64_ter, [instance+nickname] +  0x14, length: [instance+nickname] + 0x10
		constexpr int64_t fl_invisibilityDistance = 0x32C; // int64_t, need this?
		constexpr int64_t fl_isSpectator = 0x37A; // Boolean
		constexpr int64_t fl_isRemoteSpectating = 0x37B; // Boolean
		constexpr int64_t fl_position = 0x2C8; // Value name is randomized. x, y. Float, Float
		constexpr int64_t fl_idleTime = 0x2E4; // float
		/*
		[Notable offsets]
		2B0, Player location, Float Float,   x, y
		2BC, using left bool. camera accel?
		Using UnityEngine-Vector3.Slerp for camera movement?
		*/


	}

	namespace GameAssembly {
		/// <summary>
		/// 通过下标返回PlayerController的偏移数组<para/>
		/// Returns int64_t array of offsets of PlayerController by its index
		/// </summary>
		/// <param name="index">Index of PlayerController, range=[0,15]</param>
		/// <returns>int64_t array</returns>
		static std::vector<int64_t> playerControllerByIndex(int64_t index) {
			int64_t specialOffset = 0x30;
			specialOffset += index * 0x18;
			std::vector<int64_t> offsets = { 0x3c869B0, 0xB8, 0x20, 0x18, specialOffset, 0x0 };

			return offsets;
		}
	}
}