# Wyverns Nest

A 2D turn-based fantasy strategy game

***

## Contributing

1. Fork the repository
    * This gives you a personal copy of the project to play with and add features to.
2. Clone your fork
    * You should be cloning something that looks like [your-name]/WyvernsNest
3. Setup the project
    * See below
4. Add your feature
    * Edit code, add a .png, etc.
    * Push your changes onto your fork.
    * Please make sure your code compiles. We do not have an auto-build and test system in place yet.
5. When done, create a pull request on the main project
    * Click Pull Requests -> New Pull Request -> compare across forks
    * base fork should be ianw3214/WyvernsNest, and base should be your sub-team branch
    * head fork should be [your-name]/WyvernsNest, and compare should be where you added your changes.
6. Submit your pull request
    * Keep an eye on it. Changes may be requested.
    * If changes are requested, commit and push them to your fork.
    * The pull request will automatically update.
    * A sub-team lead will accept and merge the code into the main repository.
    
* If you are collaborating with a friend on a feature, ask your friend to clone and push to your fork ([your-name]/WyvernsNext). You can add your friend as a collaborator to your fork in the repo settings.
* If you want to get your personal fork up to date with the main project, make a pull request on your personal branch, but this time, merge the main project into yours!

## Project setup

Make sure Visual Studio is installed first.

Clone the repository onto your machine.

Download [this zip file](https://drive.google.com/file/d/1hAXUjqF0_p8v5UdK9ET0I-kugZknO9Fi/view?usp=sharing) and extract the lib folder into the same directory as your solution (*The libs folder should be at the same level as the .sln file*)

Finally, move the two **dll** files from the libs folder into the root folder. (*The root folder should contain the .sln file, the libs folder, SDL2.dll and glew32.dll*)

Ensure Visual Studio's build configuration is set to x86(32-bit) by selecting Build->Configuration Manager...->Active solution platform:->x86.

You should be able to build and run the project now.

### Mac setup

On mac, use [homebrew](https://brew.sh/) to get the needed library files.  
Run the following commands to get SDL and GLEW setup on your local machine.

> brew install sdl2  
> brew install glew

Once they are installed, the project should be able to compile just by typing **make** from the root directory of the project. You would still need to download the zip file containing the *libs* folder from the Windows step, since it contains the *stb_image* header.

## Engine Basics

### States and Entities

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

### Sprites and Rendering

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

### Handling Events

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
