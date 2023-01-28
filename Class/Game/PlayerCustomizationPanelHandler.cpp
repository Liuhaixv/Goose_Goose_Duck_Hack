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
    i_Xp = -1;
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
    i_Xp = memory.read_mem<int>(instance_Addr + Offsets::PlayerCustomizationPanelHandler::Class::StaticField::i_Xp, false);

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