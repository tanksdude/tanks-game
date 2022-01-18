# PowerTanks Battle

The C++ upgrade of my [JavaScript game](https://uncreativeusername.neocities.org/tanks.html)

## Getting Started

### Prerequisites

* If compiling from source, you'll want Visual Studio (2019)
    * [pre-compiled executables](https://github.com/tanksdude/tanks-game/releases) are provided if that's not an option for you
* OpenGL 3.3 or later
    * Earlier versions probably work if you update the shaders
* 3GHz+ CPU recommended (2GHz is probably fine)
    * Faster CPU -> more bullets on screen
    * Single-core CPU is enough (multithreading *potentially* coming "soon")
* RAM requirements unknown, but 100MB is good
    * More bullets -> more RAM needed
* No audio requirements (unlikely to change in the future)
* **Windows-only** (feel free to try your luck with an emulator) and 64-bit-only

### Installing

Install Visual Studio (2019) and the GitHub extension (I don't know if that's needed), then clone this project and run x64 Release (or Debug if you want to) (also make sure you're on the solution, not project)

Pre-compiled executables are available under "Releases" in the [GitHub repository](https://github.com/tanksdude/tanks-game).

## Running the tests

Hahaha, nope.

Will need them eventually, though.

## Built With

* [Visual Studio](https://visualstudio.microsoft.com/) - C++ IDE from Microsoft
* [FreeGLUT](http://freeglut.sourceforge.net/) - OpenGL Utility Toolkit; an updated version of the original GLUT
* [GLEW](http://glew.sourceforge.net/) - OpenGL Extension Wrangler Library; for getting the latest OpenGL commands on Windows
* [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm) - OpenGL-happy matrix and vector math library

## Contributing

I plan on working alone on this project, possibly welcoming new stuff once the underlying code is done.

If you really want a change, you could submit an Issue on GitHub.

Alternatively, you can fork this repository and add change all you want.

## Versioning

Version numbers are subjective and 1.0.0 is "game finished." This is contrary to semantic versioning because I don't know what I'm doing all the time.

## License

This project is licensed under GNU General Public License v3.0

## Acknowledgments

* JS Tanks (made by me): [tanks.html](https://uncreativeusername.neocities.org/tanks.html)
    * C++ version has more stuff
* Many people across the Interwebs who made StackOverflow and other forum posts
* Lots of YouTube tutorials, GDC talks, and my CS professors providing assistance

## More acknowledgements

* [Super Smash Bros.](https://www.smashbros.com/en_US/index.html) for being a very fun game
    * (Smash Bros the *party* game, not the competitive fighting game; use items!)
* [The Cherno](https://www.youtube.com/user/TheChernoProject/videos) is very helpful for OpenGL
* [Solarian Programmer](https://solarianprogrammer.com/) has good starter code for hardware rendering
* [The Coding Train](https://www.youtube.com/user/shiffman/videos) is helpful, but wasn't *specifically* used for this project (probably was used for the JS game, but I don't remember)
* [Factorio](https://www.factorio.com/)'s Friday Fun Facts are amazing and made me enjoy low-level C++ optimizations
* [Nitronic Rush](http://nitronic-rush.com/) is the other game that made me want to learn C++ in the first place
* [N++](http://www.nplusplus.org/) for just being a good game (also the developers had some [good GDC talks](https://www.youtube.com/watch?v=VZ4xevskMCI) and have a [useful game development blog](https://www.metanetsoftware.com/technique/tutorialAbak.html))
* [Creeper World](https://knucklecracker.com/creeperworld4/cw4.php) for also just being a good game
