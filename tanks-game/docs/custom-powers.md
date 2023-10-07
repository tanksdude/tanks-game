# Custom Power Documentation

## Getting started

In order to make custom powers, all you need is a text editor. I used Notepad out of laziness. Here's the steps you'll need to take to make a custom power:

1. Follow the same steps for making custom levels (see `custom-levels.md`), but add a `powers` in your mod's folder instead.
1. Yeah, that's it.

And that's all you need. Welcome to the world of custom powers! Unfortunately, custom powers are far more limited than real powers, but at least it's something.

Some general notes:

* Whitespace between arguments doesn't matter (as long as there is at least one space).
* Every line can only have one assignment or command. If you have a really long line, sorry, but I recommend turning on word wrap.
* Sorry, but there's no math parsing when supplying numbers. You can't do something like `(16+8)/2`; you must do `12` instead.

## Initialization stuff

Required: every power needs a name, color, types, and weights for the types.

* `Name` (string): The name of the power.
* `Color` (3 floats in range [0,1] for RGB): The power's color. (Powerups are only differentiated by their color, sorry.)
* `PowerTypes` (at least one string): The "types" or "playlists" the power is in. Recommended "modname" and "random-modname". 
* `PowerWeights` (1 float for each type): The weighting in each playlist when randomly choosing from the playlist. Recommended 1.0 as the base weight. (Custom levels can't do anything with this yet though...)

Optional: the duration and some attributes.

* `PowerTankDuration` (double, default = 500): The duration the tank has the power. Bullets always have unlimited power duration.
* `PowerAttributes` (strings, default = "stack" and "mix"): Just something to help when randomizing powers. (Which isn't possible for custom levels yet...) Currently only "stack" and "mix" are even acknowledged as existing.

After you have set the required and optional attributes, you need to write `[TANKPOWER_START]` to begin commands for the tank power, then after that, write `[BULLETPOWER_START]` to begin commands for the bullet power.

## Command list (tank)

### Additional shooting

First, do `additionalShooting_Enable` to enable this feature.

`additionalShooting_AngleRelativeToTank <angle in degrees>`: When shooting, this is the angle variance (from the cannon) that the bullets can fly out at. For example, Fire and Blast set this.

`additionalShooting_AngleRelativeToCannon <angle in degrees>`: When shooting, this is the angle variance that can be applied from the edge of the tank. No vanilla power does this.

`additionalShooting_BulletCount <number>`: The number of bullets that will be fired out. For example, Fire sets this to 4 and Blast sets this to 16.

### Add shooting points

First, do `addShootingPoints_Enable` to enable this feature. This controls how many cannons are tacked on to the tank. For example, Multishot adds seven to fire out 8 bullets.

`addShootingPoints_EquallySpaced <number>`: If you want the cannons to be equally spaced out, use this. For example, Multishot sets this to 8 to fire out eight bullets at once (and no other vanilla power sets this).

`addShootingPoints_Raw <angle 1> [<angle n>]`: If you want to manually add the cannons, use this. Numbers in range (0,360) only. (Not recommended.) Only used by Weird Extra Cannons power, which was made just to test this.

### Add extra shooting points

First, do `addExtraShootingPoints_Enable` to enable this feature. This controls how many extra cannons are tacked on to the tank. For example, Triple adds two to fire out 3 bullets.

`addExtraShootingPoints_Raw <angle 1> <angle 2> [<angle n1> <angle n2>]`: Adds the extra cannons. Numbers in range (0,360) only. Used by Triple and Shotgun.

### Modified edge collision

First, do `modifiedCollisionWithEdge_Enable` to enable this feature.

`modifiedCollisionWithEdge_AdditionalBoundary <distance>`: This sets how far a tank can go out of bounds. The only power that sets this is Annoying (and was the reason this feature exists!).

### Modified death handling

First, do `modifiedDeathHandling_Enable` to enable this feature.

`modifiedDeathHandling_DurationSubtract <amount>`: The amount of "life force" to subtract when taking a hit. "Life force" in this case is just the time left on the power. For example, Shield sets this to 50, so it can theoretically save the tank from 10 hits. Barrier sets this to 500, so it can only prevent one death (actually it *always* prevents one death regardless of how much time is left but that's not the point).

### Tank-specific changes

* `tankMaxSpeedMultiplier <number>`: Multiplies the tank's max speed by this number. Does not affect acceleration. For example, Speed uses this. Add `tankMaxSpeedStacks` to have this multiplier stack.
* `tankAccelerationMultiplier <number>`: Multiplies the tank's acceleration by this number. Does not affect max speed. Add `tankAccelerationStacks` to have this multiplier stack.
* `tankRadiusMultiplier <number>`: Multiplies the tank's radius by this number. Only used by Old Bounce (just because it wanted to be different). Add `tankRadiusStacks` to have this multiplier stack.
* `tankFiringRateMultiplier <number>`: Multiplies the tank's shooting cooldown by this number. For example, Big sets this to 4 while Multishot sets this to 0.5. Add `tankFiringRateStacks` to have this multiplier stack.
* `tankTurningIncrementMultiplier <number>`: Multiplies the tank's turning increment (turning speed) by this number. For example, Fire sets this to 2, and Invert sets this to -1 (and stacks). Add `tankTurningIncrementStacks` to have this multiplier stack.

Note on "stacking": When collecting two Speed powerups, the tank is 4× as fast. However, when collecting two Invincible powerups with a .5× max speed multiplier, the tank's max speed is still .5× its normal value. "Stacking" really means to force the multiplier to always apply. When deciding between many different multipliers, the largest and smallest ones are both used (by multiplying them together).

### Offense and defense

`offenseTier <number>`: The offense value of the tank. This is not used by any vanilla tank powerup.

`defenseTier <number>`: The defense value of the tank. For example, Invincible sets this to 2.5.

If you want your power to be more important than other powers and *override* the other powers' offense and defense, there is an option for that. `offenseImportance <number>` and `defenseImportance <number>` will decide how "important" your power is. No vanilla power sets this for tank powers (though Annoying sets this for bullet powers).

Check `list-of-powers.md` to find out what powers have what offense and defense. The important ones to look at are Invincible and Big, since the entire offense and defense system is built around them.

## Command list (bullet)

### Bounce count

`modifiedCollision_BounceCount <number>`: This sets the number of bounces the bullet can perform. This number is shared between wall and edge bounces. Does nothing if wall bouncing and edge bouncing aren't enabled. For example, Bounce sets this to 16.

### Modified edge collision

Do `modifiedCollisionWithEdge_EnableBounce` to enable bouncing off the edge.

### Modified wall collision

Do `modifiedCollisionWithWall_EnableBounce` to enable bouncing off a wall.

Do `modifiedCollisionWithWall_DestroyWall` to enable destroying the wall. Yes, you can enable bouncing and destroy the wall. (In regular gameplay, this can be accomplished by getting Bounce *then* Big.)

### Modified death handling

Do `modifiedDeathHandling_Enable` to give the bullet one extra hit. For example, Barrier and Shield set this.

### Bullet-specific changes

* `bulletSpeedMultiplier <min number> [<max number>]`: Multiplies the bullet's initial speed by this number, or a number in the range. For example, Homing sets this to 0.5. No vanilla power sets this as a range, except Banana when banana-ing. Add `bulletSpeedStacks` to have this multiplier stack.
* `bulletRadiusMultiplier <min number> [<max number>]`: Multiplies the bullet's initial radius by this number, or a number in the range. For example, Big sets this to 4. No vanilla power sets this as a range. Add `bulletRadiusStacks` to have this multiplier stack.
* `bulletDeceleration <min number> [<max number>]`: Sets the bullet's deceleration to this number, or a number in the range. For example, Banana sets this to 0.125, and Fire and Blast set this as a range.
* `bulletAccelerationImportance <number>`: Sets the bullet's acceleration "importance" to this number. Mines sets this to 0.5 and is the only vanilla power to set this.
* `bulletDeceleration_DegradeAmount <number>`: The amount of "life force" the bullet loses per tick when it has zero velocity, which is the transparency (but transparency is expensive to draw so a circular cooldown is used instead). **Make sure to set this if you set `bulletDeceleration`!** For example, Grenade sets this to 0.875.

See the tank power section "Tank-specific changes" for a note on what "stacking" means.

### Offense and defense

`offenseTier <number>`: The offense value of the bullet. For example, Big sets this to 2.

`defenseTier <number>`: The defense value of the bullet. For example, Invincible sets this to 1. (Note that Invincible defense on bullet and tank are different, because Invincible bullets need to die to Big bullets, but Invincible tanks need to live to Big bullets.)

If you want your power to be more important than other powers and *override* the other powers' offense and defense, there is an option for that. `offenseImportance <number>` and `defenseImportance <number>` will decide how "important" your power is. Only Annoying sets this, and only for `offenseImportance`.

Check `list-of-powers.md` to find out what powers have what offense and defense. The important ones to look at are Invincible and Big, since the entire offense and defense system is built around them.

