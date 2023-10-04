# List of Every Level Effect

## Wind

* Types: "vanilla", "random-vanilla", "old", "random-old", "random"
* Internal name: `wind`

Covers the level in "wind" that pushes tanks and bullets, cycling between on and off states.

## Respawning Powerups

* Types: "vanilla", "random-vanilla"
* Internal name: `respawning_powerups`

A very useful effect that allows powerups to respawn after some time.

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

* Types: "vanilla", "old"
* Internal name: `invisible_walls`

Turns all the walls invisible. (By changing their color to be the background's color.)

Pro tip: since bullets die when hitting walls, you can shoot to see where walls are. No one has ever figured this out by themselves so I still have work to do as a level designer.

## [DEV] Symmetric Tanks

* Types: "dev", "random-dev"
* Internal name: `symmetric_tanks`

Forces the tanks to have symmetrical positions. Don't remember why I made it, but probably just to test if I could.

# Quick list for types

## vanilla

* Wind
* Respawning Powerups
* Minefield
* Invisible Walls

## vanilla-extra

* Ice

## random-vanilla

* Wind
* Respawning Powerups
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

## dev

* Symmetric Tanks

## random-dev

* Symmetric Tanks
