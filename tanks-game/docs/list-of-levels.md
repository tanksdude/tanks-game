# List of Every Level

TODO: images

## Default Random

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `default_random`
* Default color: random

A level with random walls, random powerups, and random hazards. Anything can happen.

## Empty

* Types: "vanilla", "random-vanilla", "old", "random"
* Internal name: `empty`
* Default color: `(0.0, 0.0, 0.0)` (unnecessary)

It's empty. With no walls, powerups, or hazards to get in the way, this is a true test of skill!

As such, it's about as entertaining as no items, Fox only, Final Destination.

### Old Empty

* Types: "old", "random-old"
* Internal name: `old_empty`
* Default color: `(0.0, 0.0, 0.0)`

It's Empty, but with the classic JS walls. In some ways it's better since at least there's cover, but that also means corner camping is a very viable strategy.

## Evened Corridors

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `evened_corridors`
* Default color: `(0.125, 0.5, 1.0)`

Not a horribly interesting level. It was originally made to make Bounce look cool, and the main gimmick was the long corridors leading to a good powerup as a risk vs. reward opportunity. Didn't make the transition from JS Tanks very well, or maybe the fact is was one of the first levels ever means is was never very good. Maybe the Big powerups replacing the Barrier powerups contributed to this. Regardless, there will probably come a day when it is removed from vanilla.

## Big Fun

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `bigfun`
* Default color: `(0.75, 0.25, 0.5)`

A recreation of the JS Tanks random level at a really old point, around when Big was added.

It was very fun to rush to grab a Big powerup and flood the whole level with it (with superfast shooting), so I tried to recreate that experience in this version. Haven't quite nailed the feeling yet. Needs Big bullets to be able to pass through each other to properly recreate the feeling.

## Few Obstacles

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `few_obstacles`
* Default color: `(0.25, 0.75, 0x99/255)`

A level with few obstacles (wow, such an original name). You could rush for the center, or you could quickly grab a Homing power and take cover. If you rush for the center carelessly, you may pick up a Blast powerup, which is good for defense but not offense. Requires lots of careful moves in a small environment with minimal cover.

In JS Tanks, since power mixing was not very good, the level was not quite as interesting. But now it's my go-to spot to show off how awesome power mixing is!

## Many Hazards

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `many_hazards`
* Default color: `(0xDD/255, 0.5, 0.25)`

Have you ever wondered what a really dangerous level looks like? A truly hostile environment unsuited for vulnerable tanks? Look no further than this! Dodge like 40 total hazards for meager rewards, and probably die along the way.

## Hiding Places

* Types: "vanilla-extra", "random-vanilla", "old", "random-old"
* Internal name: `hiding_places`
* Default color: `(0.375, 0.75, 0.125)`

As the name suggests, there are little cubbies to hide in. Although their effectiveness may be limited if then enemy grabs Bounce and Multishot.

One of the two levels to have a Mines powerup. Arguably fits somewhat decently.

## Sneaky Reward

* Types: "vanilla-extra", "random-vanilla"
* Internal name: `sneaky_reward`
* Default color: `#9966CC` (amethyst)
* Level effects: Respawning Powerups

Two powerups hidden away, with some strange powerups next to them. How will you get them? And maybe avoid the center.

Coincidentally, this was the first new level for this version. Which is a little sad. Especially when it was mostly a test level that I thought was barely passable as a real level.

## Tight Patrolling Corridor

* Types: "vanilla-extra", "random-vanilla"
* Internal name: `tight_patrolling_corridor`
* Default color: `(15/16, 15/16, 14/16)` (very white beige)

Re-enact your favorite spy escaping from the enemy headquarters! Many turrets move around, guarding the elusive powerup in the center. Maybe you could quickly run down a different corridor to grab some speed. Should the enemy's bullets get too close to the starting area, try grabbing some invincibility.

Currently the only level with Patrolling Turrets because they don't randomize yet.

## Dangerous Center

* Types: "vanilla-extra", "random-vanilla"
* Internal name: `dangerous_center`
* Default color: `#E691A9`
* Level effects: Respawning Powerups

Will you take cover from the very hostile center, or will you claim the rewards? You could always grab a Big to thin the Targeting Turret population, but beware the reduced speed and inevitable destruction of your cover.

This level was partially made from randomly throwing down some walls and really trying to fit as much as possible in a limited space. Not a great level but good enough.

## Untraditional Divider

* Types: "vanilla-extra", "random-vanilla"
* Internal name: `untraditional_divider`
* Default color: `#76C67D`
* Level effects: Respawning Powerups, Magnetism, Minefield × 4

Instead of the usual vertical dividers, there's only one horizontal wall?! Keep your wits about you as you try to get a powerup, because all of them are in treacherous locations!

The entire inspiration for this level was making the usual vertical wall separators a massive horizontal wall instead. Then adding multiple Minefields because that seemed interesting, finishing up with a few turrets in the lava. I don't think it's good enough for vanilla, but it's too unique to ignore.

## Tricky Maneuvering

* Types: "vanilla-extra", "random-vanilla"
* Internal name: `tricky_maneuvering`
* Default color: `(0.5, 0.75, 1.0)`
* Level effects: Wind

Useful powerups lurk in the center, but beware sudden wind blowing you off course! Maybe test your luck and carefully grab a powerup by the Lava.

## Mine Heaven

* Types: "vanilla-extra", "random-vanilla", "old", "random-old"
* Internal name: `mine_heaven`
* Default color: `(0.25, 0.25, 0.25)`
* Level effects: Ice, Minefield

Do you love the Mines power? Of course you do! This level is your dream come true: nothing but Mines powerups as far as the eye can see. But beware, new mines keep showing up, and the battlefield is covered in ice!

## Winning Path

* Types: "vanilla-extra", "random-vanilla", "old", "random-old"
* Internal name: `winning_path`
* Default color: `(0.0, 0.0, 0.0)` (unnecessary)
* Level effects: Invisible Walls

Is this the empty level but with a cool powerup in the center? Nope, sike! The walls are just invisible! If you can make it to the center, you'll be rewarded with the amazing Godmode!

Pro tip: since bullets die when hitting walls, you can shoot to see where walls are. No one has ever figured this out by themselves so I still have work to do as a level designer.

## Concealed Powerups

* Types: "old", "random-old"
* Internal name: `concealed_powerups`
* Default color: `#AABB99`

A few good powerups are located in the center, but they are guarded by Lightning. Decide between the reliable Homing or the elusive Grenade and Big.

Not a very fun level, even for JS Tanks. Removed from vanilla for that reason. The only thing going for it was the Homing powerups, which are admittedly a fun addition, but that's it; it's too boring.

## Lightning Corners

* Types: "old"
* Internal name: `lightning_corners`
* Default color: `(0x77/255, 0x55/255, 0.75)`

An amazing powerup guarded in the center. Blatantly unfinished as a level.

Was likely originally made to test powerup mixing back in JS Tanks. And since I never figured out how to turn it a real level, I commented it out. But for some reason I ported it to C++, so that's why it's here.

## Lone Turret

* Types: "old", "random-old"
* Internal name: `lone_turret`
* Default color: `(0.75, 0.25, 0.25)`

A single turret in the center watches for any enemies. Stay too long deciding on the powerup you'll take and you'll be face its small wrath!

Back in JS Tanks, it was the very first level to have a turret! That's why it has nothing else going for it, and it's not very fun. (And surprise surprise, it started as a test level to make sure the Targeting Turret worked.) Removed from vanilla because of that.

## Old Portal Level

* Types: "old"
* Internal name: `old_portal`
* Default color: `#AA22FF`
* Level effects: no portals :(

The only thing going for this level is the portal effect: anything that passes from the top to bottom or vice versa will be transported to the other side. Except that was very complicated to implement, even in a non-hacky way, so the core feature of this level is not here.

I realized I had every level from JS Tanks in this version, including the cut Lightning Corners, *but* I remembered the portal level. The hope was that this C++ version would make the portal level effect far easier to implement, but that clearly didn't work out.

## [DEV] Developer Level 0

* Types: "dev", "random-dev"
* Internal name: `dev0`
* Default color: random

The first test level (of this version), which should be obvious from the power placement. Also has a Stationary Turret, likely because I didn't have another test level yet to put down a hazard.

## [DEV] Developer Level 1

* Types: "dev", "random-dev"
* Internal name: `dev1`
* Default color: `(0.25, 0.25, 0.25)`

A test level with every early hazard (except Stationary Turret, I guess). Also shows off a Vertical Lightning using the edge as an anchor point. An Ultra Bounce powerup was added much later.

## [DEV] Developer Level 2

* Types: "dev", "random-dev"
* Internal name: `dev2`
* Default color: `(0.25, 0.25, 0.25)`

A test level originally made for testing Banana by being very empty. Has *a lot* of powerups.

## [DEV] Developer Level 3

* Types: "dev", "random-dev"
* Internal name: `dev3`
* Default color: `(0.25, 0.25, 0.25)`

Another test level, probably made because I got sick of adding powerups to Dev2 and needed some walls. Features a Mother Turret and a Patrolling Turret.

## [DEV] Unnamed Level 1

* Types: "dev", "random-dev"
* Internal name: `unnamed1`
* Default color: `(1.0, 1.0, 1.0)`

An unfinished level. The wall placement is mostly fine, but I couldn't figure out how to turn it into a real level.

## [DEV] Unnamed Level 2

* Types: "dev", "random-dev"
* Internal name: `unnamed2`
* Default color: `(1.0, 1.0, 1.0)`

A test level featuring a zig-zag, with the jaggedness controllable (at compile-time).

## [DEV] Unnamed Level 3

* Types: "dev", "random-dev"
* Internal name: `unnamed3`
* Default color: `(1.0, 1.0, 1.0)`
* Level effects: Respawning Powerups

Just a test level with a Ginormous Turret. At least functions as a level, just not very interesting.

## [DEV] Unnamed Level 4

* Types: "dev", "random-dev"
* Internal name: `unnamed4`
* Default color: `(1.0, 1.0, 1.0)`
* Level effects: Respawning Powerups

Tested "enclosing" the main part of the level. There's definitely a nugget of something usable here.

## [DEV] No Walls 1

* Types: "dev", "random-dev"
* Internal name: `nowalls1`
* Default color: `(0.0, 0.0, 0.0)` (unnecessary)

Tested using No Bullet Zones instead of walls. An idea worth investigating further.

## Timed Reward

* Types: "vanilla-extra", "random-vanilla", "dev", "random-dev"
* Internal name: `timed_reward`
* Default color: `#974646`

Tested having the walls continuously move. Cool! Decided to move it to random-vanilla solely to show off the moving walls. It's not good enough to be called a real level, but on the bright side, there are moving walls, so it was put in random-vanilla.

# List of protected level types

These are the types custom levels are not allowed to have, unless `[MODS] ModSafetyChecks` in the INI file is disabled.

"vanilla", "vanilla-extra", "random-vanilla", "old", "random-old", "dev", "random-dev"

# Quick list for types

## vanilla

* Default Random
* Empty
* Evened Corridors
* Big Fun
* Few Obstacles
* Many Hazards

## vanilla-extra

* Hiding Places
* Sneaky Reward
* Tight Patrolling Corridor
* Dangerous Center
* Untraditional Divider
* Tricky Maneuvering
* Mine Heaven
* Winning Path
* Timed Reward

## random-vanilla

* Default Random
* Empty
* Evened Corridors
* Big Fun
* Few Obstacles
* Many Hazards
* Hiding Places
* Sneaky Reward
* Tight Patrolling Corridor
* Dangerous Center
* Untraditional Divider
* Tricky Maneuvering
* Mine Heaven
* Winning Path
* Timed Reward

## old

* Empty
* Evened Corridors
* Big Fun
* Few Obstacles
* Many Hazards
* Hiding Places
* Tricky Maneuvering
* Mine Heaven
* Winning Path
* Concealed Powerups
* Lightning Corners
* Lone Turret
* Old Empty
* Old Portal

## random-old

* Evened Corridors
* Big Fun
* Few Obstacles
* Many Hazards
* Hiding Places
* Tricky Maneuvering
* Mine Heaven
* Winning Path
* Concealed Powerups
* Lone Turret
* Old Empty

## random

* Default Random
* Empty
* Evened Corridors
* Big Fun
* Few Obstacles
* Many Hazards

## dev

* Developer Level 0
* Developer Level 1
* Developer Level 2
* Developer Level 3
* Unnamed Level 1
* Unnamed Level 2
* Unnamed Level 3
* Unnamed Level 4
* No Walls 1
* Timed Reward

## random-dev

* Developer Level 0
* Developer Level 1
* Developer Level 2
* Developer Level 3
* Unnamed Level 1
* Unnamed Level 2
* Unnamed Level 3
* Unnamed Level 4
* No Walls 1
* Timed Reward
