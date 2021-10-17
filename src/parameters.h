#ifndef PARAMETERS_H
#define PARAMETERS_H

// Full screen mode and window settings
#define FULLSCREEN                  0
#define SCALE                       25

// GAME PARAMETERS
#define PACMAN_SPEED                .10f
#define GHOSTS_SPEED                .05f
#define GHOSTS_NUMBER               5
#define MIN_GHOST_INIT_DISTANCE     10 // A value larger than the map will result in an infinite loop while initializing the ghosts

// ANIMATION PARAMETERS
#define GHOSTS_ANIMATION_SPEED      0.25
#define PACMAN_ANIMATION_SPEED      0.05

// MAP STYLE
#define BACKGROUND_COLOR            0, 0, 0
#define WALLS_COLOR                 0, 52.f/255.f, 252.f/255.f
#define TUNNEL_COLOR                0, 0, 0
#define PELLETS_COLOR               1, 1, 0


#endif //PARAMETERS_H
