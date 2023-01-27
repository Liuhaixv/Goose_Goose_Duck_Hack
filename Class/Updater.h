#pragma once
class Updater
{
public:
    //暂停更新/写入内存
    bool paused = false;

    void pause() {
        this->paused = true;
    }

    void unpause() {
        this->paused = false;
    }
};

