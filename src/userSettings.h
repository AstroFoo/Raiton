#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <string>

class UserSettings
{
    public:
        UserSettings();
        ~UserSettings();

        int GetScreenWidth();
        void SetScreenWidth(int screen_width);
        int GetScreenHeight();
        void SetScreenHeight(int screen_height);

        void ReadUserSettingsFile();

    private:
        std::string _user_settings_file_path = "../UserSettings/UserSettings.ini";

        // Hardwired data - cannot be changed
        const std::string _screen_width_str = "screen_width";
        const std::string _screen_height_str = "screen_height";

        // Default user settings
        int _screen_width;
        int _screen_height;
};

#endif