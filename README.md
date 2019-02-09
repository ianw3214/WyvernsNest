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
    * base fork should be ianw3214/WyvernsNest, and base should be dev
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

Download [this zip file](https://drive.google.com/open?id=1Bl_tMIT3d_DK8HydmD-6zahEXXGmK_XR) and extract the lib folder into the same directory as your solution (*The libs folder should be at the same level as the .sln file*)

Finally, move all of the **dll** files from the libs folder into the root folder. (*The root folder is the one that contains the .sln file*)

Ensure Visual Studio's build configuration is set to x86(32-bit) by selecting Build->Configuration Manager...->Active solution platform:->x86.

You should be able to build and run the project now.

### Mac setup

On mac, use [homebrew](https://brew.sh/) to get the needed library files.  
Run the following commands to get SDL and GLEW setup on your local machine.

> brew install sdl2  
> brew install glew

Once they are installed, the project should be able to compile just by typing **make** from the root directory of the project. You would still need to download the zip file containing the *libs* folder from the Windows step, since it contains the *stb_image* header.

## Engine Basics

See the [documentation page](docs/basics.md) for information on how to use the engine.

## Creating attacks

See the [attacks documentation page](docs/attacks.md) for more information on creating attacks.

## Creating enemy AI

See the [enemy AI documentation page](docs/enemyAI.md) for information on creating enemy AI.
