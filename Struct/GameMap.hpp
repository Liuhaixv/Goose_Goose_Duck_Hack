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
    /// ImGuiIO& io; //显示器屏幕坐标
    /// ImVector2& pos; //窗口组件坐标
    /// io.MousePos.x - pos.x;
    /// </summary>
    /// <param name="screenRelativePoint"></param>
    /// <returns></returns>
    Vector2 screenPointToPositionIngame(Vector2 screenRelativePoint) {
        //因为游戏内坐标很小，所以这里坐标换算中先处理缩放

        float region_x = screenRelativePoint.x;
        //反向Y轴
        float region_y = screenRelativePoint.y;

        if (region_x < 0.0f) { region_x = 0.0f; }
        if (region_y < 0.0f) { region_y = 0.0f; }
                
        region_x *= this->scaleToGamePosition;
        region_y *= this->scaleToGamePosition;

        region_x += this->offset.x;
        region_y += this->offset.y;

        return { region_x, region_y };
    }

    Vector2 positionIngameToScreenPoint(Vector2 positionIngame) {
        float region_x = positionIngame.x;
        float region_y = positionIngame.y;

        region_x -= this->offset.x;
        region_y -= this->offset.y;

        region_x /= this->scaleToGamePosition;
        region_y /= this->scaleToGamePosition;

        region_y *= -1;

        return { region_x, region_y };
    }

    //坐标相对图片偏移
    Vector2 offset{0.0f, 0.0f};
    //坐标相对图片缩放。图片坐标 * scale + offset => 游戏内坐标
    float scaleToGamePosition = 1.0f;

    //显示图片的缩放
    float scaleToDisplay = 1.0f;
};