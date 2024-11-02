class DateConfig
{
	int TriggerDay;
	int TriggerMonth;
	int TriggerYear;
	int TriggerHour;
	int TriggerMinute;

	void DateConfig(int day, int month, int year, int hour, int minute)
	{
		TriggerDay = day;
		TriggerMonth = month;
		TriggerYear = year;
		TriggerHour = hour;
		TriggerMinute = minute;
	}
}

class ZenNewYearsConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenNewYearsConfig.json";

	string Message;
	int UTC_Offset; // +10 for Brisbane, Australia etc
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

	ref array<ref DateConfig> TriggerDates; // Array to hold multiple dates

	void ZenNewYearsConfig()
	{
		TriggerDates = new ref array<ref DateConfig>;
	}

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenNewYearsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		// Initialize default TriggerDates if not loaded from config
		InitializeDefaultDates();

		// Set default config for other fields
		UTC_Offset = 10;
		Message = "Happy New Year! Celebrate with the fireworks!";
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

	void InitializeDefaultDates()
	{
		// Get current date
		int utcYear = 0;
		int utcMonth = 0;
		int utcDay = 0;
		GetYearMonthDayUTC(utcYear, utcMonth, utcDay);

		// Set default TriggerDates with a New Year's date and another date as an example
		TriggerDates.Insert(new DateConfig(1, 1, utcYear + 1, 0, 0));  // New Year
		TriggerDates.Insert(new DateConfig(25, 12, utcYear + 1, 0, 0)); // Christmas
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
