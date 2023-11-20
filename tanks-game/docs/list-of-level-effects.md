# List of Every Level Effect

## Respawning Powerups

* Types: "vanilla", "random-vanilla", "random"
* Internal name: `respawning_powerups`

A very useful effect that allows powerups to respawn after some time.

## Magnetism

* Types: "vanilla", "random-vanilla"
* Internal name: `magnetism`

Powerups may be pulled towards tanks.

Unfortunately, the "magnetism" on the powerups does not persist if they respawn through Respawning Powerups.

## Wind

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `wind`

Covers the level in "wind" that pushes tanks and bullets, cycling between on and off states.

## Minefield

* Types: "vanilla", "random-vanilla", "old", "random-old"
* Internal name: `minefield`

Occasionally creates some hazardous "mines" to be avoided.

## Ice

* Types: "vanilla-extra", "old"
* Internal name: `ice`
* Tank modifiers: 1/8 acceleration

Makes the level very "slippery" by significantly reducing tank acceleration. (But acceleration is a scalar, not a vector, so it feels more slow than slippery.)

## Invisible Walls

* Types: "vanilla-extra", "old"
* Internal name: `invisible_walls`

Turns all the walls invisible. (By changing their color to be the background's color.)

Pro tip: since bullets die when hitting walls, you can shoot to see where walls are. No one has ever figured this out by themself so I still have work to do as a level designer.

## [DEV] Symmetric Tanks

* Types: "dev"
* Internal name: `symmetric_tanks`

Forces the tanks to have symmetrical positions. Don't remember why I made it, but probably just to test if I could.

## [DEV] Tiny Tanks

* Types: "dev", "random-dev"
* Internal name: `tiny_tanks`
* Tank modifiers: 1/4 radius

Makes the tanks tiny!

# Quick list for types

## vanilla

* Respawning Powerups
* Magnetism
* Wind
* Minefield

## vanilla-extra

* Ice
* Invisible Walls

## random-vanilla

* Respawning Powerups
* Magnetism
* Wind
* Minefield

## old

* Wind
* Minefield
* Ice
* Invisible Walls

## random-old

* Wind
* Minefield

## random

* Wind
* Respawning Powerups

## dev

* Symmetric Tanks
* Tiny Tanks

## random-dev

* Tiny Tanks
