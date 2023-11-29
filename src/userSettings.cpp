

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include "userSettings.h"

UserSettings::UserSettings(){}

UserSettings::~UserSettings(){}

int UserSettings::GetScreenWidth()
{
     return UserSettings::_screen_width;
}

void UserSettings::SetScreenWidth(int screen_width)
{
    UserSettings::_screen_width = screen_width;
}

int UserSettings::GetScreenHeight()
{
    return UserSettings::_screen_height;
}

void UserSettings::SetScreenHeight(int screen_height)
{
    UserSettings::_screen_height = screen_height;
}

void UserSettings::ReadUserSettingsFile()
{
    std::ifstream settings_file_stream(UserSettings::_user_settings_file_path);
    if (settings_file_stream.is_open())
    {
        std::map<std::string, int> settings;
        std::string line, key;
        int value;

        while (std::getline(settings_file_stream, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value;
            settings[key] = value;
            std::cout << line << std::endl;
        };
        
        UserSettings::SetScreenWidth(settings[UserSettings::_screen_width_str]);
        UserSettings::SetScreenHeight(settings[UserSettings::_screen_height_str]);
    }
    else
        std::cout << "Error: No user settings file! Please reinstall." << std::endl;
}