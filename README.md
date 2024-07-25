# SDL ShootEmUp

This repo contains my attempt at creating a small game using only SDL and SDL_image as external dependencies. 

I have chosen to use C as SDL is written is also written in C, but also in order to practice this language as its something that i haven't had to do academically or professionaly. 

To compile on a Linux platform:

```
cd <your-path-here>/SDLShootEmUp
mkdir build
cd build
cmake ..
make
./SDLShootEmUp
```

It should also be possible to run this on Windows. Make sure that you create a folder called `extern` at the root of the project and place the dlls for both libraries in there.

More detail on what specifically is required can be found in the `CMakeLists.txt` file.

I have used these wonderful tutorials from Parallel Realities [(link here)](https://www.parallelrealities.co.uk/tutorials/) to make a start on this project but eventually ended up deviating from it quite a bit.

If you are interested in learning SDL I would highly recommend buying their tutorials on itch.io if you enjoy the free version, as they were incredible useful to me when starting out with this library. 