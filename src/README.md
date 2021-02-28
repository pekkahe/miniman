# Source structure

#### `\assets`
Graphics, sounds and other assets used by the game. The source assets are just regular `txt` or `zip` files that are renamed to look like something else.

#### `\bin`  
Game binary output directory. 

At the moment, there is no post-compilation step that adds the external dependencies to the output directory for VC++ debugging. 
The required SFML and PhysicsFS binaries have been added to this directory for convenience.

If you encounter errors while trying to run the game in Visual Studio, please ensure the proper DLLs are in the corresponding build directories: 
- SFML debug binaries (`...-d-2.dll`) for Debug compilation, 
- and release binaries (`...-2.dll`) for Release compilation.

#### `\external`  
External dependencies required by the game:
- [SFML](http://www.sfml-dev.org/) by Laurent Gomila <laurent.gom@gmail.com>
- [PhysicsFS](http://icculus.org/physfs/) by Ryan C. Gordon and others <icculus@icculus.org>
						
#### `\Miniman`  
The Visual C++ project source directory.
