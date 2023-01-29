#pragma once
#define byte unsigned char
#define gap(name, size) byte name[size]{}

struct ACTkByte4 // TypeDefIndex: 15317
{
    // Fields
    byte b1; // 0x0
    byte b2; // 0x1
    byte b3; // 0x2
    byte b4; // 0x3

    ACTkByte4() {
        b1 = 0;
        b2 = 0;
        b3 = 0;
        b4 = 0;
    }

    ACTkByte4(float value) {
        byte* pByte = (byte*)&value;
        b4 = *pByte++;
        b3 = *pByte++;
        b2 = *pByte++;
        b1 = *pByte;
    }

    ACTkByte4(int value) {
        b1 = value & 0xFF;
        b2 = (value >>= 8) & 0xFF00;
        b3 = (value >>= 8) & 0xFF0000;
        b4 = (value >>= 8) & 0xFF000000;
    }

    ACTkByte4 operator ^ (ACTkByte4 bytes4) {
        ACTkByte4 temp;
        temp.b1 = this->b1 ^ bytes4.b1;
        temp.b2 = this->b2 ^ bytes4.b2;
        temp.b3 = this->b3 ^ bytes4.b3;
        temp.b4 = this->b4 ^ bytes4.b4;

        return temp;
    }

    ACTkByte4 operator ^ (int intValue) {
        return ACTkByte4::operator^(ACTkByte4(intValue));
    }

    operator int() const {
        int value = 0;
        value |= (b4 & 0xFF);
        value |= (b3 & 0xFF00);
        value |= (b2 & 0xFF0000);
        value |= (b1 & 0xFF000000);
        return value;
    }

    void Shuffle() {
        b2 ^= b3;
        b3 ^= b2;
        b2 ^= b3;
    }

    void Unshuffle() {
        Shuffle();
    }
};

struct ObscuredFloat {
    int currentCryptoKey = 0; // 0x0 
    int hiddenValue = 0; // 0x4 shuffle后的hiddenValueOldByte4
    ACTkByte4 hiddenValueOldByte4 = 0; // 0x8
    bool inited = 0; // 0xC
    gap(gap_0, 3);
    float fakeValue = 0.0f; // 0x10
    bool fakeValueActive = 0; // 0x14

    ObscuredFloat() {
    }

    ObscuredFloat(int cryptoKey, float value) {
        this->currentCryptoKey = cryptoKey;
        this->Encrypt(value);
    }

    /// <summary>
    /// currentCryptoKey ^ value => hiddenValueOldByte4
    /// hiddenValueOldByte4.Shuffle()
    /// hiddenValueOldByte4 => hiddenValue
    /// </summary>
    /// <param name="value"></param>
    void Encrypt(float value) {
        hiddenValueOldByte4 = ACTkByte4(value) ^ currentCryptoKey;
        hiddenValueOldByte4.Shuffle();
        hiddenValue = hiddenValueOldByte4;
        hiddenValueOldByte4 = 0;

        if (fakeValueActive) {
            fakeValue = value;
        }
    }

    /// <summary>
    /// hiddenValue => hiddenValueOldByte4
    /// hiddenValueOldByte4.Unshuffle()
    /// hiddenValueOldByte4 ^ currentCryptKey => {return value}
    /// </summary>
    /// <returns></returns>
    float Decrypt() {
        ACTkByte4 temp = this->hiddenValue;
        temp.Unshuffle();
        return temp ^ currentCryptoKey;
    }
};

struct ObscuredInt
{
    int currentCryptoKey = 0; // 0x0
    int hiddenValue = 0; // 0x4
    bool inited = 0; // 0x8
    gap(gap_0, 3);
    int fakeValue = 0; // 0xC
    bool fakeValueActive = 0; // 0x10

    ObscuredInt() {
    }

    ObscuredInt(int cryptoKey, int value) {
        this->currentCryptoKey = cryptoKey;
        this->Encrypt(value);
    }

    int Decrypt() {
        return currentCryptoKey ^ hiddenValue;
    }

    void Encrypt(int value) {
        hiddenValue = currentCryptoKey ^ value;
        if (fakeValueActive) {
            fakeValue = value;
        }
    }
};