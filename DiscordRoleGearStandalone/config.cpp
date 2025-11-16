#define UNIVERSALAPI

class CfgPatches
{
    class UAPIBase
    {
        requiredVersion = 0.1;
        requiredAddons[] = {};
    };

    class UniversalApi
    {
        requiredVersion = 0.1;
        requiredAddons[] = {"UAPIBase", "JM_CF_Scripts"};
    };

    class DiscordRoleGear
    {
        requiredVersion = 0.1;
        requiredAddons[] = {"UniversalApi"};
    };
};

class CfgMods
{
    class UAPIBase
    {
        dir = "_UAPIBase";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Universal Api";
        credits = "DaemonForge";
        author = "DaemonForge";
        authorID = "0";
        version = "1.3.2";
        extra = 0;
        type = "mod";
        dependencies[] = {"Core", "Game", "World", "Mission"};

        class defs
        {
            class engineScriptModule
            {
                value = "";
                files[] = {
                    "_UAPIBase/scripts/Common"
                };
            };

            class gameScriptModule
            {
                value = "";
                files[] = {
                    "_UAPIBase/scripts/Common",
                    "_UAPIBase/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] = {
                    "_UAPIBase/scripts/Common",
                    "_UAPIBase/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] = {
                    "_UAPIBase/scripts/Common"
                };
            };
        };
    };

    class UniversalApi
    {
        dir = "_UniversalApi";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Universal Api";
        credits = "";
        author = "DaemonForge";
        authorID = "0";
        version = "1.3.2";
        extra = 0;
        type = "mod";
        dependencies[] = {"Core", "Game", "World", "Mission"};

        class defs
        {
            class engineScriptModule
            {
                value = "";
                files[] = {
                    "_UniversalApi/scripts/1_Core"
                };
            };

            class gameScriptModule
            {
                value = "";
                files[] = {
                    "_UniversalApi/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] = {
                    "_UniversalApi/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] = {
                    "_UniversalApi/scripts/5_Mission"
                };
            };
        };
    };

    class DiscordRoleGear
    {
        dir = "DiscordRoleGear";
        name = "DiscordRoleGear";
        credits = "DaemonForge";
        author = "DaemonForge";
        authorID = "0";
        version = "0.1";
        extra = 0;
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {
                    "DiscordRoleGear/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] = {
                    "DiscordRoleGear/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] = {
                    "DiscordRoleGear/scripts/5_Mission"
                };
            };
        };
    };
};
