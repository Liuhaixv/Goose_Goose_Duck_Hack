#pragma once
#include <d3d11.h>
#include <string>
#include "Common.hpp"
#include"../Enum/EnumGameMaps.hpp"

struct GameMap {
    GameMap(const char8_t* eng, const char8_t* cn) {
        this->engName = eng;
        this->cnName = cn;
    }

    const char8_t* engName = nullptr;
    const char8_t* cnName = nullptr;
    ID3D11ShaderResourceView* texture = nullptr;
    int width = 0;
    int height = 0;

    //坐标相对图片偏移
    Vector2 offset{0.0f, 0.0f};
    //坐标相对图片缩放
    float scale = 1.0f;
};