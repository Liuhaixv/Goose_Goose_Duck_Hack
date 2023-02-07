#pragma once

class LocalPlayer;
class PlayerController;
class Client;

class Hack {
public:
    //Must set client
    Hack() {
    }

    Hack(Client* client) {
        this->client = client;
    }

    void setClient(Client* client) {
        this->client = client;
    }

    /// <summary>
    /// 重置当前激活状态
    /// </summary>
    void resetActivationStates();

    //只需要激活一次
    void removeFogOfWar(PlayerController* localPlayerController);

    /// <summary>
    /// 开启穿墙模式
    /// Enable noclip to disable collider, which makes you able to walk through any obstacle like walls and tables<paragm/>
    /// </summary>
    /// <param name="localPlayer"></param>
    /// <param name="enable"></param>
    bool enableNoclip(PlayerController* localPlayer, bool shouldEnable);

    void noclip(PlayerController* localPlayerController);

    bool speedHack(LocalPlayer* localPlayer);

    void zoomHack(LocalPlayer* localPlayer);

    /// <summary>
    /// 设置相机的远近距离
    /// </summary>
    /// <param name="size"></param>
    /// <returns></returns>
    bool overrideOrthographicSize(LocalPlayer* localPlayer, float targetZoomSize);

private:
    Client* client = nullptr;

    bool b_hasDisabledFOW = false;
    bool b_hasEnabledNoclip = false;

    //上一次设置移速hack是否开启
    bool b_enabledSpeedHackLastTime = false;
    //上一次设置的玩家移速
    float f_lastTimeSetMovementSpeed = -1;
    float f_lastTimeZoomSize = -1;
};