# Raiton: a shooting game for the Udacity C++ course Capstone Project

Michael Bennett (michael.bennett@iesve.com)

This is the repo for submission to the Capstone project of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). It is designed to meet rubric criteria found [here](https://review.udacity.com/#!/rubrics/5373/view).  The rubric addressed is also specified below.

The code for this repo has adopted code and assets from the following sources, listed below:
  - "Creating a 2D shoot 'em up" tutorial located here: https://www.parallelrealities.co.uk/tutorials/#bad.  This has been a geat resource for learning about how to code a shooting game.  If anyone asks, Udacity recommends this resource [here](https://learn.udacity.com/nanodegrees/nd213/parts/a0c73361-219d-4ff6-984c-e37478ad300c/lessons/b4a166f9-ae01-4afc-95ed-93a367b8e4e0/concepts/4b55f1cb-507e-4bd5-8147-ca036c03355f).
  - some images have been taken from https://opengameart.org/.  Credits to the original creators are further down in the readme.  To my understanding, this art is free, even for commercial use, as long as the original authors are credited.  Credits are below in this readme.
  - The Udacity starter repo for "SnakeGame" was used as a fundamental template to start with, but it has since been gutted beyond recognition and Raiton's code bears very little resemblance to it.  It seems only fair to credit it anyway [it's here](https://github.com/udacity/CppND-Capstone-Snake-Game)

## Features

* This is Raiton! A shooting game where the player is a blue space-ship and you shoot the other space-ships.  They are definitely evil, so no need to feel bad.  Have fun! Be sure to dodge the bullets because there is a lot of them!  You probably shouldn't collide with the enemy ships either because that will end you.
* There are two different enemy types and a boss!  The enemies have different shooting patterns.
* Controls:
    * arrow keys to move around; 8-way movement.
    * 'z' key to fire.  Just hold it down and the ship will keep firing.
    * 'Esc' to immediately quit out.  The game will also quit out after a few seconds if you die or if you kill the boss.
* Please don't try to resize or maximise the window.  Thank you!

## Notes regarding code:
- Spawn data for the game is stored in "stage1.txt", which contains data regarding the time (in seconds), x-coordinate (as a fraction of the screen width) and enemy type spawns.  If you want to change the enemy spawns, you can always do so by editing the file.
- Memory management is a bit redundant since the main resource, the texture for each entity, is just a reference to a container stored in the renderer.  However, smart pointers are implemented anyway for entities and moved over to the "stage" object.  Valgrind reports lots of leaks, but apparently the SDL library is very leaky so I'm honestly not 100% sure eveything is managed.  I haven't added any of my own "new" or "delete" commands and all of the textures are "deleted" when the renderer is deconstructed, so hopefully I've caught the most onerous memory leaks.
- Concurrency is added by using separate threads to implement "Out of bounds" checking and removal.  Normally any entities outside of the screen are declared out of bounds and are removed every frame.  However, Raiton runs an "out of bounds checker" function and an "out of bounds entity remover" function in separate threads when the stage starts.  Every 10 ms, the checker looks for out of bounds entities.  If at least one entity is out of bounds, it sends a message to the remover via a message queue to remove anything out of bounds.

## Credits

Some assets used in this project have been derived from opengameart.org.  To my understanding, this art is free, even for commercial use, as long as the original authors are credited.  Here is a list of all used assets with their sources:

* Background: planet_1_0.png by Luke.RUSTLTD: https://opengameart.org/content/4-large-planets by 
* Ships: shipsheetparts.PNG by Stephen Callener (Redshrike): https://opengameart.org/content/space-ship-building-bits-volume-1.
* Bullets: M484BulletCollection1.png by Master484: https://opengameart.org/content/bullet-collection-1-m484.  Author's website: https://m484games.ucoz.com/

I modified most of the assets, so they *will* look different from the original source.  Most of them are just different colours, sizes or have added transparency.

## Udacity C++ course Rubric

The following are the rubric points required for passing the Udacity C++ Capstone project.  Rubric points that are not attempted are marked *Not attempted*.

### README (All Rubric Points REQUIRED)
* A README with instructions is included with the project
* The README indicates the new features you added to the game
* The README includes information about each rubric point addressed

### Compiling and Testing (All Rubric Points REQUIRED)
* The submission must compile and run without errors on the Udacity project workspace.

### Loops, Functions, I/O - meet at least 2 criteria
* The project demonstrates an understanding of C++ functions and control structures.
* The project reads data from a file and process the data, or the program writes data to a file
* The project accepts user input and processes the input - *Not attempted*
* The project uses data structures and immutable variables

### Object Oriented Programming - meet at least 3 criteria
* One or more classes are added to the project with appropriate access specifiers for class members
* Class constructors utilize member initialization lists
* Classes abstract implementation details from their interfaces
* Overloaded functions allow the same function to operate on different parameters
* Classes follow an appropriate inheritance hierarchy with virtual and override functions
* Templates generalize functions or classes in the project

### Memory Management - meeting at least 3 criteria
* The project makes use of references in function declarations
* The project uses destructors appropriately
* The project uses scope / Resource Acquisition is Initialization (RAII) where appropriate
* The project follows the Rule of 5
* The project uses move semantics to move data instead of copying it, where possible
* The project uses smart pointers instead of raw pointers

### Concurrency - meet at least 2 criteria
* The project uses multithreading
* A promise and future is used in the project - *Not attempted*
* A mutex or lock is used in the project
* A condition variable is used in the project

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* SDL2_IMAGE >= 2.0
  * Instructions for installing extension libraries for SDL2 can be found [here](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Raiton`.
