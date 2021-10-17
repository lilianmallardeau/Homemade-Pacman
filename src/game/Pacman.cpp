//
// Created by lilian on 04/10/2021.
//

#include "Pacman.h"


void Pacman::useNextSprite() {
    static int m = 1;
    currentSprite += m;
    if (currentSprite <= 0 || currentSprite >= 3)
        m *= -1;
}

void Pacman::computeTextureCoordinates() {
    Position baseCoordinate{
        (float) currentSprite * (1.f/6.f),
        direction == Direction::RIGHT ? 0.f :
        direction == Direction::LEFT ? .25f :
        direction == Direction::UP ? .5f :
        direction == Direction::DOWN ? .75f : 0.f
    };
    float coordinates[8] = {
            baseCoordinate.x, baseCoordinate.y,
            baseCoordinate.x + 1.f/6.f, baseCoordinate.y,
            baseCoordinate.x + 1.f/6.f, baseCoordinate.y + 1.f/4.f,
            baseCoordinate.x, baseCoordinate.y + 1.f/4.f
    };
    textureCoordinates.setData(8*sizeof(float), coordinates);
}