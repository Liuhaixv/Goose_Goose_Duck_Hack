#pragma once

#pragma warning(disable:4996)

#include<Windows.h>

#include<string>
#include<list>
#include<vector>
#include<iomanip>
#include<iostream>

#include"Enum/ActivationState.hpp"

//编码转换
#include<codecvt>

#include"Data/GameData.hpp"

class Utils {
public:
    //当前操作系统是否为中文
    //true if current OS's language is Chinese
    bool b_useChineseLanguage = 0;

    Utils();

    void changeLanguage();

    /// <summary>
    /// 检查currentVersion是否为最新
    /// </summary>
    /// <returns></returns>
    /// https://www.geeksforgeeks.org/compare-two-version-numbers/
    bool isLatestHackVersion(std::string currentVersion, std::string versionFromServer) {
        return versionCompare(currentVersion, versionFromServer) >= 0;
    }

    /// <summary>
    /// 判断两个bool的值来确定状态是否改变。只适用于确定功能一次即可激活开关的类型，不适用于需要循环判断或具有定时开关功能的场景。
    /// </summary>
    /// <param name="settingEnabled"></param>
    /// <param name="OUT"></param>
    /// <returns></returns>
    ActivationState shouldActivateOnce(IN const bool& settingEnabled, IN OUT bool* b_activatedAlready) {
        //当前启用
        if (settingEnabled)
        {
            //之前没有激活启用，说明现在是刚刚启用，此时返回1
            if (!(*b_activatedAlready)) {
                //之前没激活过，现在已经是激活的状态了
                *b_activatedAlready = true;

                return ActivationState::SHOULD_ACTIVATE_NOW;
            }
        }
        else {
            //之前是激活启用状态，说明现在是刚刚禁用，此时返回True
            if (*b_activatedAlready) {
                *b_activatedAlready = false;

                return ActivationState::SHOULD_DEACTIVATE_NOW;
            }
        }

        //已经激活过或者一直没有激活过
        return ActivationState::IDLE_DO_NOTHING;
    }

    /// <summary>
    /// 判断float值是否变化，只返回SHOULD_ACTIVATE_NOW或IDLE_DO_NOTHING
    /// </summary>
    /// <param name="settingEnabled"></param>
    /// <param name="OUT"></param>
    /// <returns></returns>
    ActivationState shouldActivateOnce(IN const float& f_currentTimeValue, IN OUT float* f_lastTimeValue) {
        //数值未变化
        if (f_currentTimeValue == *f_lastTimeValue)
        {
            *f_lastTimeValue = f_currentTimeValue;
            return ActivationState::IDLE_DO_NOTHING;
        }
        //数值变化
        else {
            *f_lastTimeValue = f_currentTimeValue;
            return ActivationState::SHOULD_ACTIVATE_NOW;
        }
    }

    /// <summary>
    /// 转换utf16到utf8
    /// </summary>
    /// <param name="source"></param>
    /// <returns></returns>
    std::string u8From16(char16_t* source) {
        //UTF-16 to UTF-8 
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(source);
    }

    //https://blog.csdn.net/mercy_ps/article/details/81218608
    static std::string wstring2string(std::wstring wstr) {
        std::string result;

        int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
        char* buffer = new char[len + 1];

        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
        buffer[len] = '\0';

        result.append(buffer);
        delete[] buffer;
        return result;
    }

    //转换本地编码string为utf8编码后的string
    std::string strToU8str(std::string& codepage_str) {

        int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, codepage_str.c_str(),
            codepage_str.length(), nullptr, 0);
        std::wstring utf16_str(size, '\0');
        MultiByteToWideChar(CP_ACP, MB_COMPOSITE, codepage_str.c_str(),
            codepage_str.length(), &utf16_str[0], size);

        int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
            utf16_str.length(), nullptr, 0,
            nullptr, nullptr);
        std::string utf8_str(utf8_size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
            utf16_str.length(), &utf8_str[0], utf8_size,
            nullptr, nullptr);

        return utf8_str;
    }

    /// <summary>
    /// 转换8字节地址为小端字节数组
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    std::vector<byte> addressToLittleEndianBytes(IN const int64_t& address) {
        std::vector<byte> bytes = {
            (byte)((address & 0x00000000000000ff) >> 0),
            (byte)((address & 0x000000000000ff00) >> 8),
            (byte)((address & 0x0000000000ff0000) >> 16),
            (byte)((address & 0x00000000ff000000) >> 24),
            (byte)((address & 0x000000ff00000000) >> 32),
            (byte)((address & 0x0000ff0000000000) >> 40),
            (byte)((address & 0x00ff000000000000) >> 48),
            (byte)((address & 0xff00000000000000) >> 56)
        };

        return bytes;
    }

    /// <summary>
    /// 转换4字节地址为小端字节数组
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    std::vector<byte> addressToLittleEndianBytes(IN const int32_t& address) {
        std::vector<byte> bytes = {
            (byte)((address & 0x000000ff) >> 0),
            (byte)((address & 0x0000ff00) >> 8),
            (byte)((address & 0x00ff0000) >> 16),
            (byte)((address & 0xff000000) >> 24)
        };

        return bytes;
    }

    template<typename var>
    std::vector<var> combineVectors(IN std::initializer_list<std::vector<var>> vectors) {
        std::vector<var> result;
        //遍历拼接Vector
        for (auto v : vectors) {
            result.insert(result.end(), v.begin(), v.end());
        }
        return result;
    }

    /// <summary>
    /// 输出字符串到控制台,如果没有指定第二个参数则默认输出第一个字符串参数。
    /// </summary>
    /// <param name="eng">英文字符串</param>
    /// <param name="cn">中文字符串</param>
    /*
    void print(std::string eng, std::string cn = "") {
        if (cn == "") {
            std::cout << eng;
        }
        else {
            if (this->b_chineseOS) {
                std::cout << cn;
            }
            else {
                std::cout << eng;
            }
        }
    }
    */

    /// <summary>
    /// 返回中文或英文字符串
    /// </summary>
    /// <param name="eng"></param>
    /// <param name="cn"></param>
    /// <returns></returns>
    std::string str(std::string eng, std::string cn) {
        return (this->b_useChineseLanguage) ? cn : eng;
    }

    /// <summary>
    /// 关闭快速输入模式防止程序卡住
    /// Disable quick-edit in console in case that user input stuck the program
    /// </summary>
    static void disableQuickEdit() {
        //使控制台无法输入，防止因为输入而卡住程序
        DWORD prev_mode;
        HANDLE hStidin = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hStidin, &prev_mode);
        SetConsoleMode(hStidin, ENABLE_EXTENDED_FLAGS);
    }

    const char8_t* getRoleName(int id);

private:

    int versionCompare(std::string v1, std::string v2)
    {
        // vnum stores each numeric 
        // part of version 
        int vnum1 = 0, vnum2 = 0;

        // loop until both string are 
        // processed 
        for (int i = 0, j = 0; (i < v1.length()
            || j < v2.length());) {
            // storing numeric part of 
            // version 1 in vnum1 
            while (i < v1.length() && v1[i] != '.') {
                vnum1 = vnum1 * 10 + (v1[i] - '0');
                i++;
            }

            // storing numeric part of 
            // version 2 in vnum2 
            while (j < v2.length() && v2[j] != '.') {
                vnum2 = vnum2 * 10 + (v2[j] - '0');
                j++;
            }

            if (vnum1 > vnum2)
                return 1;
            if (vnum2 > vnum1)
                return -1;

            // if equal, reset variables and 
            // go for next numeric part 
            vnum1 = vnum2 = 0;
            i++;
            j++;
        }
        return 0;
    }

    /// <summary>
    /// 检查当前系统是否使用中文
    /// </summary>
    /// <returns>是否使用中文</returns>
    static bool isChineseLanguageOS() {
        try {
            std::string locale = __CheckLocale();
            if (locale.find("zh") != std::string::npos || locale.find("CN") != std::string::npos){
                return true;
                //if (locale.find("zh") != std::string::npos || locale.find("CN") != std::string::npos) {
             
            }
            return false;
        }
        catch (...) {
            return false;
        }
    }

    static std::string __CheckLocale()
    {
        //LCID 实际是unsignedlong类型，所以可以通过LCID编码来比较是什么语言
        LCID lcid = GetThreadLocale();
        wchar_t name[LOCALE_NAME_MAX_LENGTH];
        if (LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) == 0) {
            // "Get locale name error"
            return "";
        }
        std::wstring ws(name);
        std::string str(ws.begin(), ws.end());
        return str;
    }
};