What Is This?

This mod launches fireworks on New Years Eve at midnight. It's purely intended for server owners and is a server-side mod.

Technically it can spawn fireworks at any date/time, but the default config it generates will automatically set it for New Years.

You need to specify your timezone in the JSON config, and then when the clock strikes midnight at the specified date and time for your server then fireworks will spawn and launch near every player and/or every static location you specify in the config.

Installation Instructions:

Install this mod like any other mod - copy it into your server folder and add it to your mods list. Make sure to copy the .bikey into your server keys if you're not using a server management tool like OmegaManager which does that automatically.

This mod can be installed as a server-side only mod.

Config:

When you run the server for the first time after this mod is installed it will automatically generate a JSON config file in your server profiles folder (server_root/profiles/Zenarchist/ZenNewYearsConfig.json)

Inside this JSON file you can configure the settings for the fireworks. 

I included a lot of settings for more experienced server owners, but the only one you really need to adjust is the UTC_Offset.

This is your GMT/UTC offset for your server. My server is based in Brisbane Australia so I need to set it to 10 to get my UTC+10 timezone. If you don't know your server's timezone, google it.

You can also add any static positions you want to spawn fireworks (eg. trader zones, cities etc). 

Here's a description of each setting:

Message: The text chat message to send to all players on the server.
TriggerDay: The day of the month to trigger (eg. 1 = 1st)
TriggerMonth: The month to trigger (eg. 1 = January)
TriggerYear: The year to trigger. The mod will automatically detect next year for this setting (eg. 2024 currently).
TriggerHour: The hour to trigger in 24hrs (eg. 0 = midnight)
TriggerMinute: The minute to trigger (eg. 0 = the first minute)
UTC_Offset: The UTC offset to use (eg. 10 = GMT+10 Brisbane Australia)
PlayerFireworksCount: How many fireworks to spawn on each player (will not spawn more than 1 cluster per nearby player)
MinFireworksDistance: Minimum distance in meters to spawn fireworks
MaxFireworksDistance: Maximum distance in meters to spawn fireworks (randomly spawned between min/max)
MinFireworksTime: Minimum delay for triggering fireworks effect
MaxFireworksTime: Maximum delay for triggering fireworks effect (in milliseconds)
StaticFireworksCount: How many fireworks to spawn at each static location
MinStaticFireworksDistance: Same as MinFireworksDistance but for static positions
MaxStaticFireworksDistance: Same as MaxFireworksDistance but for static positions
StaticFireworkSpawns: List of static spawn locations (eg. traders, cities etc - go wild)
SpawnObject: What type of fireworks to spawn. If you run the mod this year for 2024 it will spawn Anniversary fireworks, for any other year it'll be normal fireworks unless you override this setting.

Repack & Source Code:

You can repack this mod if you like, and do anything else you want with it for that matter. The source code is on my GitHub at www.zenarchist.io

Enjoy!