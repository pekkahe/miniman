# Miniman

Copyright (c) 2014 by Pekka Hellsten.

This is the Visual C++ source for Miniman, a Pac-Man clone I made with SFML. 

COMPILING THE BINARIES

The game was developed and built with Visual C++ 12 (2013) using the C++ 11 threads library,
so unfortunately it will not compile on Visual C++ 10 (2010). If you are using Visual C++ 11
(2012) you must change the platform toolset to "v110" or "v110_xp" from the project properties:

	Configuration Properties -> General -> Platform Toolset -> Select either "v110" or "V110_xp"

This should allow you to compile and run the game, but for some reason I lost audio when testing
on Visual C++ 11. Please contact me if you encounter the same issue and would like me to look into it.


SOURCE DIRECTORIES

\assets			
		Graphics, sounds and other assets used by the game. The game uses the root packages,
		which are just regular txt or zip files renamed to look like something else.

\bin
		Game binaries are compiled here. I've already added the required SFML and PhysicsFS
		binaries here, so you can run the game straight from Visual Studio.

\external
		External binaries, libraries and include files required to compile and run the game:
		- SFML (http://www.sfml-dev.org/) by Laurent Gomila <laurent.gom@gmail.com>
		- PhysicsFS (http://icculus.org/physfs/) by Ryan C. Gordon and others <icculus@icculus.org>
					
		If you encounter errors while trying to run the game in Visual Studio, please ensure
		the proper DLLs are in the corresponding build directories: SFML debug binaries (...-d-2.dll)
		for Debug compilation and release binaries (...-2.dll) for Release compilation.
	
\Miniman
		The Visual C++ project folder with source code.


		
- Pekka (pekka.hellsten@gmail.com)
www.pekkahellsten.com
