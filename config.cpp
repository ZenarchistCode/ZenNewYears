class CfgPatches
{
	class ZenNewYears
	{
		requiredVersion = 0.1;
		units[] = {};
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class ZenNewYears
	{
		dir = "ZenNewYears";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenNewYears";
		credits = "Artyfox";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenNewYears/scripts/3_game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenNewYears/scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenNewYears/scripts/5_mission" };
			};
		};
	};
};
