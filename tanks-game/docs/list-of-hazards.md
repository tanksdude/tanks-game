# List of Every Hazard

## Stationary Turret

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `stationary_turret`
* Construction type: angle
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

A tiny turret that points in a fixed direction. It charges up, "locks in," shoots, then cools down.

## Targeting Turret

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `targeting_turret`
* Construction type: angle
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

A small turret that can turn towards its target. It charges up, "locks in," shoots, then cools down. It can turn while charging up, but the time between charging up and "locking in" does not allow turning. Its target is the closer tank it can see, though if the other tank suddenly becomes closer, it will not change its target until it has fired.

## Patrolling Turret

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `patrolling_turret`
* Construction type: complex
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

A Targeting Turret that can move around! It follows its path, briefly stopping at set locations. Isn't it cool?

## Mother Turret

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `mother_turret`
* Construction type: angle
* Collision type: solid
* Defense: 2.5 (`DESTRUCTION_TIER`+0.5)

A large turret that manages making more turrets rather than shooting at enemies. It charges up, "locks in," makes a child, then cools down. Its "target" is the furthest open spot (the child locations are fixed). Default 8 children.

## Ginormous Turret

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `ginormous_turret`
* Construction type: angle
* Collision type: solid
* Defense: 3.0 (`DESTRUCTION_TIER`+1.0)

A ginormous turret that commands its turrets rather than shooting at enemies. If it sees a tank, it commands its children to focus on the target, regardless of whether they can see it. Its "making children" logic is the same as a Mother Turret. If it suddenly sees a tank while charging up (but not after), it will throw away all charging up progress and aim at the tank.

The main reason this was made was because I thought it would be funny to have a *really* big turret. And it was. However, it would be funnier to have an even *bigger* turret, which is not feasible with the current game.

### Minion Turret (inaccessible)

* Types: "vanilla", "dev"
* Internal name: `minion_turret`

Note that this hazard is inaccessible. It is the turret that Ginormous Turrets create. Mother Turrets create Targeting Turrets that do their own thing, but Ginormous Turrets force their turrets to follow their commands. As such, the only difference between a Minion Turret and a Targeting Turret is that a Minion Turret can *only* follow orders from its Ginormous Turret.

If the Minion Turret's parent is destroyed, it does nothing. Well, it won't receive the order to stop targeting, so it will keep tracking the target tank (even through walls) until it has fired. Actually, they can keep tracking through walls if the parent suddenly needs to start making more children, since they don't receive a "stop targeting" order; maybe they should automatically stop if their parent can't see the tank?

## Lightning

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `lightning`
* Construction type: radius (circular), standard (rectangular)
* Collision type: under
* Offense: 0.5
* Defense: 999 (`HIGH_TIER`)

After charging up for a while, it turns into a field of death for anything that dares enter! But most importantly, it looks frickin' cool when something invincible goes in and the lightning bolts follow it.

### Horizontal and Vertical Lightnings (rectangular)

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `horizontal_lightning` and `vertical_lightning`
* Construction type: standard; however, they must be attached to walls (or an edge)

A variant of the rectangular Lightning where its ends need to be attached to two walls. These are the main versions you'll see. If they lose their walls, they immediately stop functioning (although if they gain their walls back, they can resume operation).

## Lava

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `lava`
* Construction type: radius (circular), standard (rectangular)
* Collision type: under
* Offense: 0.5
* Defense: 999 (`HIGH_TIER`)

It's lava. If a tank hits it, it dies. Bullet can go over it no problem, unless they are stationary.

Occasionally, some bubbles may appear, though they are visual-only.

## No Bullet Zone

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `no_bullet_zone`
* Construction type: radius (circular), standard (rectangular)
* Collision type: under
* Offense: 999 (`HIGH_TIER`)
* Defense: 999 (`HIGH_TIER`)

Bullets are unable to even graze this hazard. However, tanks handle it just fine.

## Spiral Lava

* Types: "vanilla-extra", "random-vanilla", "random"
* Internal name: `spiral_lava`
* Construction type: standard square
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

Lava spirals out from the center! Don't get mesmerized, since the lava is made of Lava!

## Gravity Well

* Types: "vanilla-extra", "random-vanilla", "random"
* Internal name: `gravity_well`
* Construction type: radius
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

Pulls in tanks and bullets, as you would expect from gravity. (Doesn't change angle though, so it's not "real" gravity.)

## [DEV] "Wall"

* Types: "dev", "random-dev"
* Internal name: `wall`
* Construction type: standard
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

It's just a wall, except bullets can't bounce off it and turrets don't see it.

It was made because I realized I had no "solid" rectangular hazards. Just a test to make sure that collision type works.

## [DEV] The Reflecktor

* Types: "dev", "random-dev"
* Internal name: `reflecktor`
* Construction type: standard
* Collision type: solid
* Defense: 2 (`DESTRUCTION_TIER`)

It's a wall that reflects tanks that hit it. Pretty amusing, right?

Why the strange name? It's funnier. That's it. Wanted to make this for long time but never got around to it, probably because it is so simple and there are plenty of better things to make.

## Cloud

* Types: "dev", "random-dev"
* Internal name: `cloud`
* Construction type: complex
* Collision type: under (though "over" makes more sense but that doesn't exist)
* Offense: 0
* Defense: 999 (`HIGH_TIER`)

Occasionally spits out some powerups.

Currently a dev hazard because I haven't finished its drawing function, and it's rather underwhelming.

# Quick list for types

## vanilla

* Stationary Turret
* Targeting Turret
* Patrolling Turret
* Mother Turret
* Ginormous Turret
* Lightning
* Horizontal Lightning
* Vertical Lightning
* Lava
* No Bullet Zone
* Minion Turret (inaccessible)

## vanilla-extra

* Spiral Lava
* Gravity Well

## random-vanilla

* Stationary Turret
* Targeting Turret
* Patrolling Turret
* Mother Turret
* Ginormous Turret
* Lightning
* Horizontal Lightning
* Vertical Lightning
* Lava
* No Bullet Zone
* Spiral Lava
* Gravity Well

## old

* Stationary Turret
* Targeting Turret
* Lightning
* Horizontal Lightning
* Vertical Lightning
* Lava
* No Bullet Zone

## random-old

* Stationary Turret
* Targeting Turret
* Lightning
* Horizontal Lightning
* Vertical Lightning
* Lava
* No Bullet Zone

## random

* Stationary Turret
* Targeting Turret
* Patrolling Turret
* Mother Turret
* Ginormous Turret
* Lightning
* Horizontal Lightning
* Vertical Lightning
* Lava
* No Bullet Zone
* Spiral Lava
* Gravity Well

## dev

* Wall
* The Reflecktor
* Cloud
* Minion Turret (inaccessible)

## random-dev

* Wall
* The Reflecktor
* Cloud
