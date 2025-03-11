# List of Every Power

## Speed

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `speed`
* Attributes: "stack", "mix"
* Color: `(0.25, 0.25, 1.0)` (blue)
* Tank modifiers: double speed (stacks)

Exactly as the name suggests: doubles max speed. (Does not double acceleration.)

Treated as the generic placeholder power, mainly because it was first (in JS and C++) because it was so easy.

## Wallhack

* Types: "vanilla", "random-vanilla", "ultimate", "ultimate-vanilla", "random"
* Internal name: `wallhack`
* Attributes: "stack", "mix"
* Color: `(1.0, 1.0, 0.0)` (yellow)
* Duration: half

Ignores walls, not interacting with them at all. Do not pass GO, do not collect $200.

In the C++ version, this is the only power to have a non-standard duration (excluding The Super). Never got around to making powers have longer durations.

## Bounce

* Types: "vanilla", "random-vanilla", "old", "supermix", "supermix-vanilla", "random"
* Internal name: `bounce`
* Attributes: "mix"
* Color: `(1.0, 0.0, 0.75)` (pink)
* Bullet modifiers: half speed

Bounces off walls and edges. Can bounce a maximum of 16 times.

### Old Bounce

* Types: "old", "random-old"
* Internal name: `old_bounce`
* Tank modifiers (new): half acceleration, half radius, half shooting cooldown
* Bullet modifiers (new): quarter speed

Like Bounce, but the tank's radius is halved. Why? Well, it was one of the first powerups (fourth, right after Speed, Invincible, and Wallhack), and I wanted to make it a little more different and stand out. I think, I dunno, it was a long time ago. Maybe I just thought it was funny seeing a small tank shooting tiny bouncing bullets, maybe I wanted to have a powerup change the radius to test things out. Regardless, this power was made just for that reason.

## Multishot

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `multishot`
* Attributes: "stack", "mix"
* Color: `(0.5, 0.0, 1.0)` (purple)
* Tank modifiers: half shooting cooldown

Shoots 8 bullets. Can stack. Is fun.

## Triple

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `triple`
* Attributes: "mix"
* Color: `(0.75, 0.75, 0.5)` (beige)

Shoots three bullets parallel to each other.

## Homing

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `homing`
* Attributes: "stack", "mix"
* Color: `#BB66AA` (really faded purple)
* Bullet modifiers: half speed

Makes bullets home in on the enemy tank.

If you're sneaky, you can fire this into the up/down out of bounds and have it come back in bounds.

## Invincible

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `invincible`
* Attributes: "mix"
* Color: `(1.0, 1.0, 1.0)` (white)
* Tank modifiers: half speed, half acceleration

In exchange for being half as fast, the tank and its bullets are invincible\*! (\*Terms and conditions apply. Some things are more invincible than this.)

* Tank defense: 2.5 (a little more than a Big bullet)
* Bullet offense: 1
* Bullet defense: 1

## Big

* Types: "vanilla", "random-vanilla", "old", "random"
* Internal name: `big`
* Attributes: "stack", "mix"
* Color: `(1.0, 0.0, 0.0)` (red)
* Tank modifiers: half speed, half acceleration, quadruple shooting cooldown
* Bullet modifiers: quarter speed, quadruple size (stacks)

The bullets are now **big**. Due to their **big**ness, they destroy walls and some hazards. As a reminder, the bullets are **BIG**. And yes, the **big**ness stacks; three levels of **big** is enough to engulf the whole screen.

* Bullet offense: 2 (`DESTRUCTION_TIER`)
* Bullet defense: 2 (`DESTRUCTION_TIER`)

### Old Big

* Types: "old", "random-old"
* Internal name: `old_big`
* Bullet modifiers (new): half speed

Like Big, but the bullets move faster. Pretty pointless.

Might've been made for Lightning Corners level, then realized it wasn't needed.

## MegaDeath (WIP)

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `megadeath`
* Attributes: "mix"
* Color: `#4F3E3E` (brown-ish)
* Tank modifiers: quadruple shooting cooldown
* Bullet modifiers: quarter speed, +1/64 acceleration, ×65/64 grow

Note that this power is a work in progress. I never quite figured out what it should be. I also kept telling myself "it should really be removed from vanilla by now" but never bothered...

* Bullet offense: variable, depends on size; `r / (Bullet::default_radius * 4) * DESTRUCTION_TIER` (size relative to a Big bullet times `DESTRUCTION_TIER`)
* Bullet defense: variable, depends on size; `r / (Bullet::default_radius * 4) * DESTRUCTION_TIER`

## Grenade

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `grenade`
* Attributes: "mix"
* Color: `(0.75, 1.0, 0.0)` (lime green)
* Bullet modifiers: -1/16 acceleration, .875 degrade, ×65/64 grow (stationary)

After the bullet comes to a stop, it starts growing. Can pass through walls, though doing so degrades the bullet extra (where degrading is slowing down or losing "life force").

## Fire

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `fire`
* Attributes: "mix"
* Color: `(1.0, 0.5, 0.0)` (orange)
* Tank modifiers: half shooting cooldown, half turning speed
* Bullet modifiers: half speed, [-3/32, -1/32] acceleration, .5 degrade, +1.5/32 grow (moving)

Shoots four bullets, ±45° from the cannon. The bullets grow a small amount before coming to a stop. Eventually decays.

## Blast

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `blast`
* Attributes: "mix"
* Color: `#446688` (dark blue)
* Tank modifiers: half speed, half acceleration
* Bullet modifiers: quarter radius, [-3/16, -1/16] acceleration, .25 degrade

Shoots 16 bullets, ±60° from the cannon. Eventually decays. Used to stick to walls when it touched them, but this no longer happens due to a code restructuring.

## Banana

* Types: "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random"
* Internal name: `banana`
* Attributes: "stack", "mix"
* Color: `#999933` (dull gold)
* Bullet modifiers: -1/8 acceleration

When the bullet comes to a stop, it explodes into 8 smaller bullets! Very fun!

Why "banana"? It was named after the [banana bomb from Worms](https://worms.fandom.com/wiki/Banana_Bomb) because I thought every cluster bomb in games were called "banana bombs." Turns out [Worms](https://en.wikipedia.org/wiki/Worms_(series)) is the only place where it's called that, but no way do I want to rename this *awesome* power to "cluster" or something.

## Godmode

* Types: "vanilla", "old", "ultimate", "ultimate-vanilla"
* Internal name: `godmode`
* Attributes: none
* Color: rainbow! (changes over time)
* Color importance: 1.0

It's every power at once\*! Have fun!

(\*Actually, it's every power in the "supermix-vanilla" group.)

## Mines

* Types: "vanilla-extra", "old"
* Internal name: `mines`
* Attributes: "mix"
* Color: `(0.0, 0.0, 0.0)` (black)
* Color importance: 0.5
* Tank modifiers: half shooting cooldown
* Bullet modifiers: zero speed, zero acceleration (importance: 0.5)

Lays down a "mine," which is a bullet with zero velocity. And since the tank is completely black, it may be difficult to tell which way the tank is facing.

Only appears in Hiding Places level and Mine Heaven level because it doesn't really fit in with the rest of the game...

### Old Mines

* Types: "old"
* Internal name: `old_mines`
* Tank modifiers (new): half shooting cooldown (stacks)

Identical to Mines, except the shooting speed multiplier stacks (because that's how all powers worked in JS Tanks). Was necessary to recreate the feel of the center powerup in the Mine Heaven level.

## Shotgun

* Types: "vanilla-extra", "random-vanilla", "old", "random-old", "random"
* Internal name: `shotgun`
* Attributes: "mix"
* Color: `(0.0, 1.0, 0.25)` (green)
* Bullet modifiers: half speed

Shoots 5 bullets parallel. Like Triple, but spread out a lot more.

## Tracking

* Types: "vanilla-extra", "old", "random-old"
* Internal name: `tracking`
* Attributes: "mix"
* Color: `(0xDD/255, 0.25, 0.25)` (faded red-orange ("maroon"))
* Tank modifiers: half speed, half acceleration, double shooting cooldown
* Bullet modifiers: quarter speed

The bullets aim right for the enemy tank, and can even go through walls, although they are slow and can be defeated just as easily as any other bullet.

## Barrier

* Types: "vanilla-extra", "random-vanilla", "old", "random-old", "random"
* Internal name: `barrier`
* Attributes: "stack", "mix"
* Color: `(0.0, 0.5, 0.25)` (dark green)

The tank and its bullets have an extra life!

### Shield

* Types: "vanilla-extra"
* Internal name: `shield`
* Attributes: "mix"
* Color: `#B6E69D` (approximately Barrier but yellow, gray, and light)

The tank has several extra lives, up to ten! (Though the bullets still only get one extra life.)

## Swarm

* Types: "vanilla-extra", "random-vanilla", "random"
* Internal name: `swarm`
* Attributes: "mix"
* Color: `#E6C522`
* Tank modifiers: half shooting cooldown
* Bullet modifiers: half speed

The bullets orbit around the tank, allowing an intense swarm of bullets to form!

## [DEV] Long Invincible

* Types: "dev"
* Internal name: `longinvincible`
* Attributes: "mix"
* Color: `(1.0, 1.0, 1.0)` (white)
* Duration: infinite

Identical to Invincible, except it lasts forever. Was made to make testing easier.

* Tank defense: 2.5 (a little more than a Big bullet)
* Bullet offense: 1
* Bullet defense: 1

### [DEV] Colorless Long Invincible

* Types: "dev"
* Internal name: `longinvincible`
* Color importance: -1 (doesn't show)

Same as Long Invincible, but this time it cheats to not show any color. Was made to make getting screenshots easier.

## [DEV] The Super

* Types: "dev"
* Internal name: `the_super`
* Attributes: "mix"
* Color: cycle between black and white
* Duration: double

The strongest power ever. (Though it's still stopped by No Bullet Zones...)

Originally intended to be one step above Big (so two steps above Invincible), now it's the ultimate power.

* Tank defense: 999 (`HIGH_TIER`)
* Bullet offense: 998.9 (`HIGH_TIER - .1`)
* Bullet defense: 998.9 (`HIGH_TIER - .1`)

## Inversion

* Types: "dev", "random-dev"
* Internal name: `inversion`
* Attributes: "stack", "mix"
* Color: `(0.75, 0.75, 0.875)` (silver-teal)
* Tank modifiers: inverted turning speed (stacks)

Inverts tank turning.

## Annoying

* Types: "dev", "random-dev"
* Internal name: `annoying`
* Attributes: "mix"
* Color: `#4B0082` (indigo)
* Tank modifiers: half acceleration
* Bullet modifiers: half speed

It's annoying. You get to go out of bounds by one tank diameter, and your velocity is reset when picking it up, and your bullets push the other tank instead of killing! Well, most of those aren't very annoying: going a little out of bounds is just a bit weird, velocity getting reset is hardly worth acknowledging, and bullets being unable to kill is more bad than annoying (though it is funny watching the tank getting pushed). The original plan for this power was to just go out of bounds a bit, and after adding that ability, I realized it didn't live up to its "annoying" name, so I added some more stuff.

The color is indigo because whenever you go to pick up a purple colored pencil, you always find out it's indigo, not purple, which is annoying.

Currently considered a "dev" power because I'm not sure whether I should modify this before putting it in vanilla. However, it does appear in Sneaky Reward level... though that is a test-ish level that I pushed to vanilla.

* Bullet offense: 0 (importance: 0.5)
* Bullet defense: 0 (importance: 0) (that's default though)

## Ultra Bounce

* Types: "dev", "random-dev"
* Internal name: `ultrabounce`
* Attributes: "mix"
* Color: `#E153DB` (pink but kinda white and purple)
* Bullet modifiers: quarter speed

Bounces just like Bounce, but now walls get pushed back a little! A totally radical xtreme power.

## [DEV] Color Changing

* Types: "dev", "random-dev"
* Internal name: `color_changing`
* Attributes: "mix"
* Color: changes based on distance to the other tank (grayscale-only)
* Color importance: 0.5

Was a test on changing the color based on proximity. Doesn't work very well. Inspiration came from Godmode being able to change its color.

## [DEV] Other Stuff is Poison

* Types: "dev", "random-dev"
* Internal name: `other_stuff_is_poison`
* Attributes: "mix"
* Color: `#A3D63E` (greenish-yellow)
* Tank modifiers: half acceleration

Wanted to test making a power that kills the tank when touching safe things (walls and hazards). Does exactly what it sets out to do.

## [DEV] Backwards Movement

* Types: "dev", "random-dev"
* Internal name: `backwards_movement`
* Attributes: "mix"
* Color: `#52AADD` (light blue-ish)

This game originated from making something with tank-like movement and simple controls. For some reason, old me thought "tank controls" did not involve moving backwards, so that's why the tanks can't move backwards. (Another reason is that that key is taken by shooting, and I wanted to keep the controls simple.) Eventually I wanted to try to add backwards movement as an option to see how it would change the game, and this is a test at that (press the special key (`e` and `/`) to move backwards). Doesn't work very well because powers can't conditionally tie into acceleration, only velocity. Hopefully some day a real implementation will come around.

## Wall Sparks

* Types: "dev", "random-dev"
* Internal name: `wall_sparks`
* Attributes: "mix"
* Color: `#BC3828` (blood orange)

When bouncing off a wall, a few bullet "sparks" get created. Actually functional, just kinda boring.

## Edge Sparks

* Types: "dev", "random-dev"
* Internal name: `edge_sparks`
* Attributes: "mix"
* Color: `#BC3828` (same as Wall Sparks for now)

When bouncing off an edge, a few bullet "sparks" get created. Does not bounce off walls.

## [DEV] Weird Extra Cannons

* Types: "dev", "random-dev"
* Internal name: `weird_extra_cannons`
* Attributes: "stack", "mix"
* Color: `(0.625, 0.625, 0.625)` (light-ish gray)
* Tank modifiers: half shooting cooldown

Adds a cannon at 90° and 225°, just to test adding non-symmetric cannons. When adding extra cannons was added (pre-v0.0.7), I got the idea to make a power like this to test it out the system. Forgot about it, finally remembered, added it in much later (v0.2.5). Well, it works, so test success.

## [DEV] Triple Spread

* Types: "dev", "random-dev"
* Internal name: `triple_spread`
* Attributes: "mix"
* Color: `(0.85, 0.85, 0.60)` (beige but a bit brighter)

Sorta like Triple, except the angle variation comes at the cannon point instead of the center. Basically, Triple makes three bullets parallel, while this makes three at the same point that spread out. Got the idea a long time ago, finally decided to implement it once custom powers became a thing.

## [DEV] Ring Shooter

* Types: "dev", "random-dev"
* Internal name: `ring_shooter`
* Attributes: "mix"
* Color: `(0.6, 0.1, 1.0)` (purple but a bit brighter)
* Tank modifiers: half shooting cooldown
* Bullet modifiers: half speed

Multishot, but all the bullets face the same way. Inspiration came from my very first implementation of tank shooting with cannon points, where Multishot did that. The error was quickly rectified but the memory lived on, so I made this.

## [DEV] "Ancient" MegaDeath

* Types: "dev", "random-dev"
* Internal name: `ancient_megadeath`
* Attributes: "mix"
* Color: `#201A1A`
* Tank modifiers: double shooting cooldown
* Bullet modifiers: 1/64 speed, +1/64 acceleration

A recreation of the very first (unseen) implementation of MegaDeath in JS Tanks. It was pretty bad, but it did live up to its name "mega-death" (by being OP). I think it had increasing acceleration ([jerk](https://en.wikipedia.org/wiki/Jerk_(physics))) because I remembered it being far more menacing(ly short-lived).

Fun fact: MegaDeath actually did appear in JS Tanks! It only appears in the mixing with Godmode. However, it only existed because power mixing was very bad back then. I probably forgot because Godmode spits out so many bullets and finding the single one behaving differently is a challenge. At best, you might notice 8 slightly larger bullets because it Banana'd. However, if you gave it enough time to grow (which was very difficult given its normal spawning locations (center of Winning Path and the random level)), it would grow very fast (same speed as this version, actually) and pass through walls (because conditionally enabling destroying walls was hard).

* Bullet offense: 2 (`DESTRUCTION_TIER`)
* Bullet defense: 2 (`DESTRUCTION_TIER`)

## [DEV] Banana Split

* Types: "dev", "random-dev"
* Internal name: `banana_split`
* Attributes: "stack", "mix"
* Color: `#99BB33` (dull gold with more green (because Shotgun is green and this power was loosely based on Banana+Shotgun))
* Bullet modifiers: -1/8 acceleration (same as Banana)

When the bullet comes to a stop, it Bananas into 4 bullets at set angles. Worth investigating further.

## Trickster Circle

* Types: "dev", "random-dev"
* Internal name: `trickster_circle`
* Attributes: "mix"
* Color: `#001659` (really dark blue)
* Bullet modifiers: half speed

The bullets move in a circle motion (while traveling forward like normal).

The color is loosely based on [Mewtwo's Shadow Ball](https://www.ssbwiki.com/Mewtwo_(SSBU)), because that's where this power's inspiration comes from.

## Trickster Snake

* Types: "dev", "random-dev"
* Internal name: `trickster_snake`
* Attributes: "mix"
* Color: `#D9B3FF` (really faded purple-ish)

The bullets move up and down (while traveling forward like normal). This is the better version of this power.

The color is loosely based on [Mewtwo's Shadow Ball](https://www.ssbwiki.com/Mewtwo_(SSB4)), because that's where this power's inspiration comes from.

## [DEV] Pusher

* Types: "dev", "random-dev"
* Internal name: `pusher`
* Attributes: "mix"
* Color: `#BFBF30` (big construction vehicle yellow (kinda))

Allows the tank to push walls and solid hazards.

## [DEV] Movement Restarter

* Types: "dev", "random-dev"
* Internal name: `movement_restarter`
* Attributes: "stack", "mix"
* Color: `#838996` ("Roman Silver" apparently)

Once the bullet stops moving, it will be kickstarted into moving again by multiplying its acceleration by -1.25. Only happens once.

## Mine Layer

* Types: "dev", "random-dev"
* Internal name: `mine_layer`
* Attributes: "mix"
* Color: `(0.25, 0.25, 0.25)` (dark gray)

As the bullet travels along, it will leave behind some (non-moving) bullets.

# List of protected power types

These are the types custom powers are not allowed to have, unless `[MODS] ModSafetyChecks` in the INI file is disabled.

"vanilla", "vanilla-extra", "random-vanilla", "old", "random-old", "supermix-vanilla", "ultimate-vanilla", "dev", "random-dev"

# Quick list for types

## vanilla

* Speed
* Wallhack
* Bounce
* Multishot
* Triple
* Homing
* Invincible
* Big
* MegaDeath
* Grenade
* Fire
* Blast
* Banana
* Godmode

## vanilla-extra

* Mines
* Shotgun
* Tracking
* Barrier
* Shield
* Swarm

## random-vanilla

* Speed
* Wallhack
* Bounce
* Multishot
* Triple
* Homing
* Invincible
* Big
* MegaDeath
* Grenade
* Fire
* Blast
* Banana
* Shotgun
* Barrier
* Swarm

## old

* Speed
* Wallhack
* Bounce
* Multishot
* Triple
* Homing
* Invincible
* Big
* Grenade
* Fire
* Blast
* Banana
* Godmode
* Mines
* Shotgun
* Tracking
* Barrier
* Old Bounce
* Old Big
* Old Mines

## random-old

* Speed
* Multishot
* Triple
* Homing
* Invincible
* Grenade
* Fire
* Blast
* Banana
* Shotgun
* Tracking
* Barrier
* Old Bounce
* Old Big
* Old Mines

## supermix

* Speed
* Bounce
* Multishot
* Triple
* Homing
* Grenade
* Fire
* Banana

## supermix-vanilla

(the one Godmode uses)

* Speed
* Bounce
* Multishot
* Triple
* Homing
* Grenade
* Fire
* Banana

## ultimate

* Wallhack
* Godmode

## ultimate-vanilla

* Wallhack
* Godmode

## random

* Speed
* Wallhack
* Bounce
* Multishot
* Triple
* Homing
* Invincible
* Big
* MegaDeath
* Grenade
* Fire
* Blast
* Banana
* Shotgun
* Barrier
* Swarm

## dev

* Long Invincible
* Colorless Long Invincible
* The Super
* Inversion
* Annoying
* Ultra Bounce
* Color Changing
* Other Stuff is Poison
* Backwards Movement
* Wall Sparks
* Edge Sparks
* Weird Extra Cannons
* Triple Spread
* Ring Shooter
* Ancient MegaDeath
* Banana Split
* Trickster Circle
* Trickster Snake
* Pusher
* Movement Restarter
* Mine Layer

## random-dev

* Inversion
* Annoying
* Ultra Bounce
* Color Changing
* Other Stuff is Poison
* Backwards Movement
* Wall Sparks
* Edge Sparks
* Weird Extra Cannons
* Triple Spread
* Ring Shooter
* Ancient MegaDeath
* Banana Split
* Trickster Circle
* Trickster Snake
* Pusher
* Movement Restarter
* Mine Layer
