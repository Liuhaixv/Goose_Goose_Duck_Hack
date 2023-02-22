#include"utils.hpp"

#include "Struct/UserSettings.hpp"

extern UserSettings userSettings;

Utils::Utils() {
    this->b_useChineseLanguage = this->isChineseLanguageOS();
}

void Utils::changeLanguage() {
    this->b_useChineseLanguage = !this->b_useChineseLanguage;
    //userSettings.setInt(UserSettingsNames::b_use_chinese_language, this->b_useChineseLanguage);
}