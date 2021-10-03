# PowerTanks Battle

The C++ upgrade of my [JavaScript game](https://uncreativeusername.neocities.org/tanks.html)

## Getting Started

### Prerequisites

* If compiling from source, you'll want Visual Studio (2019)
    * pre-compiled executables are provided if that's not an option for you
* Graphics card probably required
* OpenGL 3.3 or later
    * 3.2 or earlier might work if you update the shaders, but don't count on it
* 3GHz+ CPU recommended (2GHz is probably fine)
	* Faster CPU -> more bullets on screen
    * Single-core CPU is enough (because there's no multithreading)
* RAM requirements unknown, but 100MB is good
* No audio needed (unlikely to change in the future)
* **Currently Windows-only** (as far as I know) and 64-bit-only

### Installing

Install Visual Studio (2019) and the GitHub extension (I don't know if that's needed), then clone this project and run x64 Release (or Debug if you want to) (also make sure you're on the solution, not project)

Pre-compiled executables are available under "Releases" in the [GitHub repository](https://github.com/tanksdude/tanks-game).

## Running the tests

Hahaha, nope.

Will need them eventually, though

## Built With

* [Visual Studio](https://visualstudio.microsoft.com/) - C++ IDE from Microsoft
* [FreeGLUT](http://freeglut.sourceforge.net/) - OpenGL Utility Toolkit; an updated version of the original GLUT
* [GLEW](http://glew.sourceforge.net/) - OpenGL Extension Wrangler Library; for getting the latest OpenGL commands on Windows
* [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm) - OpenGL-happy matrix and vector math library

## Contributing

I plan on working alone for the forseeable future, possibly welcoming new stuff once the underlying code is done.

If you really want a change, you could submit an Issue 

## Versioning

Version numbers are subjective and 1.0.0 is "game finished." This is contrary to semantic versioning because I don't know what I'm doing all the time.

## License

This project is licensed under GNU General Public License v3.0

## Acknowledgments

* JS game (made by me): [tanks.html](https://uncreativeusername.neocities.org/tanks.html)
    * C++ version is now at feature parity (actually, it has more stuff than JS Tanks)
* Many people across the Interwebs who made StackOverflow and other forum posts
* Lots of YouTube tutorials, GDC talks, and my CS professors providing assistance

## More acknowledgements

* [The Cherno](https://www.youtube.com/user/TheChernoProject/videos) is very helpful for OpenGL
* [Solarian Programmer](https://solarianprogrammer.com/) has good starter code for hardware rendering
* [The Coding Train](https://www.youtube.com/user/shiffman/videos) is helpful, but wasn't *specifically* used for this project (probably was used for the JS game, but I don't remember)
* [Factorio](https://www.factorio.com/)'s Friday Fun Facts are amazing and made me enjoy low-level C++ optimizations
* [Nitronic Rush](http://nitronic-rush.com/) is the other game that made me want to learn C++ in the first place
* [N++](http://www.nplusplus.org/) for just being a good game (also the developers had some [good GDC talks](https://www.youtube.com/watch?v=VZ4xevskMCI) and have a fantastic game development blog)
