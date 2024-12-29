class ZenNewYearsConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenNewYearsConfig.json";

	string Message;
	int TriggerDay;
	int TriggerMonth;
	int TriggerYear;
	int TriggerHour;
	int TriggerMinute;
	int TriggerSecond;
	int PlayerFireworksCount;
	int MinFireworksDistance;
	int MaxFireworksDistance;
	int MinFireworksTime;
	int MaxFireworksTime;
	int StaticFireworksCount;
	int MinStaticFireworksDistance;
	int MaxStaticFireworksDistance;
	ref array<vector> StaticFireworkSpawns; // Traders & safezones etc
	string SpawnObject;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenNewYearsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		// Get current date
		int utcYear = 0;
		int utcMonth = 0;
		int utcDay = 0;
		GetYearMonthDayUTC(utcYear, utcMonth, utcDay);

		// Set default config
		TriggerDay = 1;
		TriggerMonth = 1;
		TriggerYear = utcYear;
		TriggerHour = 0;
		TriggerMinute = 0;
		TriggerSecond = 0;
		Message = "Happy New Year! Good luck in " + TriggerYear + " survivor...";
		PlayerFireworksCount = 3;
		MinFireworksDistance = 50;
		MaxFireworksDistance = 150;
		MinFireworksTime = 0;
		MaxFireworksTime = 5000;
		StaticFireworksCount = 1;
		MinStaticFireworksDistance = 0;
		MaxStaticFireworksDistance = 0;
		StaticFireworkSpawns = new ref array<vector>;
		StaticFireworkSpawns.Insert("5631.078125 184.755157 10607.497070");
		SpawnObject = "FireworksLauncher";

		Save();
	}

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenNewYearsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenNewYearsConfig m_ZenNewYearsConfig;

static ZenNewYearsConfig GetZenNewYearsConfig()
{
	if (!m_ZenNewYearsConfig)
	{
		Print("[ZenNewYearsConfig] Init");
		m_ZenNewYearsConfig = new ZenNewYearsConfig;
		m_ZenNewYearsConfig.Load();
	}

	return m_ZenNewYearsConfig;
};