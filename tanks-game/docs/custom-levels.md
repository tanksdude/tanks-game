# Custom Level Documentation

## Getting started

In order to make custom levels, all you need is a text editor. I used Notepad out of laziness. Here's the steps you'll need to take to make a custom level:

1. Navigate to the `mods` folder.
1. Create a folder. This will be the name of your mod.
1. Add a `levels` folder to your mod's folder. Your custom levels go here.
1. Create a text file; this is your level. Name it however you want, but it should be related to your level's name.
1. It is recommended for you to look at the other mods' levels to get a feel on syntax.

And that's all you need. Welcome to the world of custom levels! Unfortunately, custom levels are far more limited than real levels, but at least it's something.

Some general notes:

* Whitespace between arguments doesn't matter (as long as there is at least one space).
* Every line can only have one assignment or command. If you have a really long line, sorry, but I recommend turning on word wrap.
* Sorry, but there's no math parsing when supplying numbers. You can't do something like `(16+8)/2`; you must do `12` instead.
* Comments can be added by using a semicolon (`;`) or number sign (`#`).
* The size of a level is 640×320.

## Initialization stuff

Required: every level needs a name, default color, types, and weights for the types.

* `Name` (string): The name of the level. Technically only used if you want the first level to be your level, because randomly selecting levels doesn't need their names.
* `Color` (3 floats in range [0,1] for RGB): The default color for walls.
* `LevelTypes` (at least one string): The "types" or "playlists" the level is in. Recommended "modname" and "random-modname". 
* `LevelWeights` (1 float for each type): The weighting in each playlist when randomly choosing from the playlist. Recommended 1.0 as the base weight.

Optional: choosing where the tanks start.

* `RandomStartPositionCount` (int, default = 5): The number of starting positions.
* `RandomStartPositionXValue` (float, default = 20): The x-distance from the edge.
* `RandomStartPositionYRange` (float, default = 256=320-2*32): The y-range for starting positions.

After you have set the required and optional attributes, you need to write `[LEVEL_START]` to begin actual level commands.

## Command list

### WALL

Syntax: `WALL   <center x> <center y>   <width> <height>`

Creates a wall at the specified point. Note that you are supplying the wall's *center* location, not its corner.

### WALL_LR and WALL_UD

Syntax: `WALL_LR/UD   <center x> <center y> <x/y offset from center>   <width> <height>`

Creates two walls from the specified point, offset from the center point to the walls' edges. For example, if you `WALL_UD 320 160 **20**   80 20`, there will a gap of **40** between the walls.

### WALL_Corners, WALL_DiagForwardSlash, and WALL_DiagBackwardSlash

Syntax: `WALL_Corners/DiagForwardSlash/DiagBackwardSlash   <center x> <center y> <x offset from center> <y offset from center>   <width> <height>`

`WALL_CORNERS` creates four walls from the specified point, and `WALL_DiagForwardSlash` and `WALL_DiagBackwardSlash` create two opposing walls in a forward slash (/, bottom left and top right) or backslash (\\, top left and bottom right) orientation (don't judge the naming scheme). The offset is the distance from the center point to the walls' corners.

### RANDOM_WALLS

Syntax: `RANDOM_WALLS   <area center x> <area center y> <area width> <area height>   <wall count>`

Alternate: `RANDOM_WALLS   <area center x> <area center y> <area width> <area height>   <wall count>   <wall min width> <wall max width> <wall min height> <wall max height>`

Creates `<wall count>` walls in the area with random widths and heights.

### CLASSIC_WALLS

Syntax: `CLASSIC_WALLS`

Creates the four walls present in every level in JS Tanks. Still present in some C++ levels.

### POWER

Syntax: `POWER   <center x> <center y>   <power type 1> <power name 1> [<power type n> <power name n>]`

Creates a powerup at the specified location. If you want the powerup to hold more than one power, just keep appending them.

Example 1: `POWER   320 160   "vanilla" "speed"`

Example 2: `POWER   320 160   "vanilla" "speed" "vanilla-extra" "shotgun"`

Example 3: `POWER   320 160   "vanilla" "speed" "vanilla-extra" "shotgun" "dev" "inversion"`.

See `docs/list-of-powers.md` for a list of every power.

### POWER_LR and POWER_UD

Syntax: `POWER_LR/UD   <center x> <center y> <x/y offset from center>   <power type 1> <power name 1> [<power type n> <power name n>]`

Creates two powerups from the specified location, offset from the center point.

### POWER_Corners, POWER_DiagForwardSlash, and POWER_DiagBackwardSlash

Syntax: `POWER_Corners/DiagForwardSlash/DiagBackwardSlash   <center x> <center y> <x offset from center> <y offset from center>   <power type 1> <power name 1> [<power type n> <power name n>]`

`POWER_CORNERS` creates four powerups from the specified point, and `POWER_DiagForwardSlash` and `POWER_DiagBackwardSlash` create two opposing powerups in a forward slash (/, bottom left and top right) or backslash (\\, top left and bottom right) orientation (don't judge the naming scheme).

### POWER_LR_Alternate and POWER_UD_Alternate

Syntax: `POWER_LR/UD_Alternate   <center x> <center y> <x/y offset from center>   <first power type 1> <first power name 1> [<first power type n> <first power name n>]  |  <second power type 1> <second power name 1> [<second power type n> <second power name n>]`

`POWER_LR/UD` but which powerup is on which side is determined when the level is initialized. This allows for a little bit of randomization while staying mostly fair. The powerups are separated by `|`.

### POWER_Corners_Alternate, POWER_DiagForwardSlash_Alternate, and POWER_DiagBackwardSlash_Alternate

Syntax: `POWER_Corners/DiagForwardSlash/DiagBackwardSlash_Alternate   <center x> <center y> <x offset from center> <y offset from center>   <first power type 1> <first power name 1> [<first power type n> <first power name n>]  |  <second power type 1> <second power name 1> [<second power type n> <second power name n>]`

`POWER_Corners/DiagForwardSlash/DiagBackwardSlash` but which powerup is on which side is determined when the level is initialized. This allows for a little bit of randomization while staying mostly fair. The powerups are separated by `|`.

### CHAZARD and RHAZARD

Syntax: `CHAZARD   <hazard type> <hazard name>   <center x> <center y> [<radius or angle> or <radius> <angle>]`

Syntax: `RHAZARD   <hazard type> <hazard name>   <center x> <center y> [<width> <height>]`

How many parameters to give depends on the hazard. For example, a Targeting Turret needs an angle while a No Bullet Zone needs a radius.

See `docs/list-of-hazards.md` for a list of every hazard, which also gives their construction type (and thus how many parameters are needed). Some hazards can take even more parameters; this is not supported at this time. Some hazards are too complex for this system (Patrolling Turret) and can't be made for the time being.
