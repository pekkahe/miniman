# Miniman

Miniman is a Pac-Man clone made with the [SFML](http://www.sfml-dev.org/) multimedia library. 

## Compiling the game

The game was developed and built with Visual C++ 12 (2013) using the C++ 11 threads library,
so unfortunately it will not compile on Visual C++ 10 (2010). If you are using Visual C++ 11
(2012) you must change the platform toolset to "v110" or "v110_xp" from the project properties:

> Configuration Properties -> General -> Platform Toolset -> Select either "v110" or "V110_xp"

This should allow you to compile and run the game, but for some reason I lost audio when testing
on Visual C++ 11. Please contact me if you encounter the same issue and would like me to look into it.
