#pragma once
#include "Data/GameData.hpp"
class PossibleRole
{
public:
    int roleId = gameRoleId::None;

    PossibleRole() {
        this->roleId = gameRoleId::None;
    }
};

