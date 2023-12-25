modded class MissionServer
{
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
		GetYearMonthDayUTC(utcYear, utcMonth, utcDay);

		int utcHour = 0;
		int utcMinute = 0;
		int utcSecond = 0;
		GetHourMinuteSecondUTC(utcHour, utcMinute, utcSecond);

		utcHour += GetZenNewYearsConfig().UTC_Offset;

		if (utcMinute != GetZenNewYearsConfig().TriggerMinute)
			return false;

		if (utcHour != GetZenNewYearsConfig().TriggerHour)
			return false;

		if (utcDay != GetZenNewYearsConfig().TriggerDay)
			return false;

		if (utcMonth != GetZenNewYearsConfig().TriggerMonth)
			return false;

		if (utcYear != GetZenNewYearsConfig().TriggerYear)
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
		autoptr array<vector> m_FireworksPositions = new array<vector>; // autoptr = delete automatically this array when this scope ends

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