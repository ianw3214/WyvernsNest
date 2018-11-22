# Creating Enemy AI

## Setup

Currently, each enemy will be represented by different classes that override a base *Enemy* classes' behaviour. The interface for the base enemy class looks like this:

```c++
class Enemy : public Unit {

public:

    Enemy();
    ~Enemy();

    // Functions that CAN be overriden to customize functionality
    // These two functions should not be touched for more basic enemies
    virtual void render();
    virtual void update(int delta);

    // The accessor method for the combat state to call an enemy to take its turn
    void takeTurn();

private:

    // OVERRIDE this function to customize enemy movement AI
    // Helper method to handle the movement portion of an enemy turn
    virtual void handleMovement();
    // OVERRIDE this function to customize enemy attack AI
    // Helper method to handle the attack portion of an enemy turn
    virtual void handleAttack();

    // Enemy sprite
    Sprite sprite;

};
```

To create a custom enemy, the **sprite** needs to be changed to the desired enemy sprite, and the **handleMovement** and **handleAttack** functions need to be overriden to achieve desired functionality.

***

## Customizing Enemy movement

The base unit class provides a **move** function that helps with unit movement:  

```c++
bool move(Vec2<int> pos);`  
```

The function takes in a reference to the combat state in order to determine a valid path, as well as the target destination. It then sets the **path** variable of the unit to reflect the new path that the unit should be taking to reach the destination.  
The return value of the function is a *boolean* indicating whether a valid path was found or not.  

The combat state has some utility methods to help the unit determine its movement. For example, if the enemy needs to know about player positions in order to determine its' movement, then the combat state has a **getPlayers** function to help easily get the player positions. The function signature looks like this:

```c++
std::vector<Player*> getPlayers() const;
```

If you want to test if a path works without moving the enemy, the base unit class provides a **getPath** method that returns a valid path to the destination if it exists.

```c++
std::vector<ScreenCoord> getPath(ScreenCoord to);
```

***

## Customizing Enemy attacks

First, to add enemy attacks, they need to be stored in the enemy class somewhere. They can simply be added as a class property like so:

```c++
class Enemy : public Unit {

public:
    // ...
private:
    // ...

    Attack example_attack1;
    Attack example_attack2;

};
```

Attacks can be initialized programmatically, but ideally they are stored in the [attacks data JSON file](../res/data/attacks.json) and acquired statically via the attack loader. More documentation about how attacks can be create can be found in the [attacks documentation page](attacks.md).

To execute an attack, replace the **EXAMPLE_ATTACK** and **ATTACK_COORDINATE** variables in the following block of code:

```c++
EXAMPLE_ATTACK.attack(ATTACK_COORDINATE, *combat);
state = UnitState::ATTACK;
startCounter();
```

The base unit update function will take care of the common logistics, including timing, animations, etc...

If the attack coordinates need to be calculated according to some sort of the combat state, the helper methods in the combat state could be useful just like in the handleMovement function.

***

## EXAMPLE AI

Below is an example of an implementation of enemy AI containing logic for a basic enemy with random movement and a basic melee attack. I have included the constructor as well as the handleMovement and handleAttack methods since those are realistically the only functions that need to be changed for new enemy AIs.

```c++
// Example enemy constructor
Enemy::Enemy() :
    // Initialize class variables
    Unit(UnitType::ENEMY),
    // Sprite initialization
    sprite("res/assets/enemies/WyrmSprite.png"),
    // Attack initialization via static attack loader
    bite(Attacks::get("PUNCH", this))
{
    // This code is necessary for sprites to display correctly
    sprite.setSize(sprite_width, sprite_height);
}
```

```c++
// Example enemy movement AI
void Enemy::handleMovement() {
    // Try to find a valid location to move to
    int x_offset;
    int y_offset;
    int tries = 20;
    while (tries > 0) {
        x_offset = rand() % (getMoveSpeed() * 2 + 1) - getMoveSpeed();
        y_offset = getMoveSpeed() - std::abs(x_offset);
        // If we can find a path to the target location, then move to that location
        if (getPath(position - Vec2<int>(x_offset, y_offset)).size() > 0) {
            break;
        }
        tries--;
    }
    // If the movement fails, then directly execute the attack state
    // NOTE: if movement succeeds, then the handleAttack function doesn't need to be explicitly called
    if (!move(position - Vec2<int>(x_offset, y_offset))) {
        // Directly handle the attacks if no movement could be done
        handleAttack();
    }
}
```

```c++
// Example enemy attack AI
void Enemy::handleAttack() {
    // If there is a player adjacent to the enemy, attack the player
    if (combat->getUnitAt(position - Vec2<int>(1, 0))) {
        bite.attack(position - Vec2<int>(1, 0), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(0, 1))) {
        bite.attack(position - Vec2<int>(0, 1), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(-1, 0))) {
        bite.attack(position - Vec2<int>(-1, 0), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    if (combat->getUnitAt(position - Vec2<int>(0, -1))) {
        bite.attack(position - Vec2<int>(0, -1), *combat);
        state = UnitState::ATTACK;
        startCounter();
        return;
    }
    // If no attacks could be done, set the unit to be at done state
    state = UnitState::DONE;
}
```

To test the enemy AI, you can instantiate an enemy in the combat state constructor via hard code for now. The usage is as so:

```c++
Combat::Combat() :
    current(nullptr)
{
    // ...

    // The first argument is a pointer to the newly created enemy
    // The last two arguments are the initial position of the enemy
    addEnemy(new YOUR_ENEMY_CLASS(), 4, 4);

    // ...
}
```