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

    /// <summary>
    /// 图片最左下角 = (0, 0)
    /// Y↑
    /// X→
    /// <returns></returns>
    /// </summary>
    Vector2 relativePositionLeftBottom_to_PositionInGame(const Vector2 &relativePosition) {
        //因为游戏内坐标很小，所以这里坐标换算中先处理缩放

        float x = relativePosition.x;
        float y = relativePosition.y;

        //先将图片的分辨率px缩放到接近游戏内坐标的比例                       
        x = x * this->scaleToGamePosition / this->scaleToDisplay;
        y = y * this->scaleToGamePosition / this->scaleToDisplay;

        //地图的最左下角并不是从(0, 0)坐标开始的
        //所以这里需要加上对应的偏移
        x += this->offset.x;
        y += this->offset.y;

        return { x, y };
    }


    Vector2 positionInGame_to_relativePositionLeftBottom(const Vector2 &positionIngame) {
        float x = positionIngame.x;
        float y = positionIngame.y;

        //首先减去游戏内最左下角的偏移, 使得左下角与(0, 0)对齐
        x -= this->offset.x;
        y -= this->offset.y;

        x = x * this->scaleToDisplay / this->scaleToGamePosition;
        y = y * this->scaleToDisplay / this->scaleToGamePosition;

        return { x, y };
    }

    //坐标相对图片偏移
    Vector2 offset{ 0.0f, 0.0f };
    //坐标相对图片缩放。图片坐标 * scale + offset => 游戏内坐标
    float scaleToGamePosition = 1.0f;

    //显示图片的缩放
    float scaleToDisplay = 1.0f;
};