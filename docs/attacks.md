# Attacks

## Structure

There are a few key variables that influence how an attack works, they are as follows:

```c++
// The name of the attack
std::string name;
// The source unit of the attack
Unit * source;

// The type of the attack   -> Determines how valid attacks are calculated
AttackType type;
// The range of the attack  -> Determines range when applicable
AttackRange range;
// The effect of the attack -> Determines what happens when an attack is executed
AttackEffect * effect;
// The AoE of the attack    -> Determines the radius of an attack
AttackAoE aoe;
// Flag to determine if the attack affects the user itself
bool affect_self;
```

Although these are declarations from the actual code itself, most of the data will be stored in a data file and loaded into the game at run time. The file can be found [here](../res/data/attacks.json).

## Storing attacks in the file

The [attacks.json file](../res/data/attacks.json) stores an array of JSON objects. Each JSON object represents an attack in the game, and they should be uniquely identified by the *name* parameter. Here is an example attack:

```json
{
    "name" : "PUNCH",
    "type" : "melee",
    "range" : 0,
    "aoe" : 0,
    "affect_self": false,
    "effects": [
        {
            "type": "damage",
            "damage": 3
        }
    ],
    "modifiers": [
        {
            "type": "INT",
            "mod": 20.0
        }
    ]
}
```

Since the attack types and effects are incomplete, there is no documentation for what string corresponds to what in the game. Thus, the best solution for now is to look at the source code for the [**Attack Loader**](https://github.com/ianw3214/WyvernsNest/blob/master/src/game/util/attackloader.cpp) and see which strings will result in what outputs.

Also, although *effects* are stored as an array in the JSON file, only one effect is stored in the attack in the game currently, so only the first effect in the array of effects will have an effect in game.

The *modifiers* however is fully implemented in the game, so adding any amount of modifiers in the JSON file will reflect in game.

## Implementing effects

To create a new effect, create a new class that overrides the base AttackEffect class:

```c++
class AttackEffect {
public:
    virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) = 0;
};
```

The attack method is what is called when the attack is actually executed. The **pos** variable determines the tile that the effect is being executed on, the **combat** is a reference to the current combat state in case any utility methods are needed from there.

Modifiers calculations are done in the effect when the attack executes. The decision to calculate modifiers dynamically rather than when the effect is initialized to generalize effect modifiers in the future for when there might be combat modifiers that come and go.  
The effect modifiers can be accessed in the effect attack method from the *Attack reference* via the **attack.getEffectModifiers()** function call. The actual stat of the unit to the corresponding attack can be accessed by calling **attack.getSource()->getStat(modifier.stat)**.  
Some attacks don't use the modifiers, for example if an attack generates terrain or something like that. In that case, the effect modifiers can be ignored.

Below is an example implementation of a damage effect:

```c++
// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
    DamageEffect() : base_damage(1) {}
    DamageEffect(int damage) : base_damage(damage) {}
    virtual void attack(ScreenCoord pos, Combat& combat, const Attack& attack) override;

private:
    int base_damage;
};

void DamageEffect::attack(ScreenCoord pos, Combat & combat, const Attack& attack) {
    Unit * unit = combat.getUnitAt(pos);
    if (unit) {
        int damage = base_damage;
        for (EffectModifier modifier : attack.getEffectModifiers()) {
            damage += static_cast<int>(attack.getSource()->getStat(modifier.stat) * modifier.modifier);
        }
        unit->takeDamage(damage);
    }
}

```