# More Games in C++2: Assignment 1

> Captain, the enemy have invaded our space and begun a number of simultaneous attacks focused
> on the Starbases we have across the sector. We believe they've deliberately done this to
> stretch our forces and reduce the strength of our response. WE must go to their defence, we
> can not let the people of our federation suffer! Unfortunately, much of the fleet is combating
> the enemy in different regions, for this mission you're own your own. Admiral out!

### It's **[ASGETrek](https://pixeljudge.com/articles/history-time-egatrek/)** time!


For this assignment you will need to recreate or re-imagine EGATrek using the ASGE engine and
modern C++. EGATrek is one of the best Bridge Commander experiences out there and it's your
job to capture the strategy and immersion of the original, whilst delivering a modern
fresh coat of technical and visual paint.  

![](https://www.ex-astris-scientia.org/reviews/games/egatrek-screen1.png)

Layout
------
* app
  * src: the game itself
  * data: used for assets and game deployment
  * resources: used for icons 

* extern
  * important build scripts and other submodules
  * to update the submodules
    * `git pull --recurse-submodules`
    * `git submodule update --remote --recursive`
  
* gamelib
  * include: header files belonging to your game library
  * src: source files belonging to your game library
  * **reusable game components go here i.e. dice, animated sprite, game panel, game state etc** 
   
   
## Deliverables
You will need to provide the following:

* A working version of the game in C++ and ASGE
* A short let's play video

A choice between one of the following:

* A developer diary
* A developer video diary
* TDD and GDD

Static Analysis
------
You may run the clang-tidy CI checks locally using the Clang-Tidy target. Select it from the drop-down and click the hammer. This assignment uses GitHub actions instead of Travis to perform the build checks. You can view them in the "Actions" tab.
You can ignore any errors relating to third-party libraries. 

GameData
------
In order to access your game data from the game, you need to ensure that the +GD target has been run. You need to run this when you've modified the contents of your Data folder. 
