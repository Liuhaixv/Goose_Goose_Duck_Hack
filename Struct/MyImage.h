#pragma once
#include <d3d11.h>

struct MyImage {
    ID3D11ShaderResourceView* texture = nullptr;
    int width = 0;
    int height = 0;
};