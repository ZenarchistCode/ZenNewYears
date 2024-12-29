modded class MissionServer
{
	private bool m_ZenDebugNewYearsMsg = false;
	private bool m_LaunchedNewYearsFireworks = false;

	override void OnInit()
	{
		super.OnInit();

		// Load json config
		GetZenNewYearsConfig();
	}

	// Detect if current date/time = config date and time down to the minute
	bool IsEventTime()
	{
		int utcYear = 0;
		int utcMonth = 0;
		int utcDay = 0;
		GetYearMonthDay(utcYear, utcMonth, utcDay);

		int utcHour = 0;
		int utcMinute = 0;
		int utcSecond = 0;
		GetHourMinuteSecond(utcHour, utcMinute, utcSecond);

		if (!m_ZenDebugNewYearsMsg)
		{
			m_ZenDebugNewYearsMsg = true;
			string msg = "[ZenNewYears] Startup - ";
			msg = msg + " Year=" + utcYear + "/" + GetZenNewYearsConfig().TriggerYear;
			msg = msg + " Month=" + utcMonth + "/" + GetZenNewYearsConfig().TriggerMonth;
			msg = msg + " Day=" + utcDay + "/" + GetZenNewYearsConfig().TriggerDay;
			msg = msg + " HR=" + utcHour + "/" + GetZenNewYearsConfig().TriggerHour;
			msg = msg + " MN=" + utcMinute + "/" + GetZenNewYearsConfig().TriggerMinute;
			Print(msg);
		}

		if (utcYear != GetZenNewYearsConfig().TriggerYear)
			return false;

		if (utcMonth != GetZenNewYearsConfig().TriggerMonth)
			return false;

		if (utcDay != GetZenNewYearsConfig().TriggerDay)
			return false;

		if (utcHour != GetZenNewYearsConfig().TriggerHour)
			return false;

		if (utcMinute != GetZenNewYearsConfig().TriggerMinute)
			return false;

		if (utcSecond < GetZenNewYearsConfig().TriggerSecond)
			return false;

		return true;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (m_LaunchedNewYearsFireworks || !IsEventTime())
			return;

		int i;
		vector playerPos;
		PlayerBase player;
		array<Man> players();
		GetGame().GetPlayers(players);
		array<vector> m_FireworksPositions = new array<vector>;

		// Launch fireworks on players
		foreach (Man man : players)
		{
			player = PlayerBase.Cast(man);
			if (!player)
				continue;

			// Send message
			if (GetZenNewYearsConfig().Message != "")
				GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(GetZenNewYearsConfig().Message), true, player.GetIdentity());

			// Skip if player fireworks spawn is disabled
			if (GetZenNewYearsConfig().PlayerFireworksCount <= 0)
				continue;

			// To prevent a shit ton of fireworks spawning on groups of players, only launch 1 cluster per nearby player
			playerPos = player.GetPosition();
			bool skipFireworks = false;
			foreach (vector fireworkPos : m_FireworksPositions)
			{
				if (vector.Distance(playerPos, fireworkPos) < GetZenNewYearsConfig().MaxFireworksDistance)
					skipFireworks = true;
			}

			if (skipFireworks)
				continue;

			// Spawn X amount of fireworks
			for (i = 0; i < GetZenNewYearsConfig().PlayerFireworksCount; i++)
			{
				SpawnFireworks(playerPos);
				m_FireworksPositions.Insert(playerPos);
			}
		}

		// Launch fireworks at static positions
		foreach (vector staticPos : GetZenNewYearsConfig().StaticFireworkSpawns)
		{
			for (i = 0; i < GetZenNewYearsConfig().StaticFireworksCount; i++)
			{
				SpawnFireworks(staticPos, true);
			}
		}

		m_LaunchedNewYearsFireworks = true;
	}

	private void SpawnFireworks(vector pos, bool staticFireworks = false)
	{
		vector spawnPos;

		if (staticFireworks)
		{
			spawnPos = ZenGetRandomPointInCircle(pos, GetZenNewYearsConfig().MaxStaticFireworksDistance, GetZenNewYearsConfig().MinStaticFireworksDistance, false);
		}
		else
		{
			spawnPos = ZenGetRandomPointInCircle(pos, GetZenNewYearsConfig().MaxFireworksDistance, GetZenNewYearsConfig().MinFireworksDistance, true);
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedSpawnFireworks, Math.RandomFloatInclusive(GetZenNewYearsConfig().MinFireworksTime, GetZenNewYearsConfig().MaxFireworksTime), false, spawnPos);
	}

	private void DelayedSpawnFireworks(vector pos)
	{
		FireworksLauncher fireworks = FireworksLauncher.Cast(GetGame().CreateObjectEx(GetZenNewYearsConfig().SpawnObject, pos, ECE_PLACE_ON_SURFACE));
		if (fireworks)
		{
			Print("[ZenNewYears] Spawned fireworks @ " + pos);
			fireworks.OnIgnitedThis(null);
			fireworks.SetLifetime(69);
		}
		else
		{
			Error("[ZenNewYears] Failed to spawn object " + GetZenNewYearsConfig().SpawnObject);
		}
	}

	//! Generate a random point inside a circle. minDistance = minimum distance in meters from center (again thanks ChatGPT)
	static vector ZenGetRandomPointInCircle(vector position, float radius, float minDistance = 0, bool placeOnSurface = true)
	{
		// Get a random angle between 0 and 2*PI
		float angle = Math.RandomFloatInclusive(0.0, Math.PI2);

		// Convert polar coordinates to Cartesian coordinates
		float x = (minDistance + Math.RandomFloatInclusive(0.0, radius - minDistance)) * Math.Cos(angle);
		float z = (minDistance + Math.RandomFloatInclusive(0.0, radius - minDistance)) * Math.Sin(angle);

		// Adjust new position and return it
		vector newPos = position;
		newPos[0] = newPos[0] + x;
		newPos[2] = newPos[2] + z;

		if (placeOnSurface)
			newPos[1] = GetGame().SurfaceY(newPos[0], newPos[2]);

		return newPos;
	}
}