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
    ]
}
```

Since the attack types and effects are incomplete, there is no documentation for what string corresponds to what in the game. Thus, the best solution for now is to look at the source code for the [**Attack Loader**](https://github.com/ianw3214/WyvernsNest/blob/master/src/game/util/attackloader.cpp) and see which strings will result in what outputs.

Also, although *effects* are stored as an array in the JSON file, only one effect is stored in the attack in the game currently, so only the first effect in the array of effects will have an effect in game.

## Implementing effects

To create a new effect, create a new class that overrides the base AttackEffect class:

```c++
class AttackEffect {
public:
    virtual void attack(ScreenCoord pos, Combat& combat, int stat, double mult) = 0;
};
```

The attack method is what is called when the attack is actually executed. The **pos** variable determines the tile that the effect is being executed on, the **combat** is a reference to the current combat state in case any utility methods are needed from there.  
The **stat** and **mult** variables determine the modifiers for the attack. They will most likely be changed in the future, so it would be best to just copy what the other effects have done with these variables for now.

Below is an example implementation of a damage effect:

```c++
// Attack effect that damages units
class DamageEffect : public AttackEffect {
public:
    DamageEffect() : damage(1) {}
    DamageEffect(int damage) : damage(damage) {}
    void attack(ScreenCoord pos, Combat& combat, int stat, double mult);
    int damage;

private:
};

// The implementation of the attack callback
void DamageEffect::attack(ScreenCoord pos, Combat & combat, int stat, double mult) {
    // Damage the unit if it exists at the position
    Unit * unit = combat.getUnitAt(pos);
    if (unit) {
        unit->takeDamage(static_cast<int>(damage + stat * mult));
    }
}

```