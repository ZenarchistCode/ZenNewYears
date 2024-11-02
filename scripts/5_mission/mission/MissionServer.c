modded class MissionServer
{
	private bool m_LaunchedNewYearsFireworks = false;

	override void OnInit()
	{
		super.OnInit();

		// Load json config
		GetZenNewYearsConfig();
	}

	// Check if the current date/time matches any date and time in TriggerDates
	bool IsEventTime()
	{
		int utcYear = 0;
		int utcMonth = 0;
		int utcDay = 0;
		GetYearMonthDayUTC(utcYear, utcMonth, utcDay);

		int utcHour = 0;
		int utcMinute = 0;
		int utcSecond = 0;
		GetHourMinuteSecondUTC(utcHour, utcMinute, utcSecond);

		utcHour += GetZenNewYearsConfig().UTC_Offset;

		// Loop through each date in the TriggerDates array to see if any match the current time
		foreach (DateConfig dateConfig : GetZenNewYearsConfig().TriggerDates)
		{
			if (utcMinute == dateConfig.TriggerMinute &&
			    utcHour == dateConfig.TriggerHour &&
			    utcDay == dateConfig.TriggerDay &&
			    utcMonth == dateConfig.TriggerMonth &&
			    utcYear == dateConfig.TriggerYear)
			{
				return true; // Event time matches one of the specified dates
			}
		}
		
		return false; // No matching event time found
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
		autoptr array<vector> m_FireworksPositions = new array<vector>; // autoptr deletes automatically when scope ends

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

			// Avoid multiple fireworks in close proximity
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
		float xOffset;
		float yOffset;

		if (staticFireworks)
		{
			xOffset = Math.RandomFloatInclusive(GetZenNewYearsConfig().MinStaticFireworksDistance, GetZenNewYearsConfig().MaxStaticFireworksDistance);
			yOffset = Math.RandomFloatInclusive(GetZenNewYearsConfig().MinStaticFireworksDistance, GetZenNewYearsConfig().MaxStaticFireworksDistance);
		}
		else
		{
			xOffset = Math.RandomFloatInclusive(GetZenNewYearsConfig().MinFireworksDistance, GetZenNewYearsConfig().MaxFireworksDistance);
			yOffset = Math.RandomFloatInclusive(GetZenNewYearsConfig().MinFireworksDistance, GetZenNewYearsConfig().MaxFireworksDistance);
		}

		if (Math.RandomFloat01() < 0.5)
			xOffset = xOffset * -1;
		if (Math.RandomFloat01() < 0.5)
			yOffset = yOffset * -1;

		pos[0] = pos[0] + xOffset;
		pos[2] = pos[2] + yOffset;
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedSpawnFireworks, Math.RandomFloatInclusive(GetZenNewYearsConfig().MinFireworksTime, GetZenNewYearsConfig().MaxFireworksTime), false, pos);		
	}

	private void DelayedSpawnFireworks(vector pos)
	{
		FireworksLauncher fireworks = FireworksLauncher.Cast(GetGame().CreateObjectEx(GetZenNewYearsConfig().SpawnObject, pos, ECE_PLACE_ON_SURFACE));
		if (fireworks)
		{
			fireworks.OnIgnitedThis(null);
			fireworks.SetLifetime(69);
		}
	}
};
