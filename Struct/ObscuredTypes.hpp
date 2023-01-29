#pragma once
#define byte unsigned char
#define gap(name, size) byte name[size]

struct ACTkByte4 // TypeDefIndex: 15317
{
    // Fields
    byte b1; // 0x0
    byte b2; // 0x1
    byte b3; // 0x2
    byte b4; // 0x3
};

struct ObscuredFloat {
    int currentCryptoKey; // 0x0 
    int hiddenValue; // 0x4
    ACTkByte4 hiddenValueOldByte4; // 0x8
    bool inited; // 0xC
    gap(gap_0, 3);
    float fakeValue; // 0x10
    bool fakeValueActive; // 0x14
};

struct ObscuredInt
{
    int currentCryptoKey; // 0x0
    int hiddenValue; // 0x4
    bool inited; // 0x8
    gap(gap_0, 3);
    int fakeValue; // 0xC
    bool fakeValueActive; // 0x10
};