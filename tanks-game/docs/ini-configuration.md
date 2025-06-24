# INI Configuration Documentation

## Getting started

Since I don't have the expertise or willingness to add advanced features like "a menu," I made an INI file to be able to configure the game outside of recompiling. Here is the list of everything you can change, arranged by section.

## UNIVERSAL

* `GraphicsContext` (default "OpenGL"): options are "OpenGL", "software", and "NULL". Only "OpenGL" is supported...
* `RNGSeed`: Set the seed for RNG, or blank for random.
* `ThreadCount` (default 4): The number of total threads to use for multithreading (counting the main thread); <=0 is all cores on your system minus the number (so -2 is all but two cores). Performance is only somewhat better when using extra threads, as the extra threads are only used for the broad phase of collision, though ~20% should be possible. Too many threads will ruin the cache for the narrow phase, thus lower performance.
* `ThreadTaskSize` (default 256): The minimum number of items to be worked on by each thread. If this is too low, then the overhead in spinning up a new thread will greatly outweigh the work it could do; too high and not all threads will do anything.

## MODS

* `LoadMods` (default 1): Controls whether to load mods or not.
* `ModSafetyChecks` (default 1): Controls whether to force checks on mods' types. For example, "vanilla" is reserved thanks to this setting. If this is turned on, playing on regular and custom levels at the same time is challenging.

## DEBUG

* `PerformanceGraphEnable` (default 0): Controls whether to draw the performance graph.
* `PerformanceGraphOffsetMultiplier` (default 0.0): Controls the performance graph's y-offset multiplier. 0 keeps it at the bottom of the screen, 1 puts it above the screen.
* `EnableDebugDrawing` (default 0): Controls whether to draw "debug drawings" on some objects.
* `EnableDebugDrawingObjects` (default "patrolling_turret" "mother_turret" "ginormous_turret"): Sets which objects to enable debug drawing for.
* `DeveloperInsertMenuPowerAdditions` (default "special bounce" "dev" "ultrabounce" "special triple" "dev" "triple_spread" "testpower2" "testmod1" "test-power-2"): Adds certain powerups to the developer "menu". In groups of 3, supply the item's identifier, the power's type, and the power's name.

## GRAPHICS_SETTINGS

* `Position.StartX` (default 120): Sets the window's starting x-position.
* `Position.StartY` (default 120): Sets the window's starting y-position. Remember that this is counted from the top left of your primary monitor.
* `Position.SizeMultiplier` (default 2.5): Controls the sizing of the window. The actual game's size is 640×320, so by default the window is scaled to 1600×800.

Note: Press F11 to toggle fullscreen. (Well, it's borderless fullscreen, but it still fills the whole screen.)

## GAME_OPTIONS

* `GameFirstLevel` (default "vanilla" "default_random"): Controls the first level of the game. Very useful.
* `GameLevelPlaylist` (default "random-vanilla"): Controls which level playlist to choose from once the game has started.
* `GameForceSameLevel`: Forces the game to only play on the same level, which is useful for testing new levels. Set to the name of the level to enable (for example, "vanilla" "default_random").
* `CustomLevelPlaylist`: Creates a level playlist with levels of your choosing. In groups of 3, supply the level's type, name, and weight. Overrides `GameLevelPlaylist` if set.
* `ReportCurrentLevel` (default 1): Prints the level being played when a new game is started.
* `LimitBullets` (default 1): Controls whether to limit the number of bullets on screen. Look, your computer has a limit at some point.
* `MaxBullets` (default 8192): Controls the maximum number of bullets, assuming `LimitBullets` is true.
* `FewerExtraShootingBullets` (default 0): Controls whether something like Triple + Fire shoots 7 or 12 bullets. Keeping this setting off is a significant change (and performance hit), so that's why this is an option instead of always doing more bullets. Remember, Shotgun + Blast goes from 4+16 to 5×16 bullets; that's a lot!
* `ShootingCooldown` (default 100): Controls the tanks' shooting cooldown. Set to 0 for superfast shooting (very fun!).
* `PowerupDurationBaseTime` (default 500): Controls the base time of powerups.
* `DisableTraps` (default 1): Prevents traps from activating. Does nothing since traps don't exist but might in the future. (They existed in JS Tanks.)
* `RestrictTankTurning` (default 0): Prevent the tanks from turning while moving. A strange option, but maybe you like it, I won't judge.
* `NoTankAcceleration` (default 0): Tanks no longer accelerate. Only added because JS Tanks had this option (which only had the option because there was a time before tank acceleration existed).
* `AlwaysShootingMode` (default 0): Tanks are always firing. Intended as a debug option.

## CONTROLS

* `Tank1.Name` (default "WASD"): Sets the left tank's name. Since tank names aren't drawn anymore, this doesn't show up anywhere.
* `Tank2.Name` (default "Arrow Keys"): Sets the right tank's name.
* `Tank1.TeamName` (default "\\"WASD 4 Life\\""): Sets the left tank's team name. Only viewable when scores are printed to the console.
* `Tank2.TeamName` (default "\\"Arrow Keys R WINZ\\""): Sets the right tank's team name.
* `Tank1.Forward` (default w): Left tank's forward key. The keyboard key's names are from AHK: https://www.autohotkey.com/docs/commands/Send.htm#keynames
* `Tank1.Left` (default a): Left tank's turn CCW (counterclockwise) key.
* `Tank1.Right` (default d): Left tank's turn CW (clockwise) key.
* `Tank1.Shoot` (default s): Left tank's shoot key.
* `Tank1.Special` (default e): Left tank's "special" key. Not really used for anything.
* `Tank2.Forward` (default Up): Right tank's forward key. The keyboard key's names are from AHK: https://www.autohotkey.com/docs/commands/Send.htm#keynames
* `Tank2.Left` (default Left): Right tank's turn CCW key.
* `Tank2.Right` (default Right): Right tank's turn CW key.
* `Tank2.Shoot` (default Down): Right tank's shoot key.
* `Tank2.Special` (default /): Right tank's "special" key. Not really used for anything.

## SPECIFIC_ADJUSTMENTS

* `BananaPower.ExplodeCount` (default 8): Sets the number of bullets Banana spits out when it explodes. Currently does nothing.
