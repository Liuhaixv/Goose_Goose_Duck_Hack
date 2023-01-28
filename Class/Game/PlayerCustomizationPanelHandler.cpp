#include "PlayerCustomizationPanelHandler.h"

#include "../memory.h"
#include"../utils.hpp"
#include "../Data/offsets.hpp"
#include "../../Client.h"

extern Client g_client;
extern Memory memory;
void PlayerCustomizationPanelHandler::resetMemberFields()
{
    i_SilverBalance = -1;
    i_GoldBalance = -1;
    i_TotalXp = -1;
    i_CurrentXp = -1;
    i_CurrentMaxXp = -1;
    i_Level = -1;
    
}

bool PlayerCustomizationPanelHandler::update()
{
    //无效指针
        //invalid pointer address
    if (this->address == NULL) {
        return false;
    }

    static std::vector<int64_t> offsets = GameAssembly::playerCustomizationPanelHandler();

    int64_t instance_Addr = memory.FindPointer(memory.gameAssemblyBaseAddress, offsets);

    if (instance_Addr == NULL) {
        return false;
    }

    i_SilverBalance = memory.read_mem<int>(instance_Addr + Offsets::PlayerCustomizationPanelHandler::Class::StaticField::i_SilverBalance, false);
    i_GoldBalance = memory.read_mem<int>(instance_Addr + Offsets::PlayerCustomizationPanelHandler::Class::StaticField::i_GoldBalance, false);
    i_TotalXp = memory.read_mem<int>(instance_Addr + Offsets::PlayerCustomizationPanelHandler::Class::StaticField::i_Xp, false);

    static std::vector<int64_t> xpoffsets = GameAssembly::LGHFFHFMADD_xpArray();

    int64_t xpArray_Addr = memory.FindPointer(memory.gameAssemblyBaseAddress, xpoffsets);

    i_Level = 0;
    int index = 0;
    int cmpValue = 0;
    int LastLevel_Max_Xp = 0;

    while (index < 999)
    {
        cmpValue = memory.read_mem<int>(xpArray_Addr + index * 0x4, false);
        if (i_TotalXp < cmpValue)
            break;

        ++i_Level;
        index = i_Level;
    }

    if (i_TotalXp > 0)
    {
        LastLevel_Max_Xp = memory.read_mem<int>(xpArray_Addr + (index - 1) * 0x4, false);
        i_CurrentXp = i_TotalXp - LastLevel_Max_Xp;
        i_CurrentMaxXp = memory.read_mem<int>(xpArray_Addr + index * 0x4, false);
    }
    else
    {
        i_CurrentMaxXp = memory.read_mem<int>(xpArray_Addr, false);
        i_CurrentXp = 0;
    }
    

    return true;
}

bool PlayerCustomizationPanelHandler::validateAddress(IN int64_t address) {

    int64_t playerCustomizationPanelHandlerClass = memory.read_mem<int64_t>(memory.gameAssemblyBaseAddress + GameAssembly::Class::ptr_PlayerCustomizationPanelHandlerClass, NULL);

    if (playerCustomizationPanelHandlerClass == NULL) {
        //Error finding class
        return false;
    }

    int64_t playerCustomizationPanelHandlerClass_ = memory.read_mem<int64_t>(address, NULL);

    return playerCustomizationPanelHandlerClass == playerCustomizationPanelHandlerClass_;
}