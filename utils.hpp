#pragma once

#include<string>
#include<list>
#include<iomanip>
#include<winnls.h>
#include<iostream>

#include"Data/GameData.hpp"

class Utils {
public:
    //当前操作系统是否为中文
    //true if current OS's language is Chinese
    bool b_chineseOS = false;

	Utils() {
		if (b_chineseOS < 0) {
			b_chineseOS = isChineseLanguageOS();
		}
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

	const char* getRoleName(int id) {
		//返回中文角色名
		if (this->b_chineseOS) {
			switch (id)
			{
			case gameRoleId::None:
				return("无");
				break;
			case gameRoleId::Goose:
				return("鹅");
				break;
			case gameRoleId::Duck:
				return("鸭子");
				break;
			case gameRoleId::Dodo:
				return("呆呆鸟");
				break;
			case gameRoleId::Bounty:
				return("赏金");
				break;
			case gameRoleId::Mechanic:
				//可以使用管道(该角色已被开发者移除)
				return("Mechanic");
				break;
			case gameRoleId::Technician:
				//可以在迷你地图上看到破坏的大约位置(该角色已被开发者移除)
				return("Technician");
				break;
			case gameRoleId::Medium:
				return("通灵");
				break;
			case gameRoleId::Vigilante:
				return("正义使者");
				break;
			case gameRoleId::Cannibal:
				return("食鸟鸭");
				break;
			case gameRoleId::Morphling:
				return("变形者(鸭子)");
				break;
			case gameRoleId::Sheriff:
				return("警长");
				break;
			case gameRoleId::Silencer:
				return("静语者(鸭子)");
				break;
			case gameRoleId::Canadian:
				return("加拿大鹅");
				break;
			case gameRoleId::LoverDuck:
				return("恋人(鸭子)");
				break;
			case gameRoleId::LoverGoose:
				return("恋人(鹅)");
				break;
			case gameRoleId::Vulture:
				return("秃鹫");
				break;
			case gameRoleId::Professional:
				return("专业杀手(鸭子)");
				break;
			case gameRoleId::Spy:
				return("间谍(鸭子)");
				break;
			case gameRoleId::Mimic:
				return("模仿者");
				break;
			case gameRoleId::Detective:
				return("侦探");
				break;
			case gameRoleId::Pigeon:
				return("鸽子");
				break;
			case gameRoleId::Birdwatcher:
				return("观鸟者");
				break;
			case gameRoleId::Assassin:
				return("刺客(鸭子)");
				break;
			case gameRoleId::Falcon:
				return("猎鹰");
				break;
			case gameRoleId::Hitman:
				return("雇佣杀手(鸭子)");
				break;
			case gameRoleId::Bodyguard:
				return("保镖鹅");
				break;
			case gameRoleId::Snitch:
				return("告密者(鸭子)");
				break;
			case gameRoleId::Politician:
				return("政治家");
				break;
			case gameRoleId::Locksmith:
				return("锁匠");
				break;
			case gameRoleId::Mortician:
				return("殡仪员");
				break;
			case gameRoleId::Celebrity:
				return("网红");
				break;
			case gameRoleId::Party:
				return("派对狂(鸭子)");
				break;
			case gameRoleId::Demolitionist:
				return("爆炸王(鸭子)");
				break;
			case gameRoleId::DuelingDodo:
				return("决斗呆呆鸟");
				break;
			case gameRoleId::GHGoose:
				return("【猎鹅】鹅");
				break;
			case gameRoleId::GHDuck:
				return("【猎鹅】鸭子");
				break;
			case gameRoleId::GHBounty:
				//你的任务是被第一个消灭，如果任务完成则可以获得奖励(该角色已被开发者移除)
				return("【猎鹅】赏金鹅");
				break;
			case gameRoleId::HNSGoose:
				return("【HNS】鹅");
				break;
			case gameRoleId::HNSDuck:
				return("【HNS】鸭子");
				break;
			case gameRoleId::HNSBounty:
				return("【HNS】赏金鹅");
				break;
			case gameRoleId::DNDDuck:
				return("【霸王餐】鸭子");
				break;
			case gameRoleId::DNDFalcon:
				return("【霸王餐】猎鹰");
				break;
			case gameRoleId::DNDVulture:
				return("【霸王餐】秃鹫");
				break;
			case gameRoleId::DNDMorphling:
				return("【霸王餐】变形鸭");
				break;
			case gameRoleId::FPGoose:
				return("FPGoose");
				break;
			case gameRoleId::ExploreGoose:
				return("ExploreGoose");
				break;
			case gameRoleId::TTVampire:
				return("【TT】吸血鬼");
				break;
			case gameRoleId::TTPeasant:
				return("【TT】村民");
				break;
			case gameRoleId::TTThrall:
				return("【TT】鬼奴");
				break;
			case gameRoleId::Spectator:
				return("旁观");
				break;
			case gameRoleId::IdentityThief:
				return("身份窃贼");
				break;
			case gameRoleId::Adventurer:
				return("冒险家");
				break;
			case gameRoleId::Avenger:
				return("复仇者");
				break;
			case gameRoleId::Ninja:
				return("忍者(鸭子)");
				break;
			case gameRoleId::Undertaker:
				return("丧葬者");
				break;
			case gameRoleId::Snoop:
				//可以隐藏在地图的一些地方(该角色已被开发者移除)
				return("Snoop");
				break;
			case gameRoleId::Esper:
				return("超能力者");
				break;
			case gameRoleId::Invisibility:
				return("隐身者");
				break;
			case gameRoleId::Astral:
				return("星界行者");
				break;
			case gameRoleId::Pelican:
				return("鹈鹕");
				break;
			case gameRoleId::TTEThrall:
				return("【TTE】鬼奴");
				break;
			case gameRoleId::TTMummy:
				return("【TT】木乃伊");
				break;
			case gameRoleId::SerialKiller:
				return("连环杀手(鸭子)");
				break;
			case gameRoleId::Engineer:
				return("工程师");
				break;
			case gameRoleId::Warlock:
				return("术士(鸭子)");
				break;
			case gameRoleId::StreetUrchin:
				return("流浪儿童");
				break;
			case gameRoleId::Tracker:
				return("追踪者");
				break;
			default:
				return("无");
				break;
			}
		}
		else {
			switch (id)
			{
			case gameRoleId::None:
				return("None");
				break;
			case gameRoleId::Goose:
				return("Goose");
				break;
			case gameRoleId::Duck:
				return("Duck");
				break;
			case gameRoleId::Dodo:
				return("Dodo");
				break;
			case gameRoleId::Bounty:
				return("Bounty");
				break;
			case gameRoleId::Mechanic:
				return("Mechanic");
				break;
			case gameRoleId::Technician:
				return("Technician");
				break;
			case gameRoleId::Medium:
				return("Medium");
				break;
			case gameRoleId::Vigilante:
				return("Vigilante");
				break;
			case gameRoleId::Cannibal:
				return("Cannibal");
				break;
			case gameRoleId::Morphling:
				return("Morphling");
				break;
			case gameRoleId::Sheriff:
				return("Sheriff");
				break;
			case gameRoleId::Silencer:
				return("Silencer");
				break;
			case gameRoleId::Canadian:
				return("Canadian");
				break;
			case gameRoleId::LoverDuck:
				return("LoverDuck");
				break;
			case gameRoleId::LoverGoose:
				return("LoverGoose");
				break;
			case gameRoleId::Vulture:
				return("Vulture");
				break;
			case gameRoleId::Professional:
				return("Professional");
				break;
			case gameRoleId::Spy:
				return("Spy");
				break;
			case gameRoleId::Mimic:
				return("Mimic");
				break;
			case gameRoleId::Detective:
				return("Detective");
				break;
			case gameRoleId::Pigeon:
				return("Pigeon");
				break;
			case gameRoleId::Birdwatcher:
				return("Birdwatcher");
				break;
			case gameRoleId::Assassin:
				return("Assassin");
				break;
			case gameRoleId::Falcon:
				return("Falcon");
				break;
			case gameRoleId::Hitman:
				return("Hitman");
				break;
			case gameRoleId::Bodyguard:
				return("Bodyguard");
				break;
			case gameRoleId::Snitch:
				return("Snitch");
				break;
			case gameRoleId::Politician:
				return("Politician");
				break;
			case gameRoleId::Locksmith:
				return("Locksmith");
				break;
			case gameRoleId::Mortician:
				return("Mortician");
				break;
			case gameRoleId::Celebrity:
				return("Celebrity");
				break;
			case gameRoleId::Party:
				return("Party");
				break;
			case gameRoleId::Demolitionist:
				return("Demolitionist");
				break;
			case gameRoleId::DuelingDodo:
				return("DuelingDodo");
				break;
			case gameRoleId::GHGoose:
				return("GHGoose");
				break;
			case gameRoleId::GHDuck:
				return("GHDuck");
				break;
			case gameRoleId::GHBounty:
				return("GHBounty");
				break;
			case gameRoleId::HNSGoose:
				return("HNSGoose");
				break;
			case gameRoleId::HNSDuck:
				return("HNSDuck");
				break;
			case gameRoleId::HNSBounty:
				return("HNSBounty");
				break;
			case gameRoleId::DNDDuck:
				return("DNDDuck");
				break;
			case gameRoleId::DNDFalcon:
				return("DNDFalcon");
				break;
			case gameRoleId::DNDVulture:
				return("DNDVulture");
				break;
			case gameRoleId::DNDMorphling:
				return("DNDMorphling");
				break;
			case gameRoleId::FPGoose:
				return("FPGoose");
				break;
			case gameRoleId::ExploreGoose:
				return("ExploreGoose");
				break;
			case gameRoleId::TTVampire:
				return("TTVampire");
				break;
			case gameRoleId::TTPeasant:
				return("TTPeasant");
				break;
			case gameRoleId::TTThrall:
				return("TTThrall");
				break;
			case gameRoleId::Spectator:
				return("Spectator");
				break;
			case gameRoleId::IdentityThief:
				return("IdentityThief");
				break;
			case gameRoleId::Adventurer:
				return("Adventurer");
				break;
			case gameRoleId::Avenger:
				return("Avenger");
				break;
			case gameRoleId::Ninja:
				return("Ninja");
				break;
			case gameRoleId::Undertaker:
				return("Undertaker");
				break;
			case gameRoleId::Snoop:
				return("Snoop");
				break;
			case gameRoleId::Esper:
				return("Esper");
				break;
			case gameRoleId::Invisibility:
				return("Invisibility");
				break;
			case gameRoleId::Astral:
				return("Astral");
				break;
			case gameRoleId::Pelican:
				return("Pelican");
				break;
			case gameRoleId::TTEThrall:
				return("TTEThrall");
				break;
			case gameRoleId::TTMummy:
				return("TTMummy");
				break;
			case gameRoleId::SerialKiller:
				return("SerialKiller");
				break;
			case gameRoleId::Engineer:
				return("Engineer");
				break;
			case gameRoleId::Warlock:
				return("Warlock");
				break;
			case gameRoleId::StreetUrchin:
				return("StreetUrchin");
				break;
			case gameRoleId::Tracker:
				return("Tracker");
				break;
			default:
				return("None");
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