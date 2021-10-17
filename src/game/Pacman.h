//
// Created by lilian on 04/10/2021.
//

#ifndef PACMAN_H
#define PACMAN_H

#include "MovableEntity.h"
#include "Map.h"


class Pacman : public MovableEntity {
    private:
        int currentSprite = 0;

    public:
        explicit Pacman(const Position & initialPosition) : MovableEntity(initialPosition) {};

        void useNextSprite() override;
        void computeTextureCoordinates() override;
};


#endif //PACMAN_H
