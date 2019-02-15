# Building the project

## Requirements

You need the following to successfully build the project on your machine:

- A C++ compiler
- A package manager (vcpkg on windows, brew on mac)
- CMake

## Installing packages

### Windows

First, download vcpkg from [here] and find that directory using the command line.  
Hook up user wide integration via the following command:  
> .\vcpkg integrate install

Then, install the necessary libraries using the following commands:

> .\vcpkg install sdl2
> .\vcpkg install sdl2-mixer
> .\vcpkg install glew
> .\vcpkg install freetype

### Mac

On mac, use [homebrew](https://brew.sh/) to get the needed library files.  
Run the following commands to get SDL, GLEW, and FreeType setup on your local machine.

> brew install sdl2  
> brew install sdl2_mixer  
> brew install glew  
> brew install freetype  

## Running cmake

For this project, it is recommended to use an **out of source** build so that the build files don't interfere with the source files.  
To do so, first make a directory called **build**, and *cd* into it on the command line.  
Then, run the following command to generate the project:  
> cmake ..

Alternatively, you can use the included script files to generate the cmake build, just run the following command on windows:
> setup.bat

## Working with the project

The project should be all set up now. On windows, simply open the .vcxproj file to open the project in Visual Studio.

**IMPORTANT NOTE**: In order to run the game from within the build folder, you need to copy the res folder from the root folder to the build folder. This step is done automatically if the previous *setup.bat* script was used to generate the build.