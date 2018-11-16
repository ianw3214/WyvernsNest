# Engine Basics

## States and Entities

The **State** class is the high level state logic should be handled. Every game state should inherit the base State class, and the initial State of the game is passed to the Engine upon initialization. Below is an example of a game state:

```c++
class Combat : public State {

public:

    Combat();
    ~Combat();

    void handleEvent(const SDL_Event& e);   // Handles incoming events
    void update(int delta);                 // Handles state logic
    void render();                          // Handles entity rendering

private:
    // Other state specific variables/functions here
};
```

The base State class also stores a list of **Entities**. The **handleEvent**, **update**, and **render** functions will all call their corresponding functions over each if not overriden. Below is the interface for the entity class:

```c++
class Entity {

public:

    Entity();
    virtual ~Entity();

    virtual void handleEvent(const SDL_Event& event);   // Handles incoming events
    virtual void update(int delta);                     // Handles entity logic
    virtual void render();                              // Handles entity rendering

    void remove();                                      // Marks the entity for removing
    bool shouldRemove() const;                          // Getter for if the entity is removed
};
```

## Sprites and Rendering

The **Sprite** class represents an image file to be rendered as a Sprite. The interface should explain most of how the sprite class works:

```c++
class Sprite {

public:

    // Initialize sprites with a path to it's image file
    Sprite(const std::string& path);
    ~Sprite();

    // The render function to actually display the sprite on screen
    void render();

    // Sprite position
    int x, y;
    // Sprite size
    int w, h;

    // helper methods to make life easier
    void setPos(int x, int y);
    void setSize(int w, int h);
};
```

There are also primitive functions defined in the Renderer class. Most of the functions that are needed to make the game work have been wrapped by a function in the **Core** namespace, which includes accessors to the renderer. For example, a draw call to render a line might look like this:

```c++
#include "engine/core.hpp"
void exampleDraw() {
    Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.f, 0.f, 0.f));
}
```

Other available function calls can be found in the [core.hpp header file](https://github.com/ianw3214/WyvernsNest/blob/master/src/engine/core.hpp).

## Handling Events

There is no real wrapper around the SDL events, so the SDL event interface will be used to handle user events. The documentation for SDL events can be found [here](https://wiki.libsdl.org/SDL_Event). Below is an example of handling a key input event:

```c++
void State::update(const SDL_Event& event) {
    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_A) {
            std::cout << "'A' key pressed!" << std::endl;
        }
    }
}
```


This project uses the [FreeType text rendering library](https://www.freetype.org/).