#pragma once

#pragma warning(disable:4996)

#include<string>
#include<list>
#include<iomanip>
#include<winnls.h>
#include<iostream>

#include"Enum/ActivationState.hpp"

//编码转换
#include<codecvt>

#include"Data/GameData.hpp"

class Utils {
public:
    //当前操作系统是否为中文
    //true if current OS's language is Chinese
    bool b_chineseOS = false;

    Utils() {
        b_chineseOS = isChineseLanguageOS();
    }

    ActivationState shouldActivateOnce(IN const bool& settingEnabled,IN OUT bool* b_activatedAlready) {
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
    /// 转换utf16到utf8
    /// </summary>
    /// <param name="source"></param>
    /// <returns></returns>
    std::string u8From16(char16_t* source) {
        //UTF-16 to UTF-8 
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(source);
    }

    ///*https://blog.csdn.net/mercy_ps/article/details/81218608
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
    //*/

    /// <summary>
    /// 输出字符串到控制台,如果没有指定第二个参数则默认输出第一个字符串参数。
    /// </summary>
    /// <param name="eng">英文字符串</param>
    /// <param name="cn">中文字符串</param>
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

    /// <summary>
    /// 返回中文或英文字符串
    /// </summary>
    /// <param name="eng"></param>
    /// <param name="cn"></param>
    /// <returns></returns>
    std::string str(std::string eng, std::string cn) {
        return (this->b_chineseOS) ? cn : eng;
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

    const char8_t* getRoleName(int id) {
        //返回中文角色名
        if (this->b_chineseOS) {
            switch (id)
            {
            case gameRoleId::None:
                return(u8"无");
                break;
            case gameRoleId::Goose:
                return(u8"鹅");
                break;
            case gameRoleId::Duck:
                return(u8"鸭子");
                break;
            case gameRoleId::Dodo:
                return(u8"呆呆鸟");
                break;
            case gameRoleId::Bounty:
                return(u8"赏金");
                break;
            case gameRoleId::Mechanic:
                //可以使用管道(该角色已被开发者移除)
                return(u8"Mechanic");
                break;
            case gameRoleId::Technician:
                //可以在迷你地图上看到破坏的大约位置(该角色已被开发者移除)
                return(u8"Technician");
                break;
            case gameRoleId::Medium:
                return(u8"通灵");
                break;
            case gameRoleId::Vigilante:
                return(u8"正义使者");
                break;
            case gameRoleId::Cannibal:
                return(u8"食鸟鸭");
                break;
            case gameRoleId::Morphling:
                return(u8"变形者(鸭子)");
                break;
            case gameRoleId::Sheriff:
                return(u8"警长");
                break;
            case gameRoleId::Silencer:
                return(u8"静语者(鸭子)");
                break;
            case gameRoleId::Canadian:
                return(u8"加拿大鹅");
                break;
            case gameRoleId::LoverDuck:
                return(u8"恋人(鸭子)");
                break;
            case gameRoleId::LoverGoose:
                return(u8"恋人(鹅)");
                break;
            case gameRoleId::Vulture:
                return(u8"秃鹫");
                break;
            case gameRoleId::Professional:
                return(u8"专业杀手(鸭子)");
                break;
            case gameRoleId::Spy:
                return(u8"间谍(鸭子)");
                break;
            case gameRoleId::Mimic:
                return(u8"模仿者");
                break;
            case gameRoleId::Detective:
                return(u8"侦探");
                break;
            case gameRoleId::Pigeon:
                return(u8"鸽子");
                break;
            case gameRoleId::Birdwatcher:
                return(u8"观鸟者");
                break;
            case gameRoleId::Assassin:
                return(u8"刺客(鸭子)");
                break;
            case gameRoleId::Falcon:
                return(u8"猎鹰");
                break;
            case gameRoleId::Hitman:
                return(u8"雇佣杀手(鸭子)");
                break;
            case gameRoleId::Bodyguard:
                return(u8"保镖鹅");
                break;
            case gameRoleId::Snitch:
                return(u8"告密者(鸭子)");
                break;
            case gameRoleId::Politician:
                return(u8"政治家");
                break;
            case gameRoleId::Locksmith:
                return(u8"锁匠");
                break;
            case gameRoleId::Mortician:
                return(u8"殡仪员");
                break;
            case gameRoleId::Celebrity:
                return(u8"网红");
                break;
            case gameRoleId::Party:
                return(u8"派对狂(鸭子)");
                break;
            case gameRoleId::Demolitionist:
                return(u8"爆炸王(鸭子)");
                break;
            case gameRoleId::DuelingDodo:
                return(u8"决斗呆呆鸟");
                break;
            case gameRoleId::GHGoose:
                return(u8"【猎鹅】鹅");
                break;
            case gameRoleId::GHDuck:
                return(u8"【猎鹅】鸭子");
                break;
            case gameRoleId::GHBounty:
                //你的任务是被第一个消灭，如果任务完成则可以获得奖励(该角色已被开发者移除)
                return(u8"【猎鹅】赏金鹅");
                break;
            case gameRoleId::HNSGoose:
                return(u8"【HNS】鹅");
                break;
            case gameRoleId::HNSDuck:
                return(u8"【HNS】鸭子");
                break;
            case gameRoleId::HNSBounty:
                return(u8"【HNS】赏金鹅");
                break;
            case gameRoleId::DNDDuck:
                return(u8"【霸王餐】鸭子");
                break;
            case gameRoleId::DNDFalcon:
                return(u8"【霸王餐】猎鹰");
                break;
            case gameRoleId::DNDVulture:
                return(u8"【霸王餐】秃鹫");
                break;
            case gameRoleId::DNDMorphling:
                return(u8"【霸王餐】变形鸭");
                break;
            case gameRoleId::FPGoose:
                return(u8"FPGoose");
                break;
            case gameRoleId::ExploreGoose:
                return(u8"ExploreGoose");
                break;
            case gameRoleId::TTVampire:
                return(u8"【TT】吸血鬼");
                break;
            case gameRoleId::TTPeasant:
                return(u8"【TT】村民");
                break;
            case gameRoleId::TTThrall:
                return(u8"【TT】鬼奴");
                break;
            case gameRoleId::Spectator:
                return(u8"旁观");
                break;
            case gameRoleId::IdentityThief:
                return(u8"身份窃贼");
                break;
            case gameRoleId::Adventurer:
                return(u8"冒险家");
                break;
            case gameRoleId::Avenger:
                return(u8"复仇者");
                break;
            case gameRoleId::Ninja:
                return(u8"忍者(鸭子)");
                break;
            case gameRoleId::Undertaker:
                return(u8"丧葬者");
                break;
            case gameRoleId::Snoop:
                //可以隐藏在地图的一些地方(该角色已被开发者移除)
                return(u8"Snoop");
                break;
            case gameRoleId::Esper:
                return(u8"超能力者");
                break;
            case gameRoleId::Invisibility:
                return(u8"隐身者");
                break;
            case gameRoleId::Astral:
                return(u8"星界行者");
                break;
            case gameRoleId::Pelican:
                return(u8"鹈鹕");
                break;
            case gameRoleId::TTEThrall:
                return(u8"【TTE】鬼奴");
                break;
            case gameRoleId::TTMummy:
                return(u8"【TT】木乃伊");
                break;
            case gameRoleId::SerialKiller:
                return(u8"连环杀手(鸭子)");
                break;
            case gameRoleId::Engineer:
                return(u8"工程师");
                break;
            case gameRoleId::Warlock:
                return(u8"术士(鸭子)");
                break;
            case gameRoleId::StreetUrchin:
                return(u8"流浪儿童");
                break;
            case gameRoleId::Tracker:
                return(u8"追踪者");
                break;
            case gameRoleId::EsperDuck:
                return(u8"超能力者(鸭子)");
                break;
            case gameRoleId::Stalker:
                return(u8"追踪者");
                break;
            default:
                return(u8"无");
                break;
            }
        }
        else {
            switch (id)
            {
            case gameRoleId::None:
                return(u8"None");
                break;
            case gameRoleId::Goose:
                return(u8"Goose");
                break;
            case gameRoleId::Duck:
                return(u8"Duck");
                break;
            case gameRoleId::Dodo:
                return(u8"Dodo");
                break;
            case gameRoleId::Bounty:
                return(u8"Bounty");
                break;
            case gameRoleId::Mechanic:
                return(u8"Mechanic");
                break;
            case gameRoleId::Technician:
                return(u8"Technician");
                break;
            case gameRoleId::Medium:
                return(u8"Medium");
                break;
            case gameRoleId::Vigilante:
                return(u8"Vigilante");
                break;
            case gameRoleId::Cannibal:
                return(u8"Cannibal");
                break;
            case gameRoleId::Morphling:
                return(u8"Morphling");
                break;
            case gameRoleId::Sheriff:
                return(u8"Sheriff");
                break;
            case gameRoleId::Silencer:
                return(u8"Silencer");
                break;
            case gameRoleId::Canadian:
                return(u8"Canadian");
                break;
            case gameRoleId::LoverDuck:
                return(u8"LoverDuck");
                break;
            case gameRoleId::LoverGoose:
                return(u8"LoverGoose");
                break;
            case gameRoleId::Vulture:
                return(u8"Vulture");
                break;
            case gameRoleId::Professional:
                return(u8"Professional");
                break;
            case gameRoleId::Spy:
                return(u8"Spy");
                break;
            case gameRoleId::Mimic:
                return(u8"Mimic");
                break;
            case gameRoleId::Detective:
                return(u8"Detective");
                break;
            case gameRoleId::Pigeon:
                return(u8"Pigeon");
                break;
            case gameRoleId::Birdwatcher:
                return(u8"Birdwatcher");
                break;
            case gameRoleId::Assassin:
                return(u8"Assassin");
                break;
            case gameRoleId::Falcon:
                return(u8"Falcon");
                break;
            case gameRoleId::Hitman:
                return(u8"Hitman");
                break;
            case gameRoleId::Bodyguard:
                return(u8"Bodyguard");
                break;
            case gameRoleId::Snitch:
                return(u8"Snitch");
                break;
            case gameRoleId::Politician:
                return(u8"Politician");
                break;
            case gameRoleId::Locksmith:
                return(u8"Locksmith");
                break;
            case gameRoleId::Mortician:
                return(u8"Mortician");
                break;
            case gameRoleId::Celebrity:
                return(u8"Celebrity");
                break;
            case gameRoleId::Party:
                return(u8"Party");
                break;
            case gameRoleId::Demolitionist:
                return(u8"Demolitionist");
                break;
            case gameRoleId::DuelingDodo:
                return(u8"DuelingDodo");
                break;
            case gameRoleId::GHGoose:
                return(u8"GHGoose");
                break;
            case gameRoleId::GHDuck:
                return(u8"GHDuck");
                break;
            case gameRoleId::GHBounty:
                return(u8"GHBounty");
                break;
            case gameRoleId::HNSGoose:
                return(u8"HNSGoose");
                break;
            case gameRoleId::HNSDuck:
                return(u8"HNSDuck");
                break;
            case gameRoleId::HNSBounty:
                return(u8"HNSBounty");
                break;
            case gameRoleId::DNDDuck:
                return(u8"DNDDuck");
                break;
            case gameRoleId::DNDFalcon:
                return(u8"DNDFalcon");
                break;
            case gameRoleId::DNDVulture:
                return(u8"DNDVulture");
                break;
            case gameRoleId::DNDMorphling:
                return(u8"DNDMorphling");
                break;
            case gameRoleId::FPGoose:
                return(u8"FPGoose");
                break;
            case gameRoleId::ExploreGoose:
                return(u8"ExploreGoose");
                break;
            case gameRoleId::TTVampire:
                return(u8"TTVampire");
                break;
            case gameRoleId::TTPeasant:
                return(u8"TTPeasant");
                break;
            case gameRoleId::TTThrall:
                return(u8"TTThrall");
                break;
            case gameRoleId::Spectator:
                return(u8"Spectator");
                break;
            case gameRoleId::IdentityThief:
                return(u8"IdentityThief");
                break;
            case gameRoleId::Adventurer:
                return(u8"Adventurer");
                break;
            case gameRoleId::Avenger:
                return(u8"Avenger");
                break;
            case gameRoleId::Ninja:
                return(u8"Ninja");
                break;
            case gameRoleId::Undertaker:
                return(u8"Undertaker");
                break;
            case gameRoleId::Snoop:
                return(u8"Snoop");
                break;
            case gameRoleId::Esper:
                return(u8"Esper");
                break;
            case gameRoleId::Invisibility:
                return(u8"Invisibility");
                break;
            case gameRoleId::Astral:
                return(u8"Astral");
                break;
            case gameRoleId::Pelican:
                return(u8"Pelican");
                break;
            case gameRoleId::TTEThrall:
                return(u8"TTEThrall");
                break;
            case gameRoleId::TTMummy:
                return(u8"TTMummy");
                break;
            case gameRoleId::SerialKiller:
                return(u8"SerialKiller");
                break;
            case gameRoleId::Engineer:
                return(u8"Engineer");
                break;
            case gameRoleId::Warlock:
                return(u8"Warlock");
                break;
            case gameRoleId::StreetUrchin:
                return(u8"StreetUrchin");
                break;
            case gameRoleId::Tracker:
                return(u8"Tracker");
                break;
            case gameRoleId::EsperDuck:
                return(u8"EsperDuck");
                break;
            case gameRoleId::Stalker:
                return(u8"Stalker");
                break;
            default:
                return(u8"None");
                break;
            }
        }
    }

private:
    /// <summary>
    /// 检查当前系统是否使用中文
    /// </summary>
    /// <returns>是否使用中文</returns>
    static bool isChineseLanguageOS() {
        try {
            std::string locale = __CheckLocale();
            if (locale.find("zh") != std::string::npos || locale.find("CN") != std::string::npos) {
                //中文 
                return true;
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