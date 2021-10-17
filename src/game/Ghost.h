//
// Created by lilian on 04/10/2021.
//

#ifndef GHOST_H
#define GHOST_H

#include "MovableEntity.h"
#include "Map.h"


class Ghost : public MovableEntity {
    private:
        int currentSprite = 0;

    public:
        explicit Ghost(const Position & initialPosition) : MovableEntity(initialPosition) {};

        void useNextSprite() override;
        void computeTextureCoordinates() override;
};


#endif //GHOST_H
