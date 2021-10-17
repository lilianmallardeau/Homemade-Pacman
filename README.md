# Homemade Pacman
Homemade Pacman with C++ and OpenGL

![homemade_pacman_screenshot](https://user-images.githubusercontent.com/56023249/137645276-d19c4043-2d7f-45b7-97dd-9903099c957c.png)


## Clone

Clone the repository with the `recurse-submodules` option to clone the libraries repositories as well: 
```
git clone --recurse-submodules https://github.com/lilianmallardeau/Homemade-Pacman.git
```
Then move to the cloned folder.

## Build and run
```
mkdir build
cd build
cmake ..
make
cd ..
./build/Homemade_Pacman levels/level0
```
To run, the current working directory **MUST** be the root folder of the repository, because the program has to load the shaders files in the `src/shaders/` folder and the texture in the `assets/` folder.

## Changing the game options
Some game options can be changed in the `src/parameters.h` header file. Although their name is already kind of explicit, here is a brief description of each of them:

| Parameter | Description |
|:---------:|:-----------:|
| `FULLSCREEN` | 1 to enable fullscreen, 0 otherwise |
| `SCALE` | Initial size of each map unit, in pixels. Ignored if `FULSCREEN` is set to 1. |
| `PACMAN_SPEED` | Pacman movement at each frame, as a float[^1] |
| `GHOSTS_SPEED` | Ghosts movement at each frame, as a float[^1] |
| `GHOSTS_NUMBER` | Number of ghosts |
| `MIN_GHOST_INIT_DISTANCE` | Minimum distance between Pacman and the generated ghosts |
| `GHOSTS_ANIMATION_SPEED` | Delay in sec. between each sprite change for the ghosts |
| `PACMAN_ANIMATION_SPEED` | Delay in sec. between each sprite change for Pacman |
| `BACKGROUND_COLOR` | RGB background color |
| `WALLS_COLOR` | RGB walls color |
| `TUNNEL_COLOR` | RGB tunnels color |
| `PELLETS_COLOR` | RGB pellets color |

[^1]: The size of each map square is 1, ie. a speed of 1 will make pacman or the ghosts move of 1 square at each frame update (which is pretty fast 😅)

## TODOs
- [ ] Make the texture and the shader for `MovableEntities` static to avoid creating one for each entity
- [ ] Use smart pointers
- [ ] Use delta time instead of render loop to update the movement of the entities, to make the speed of the game independent of the system
