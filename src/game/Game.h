//
// Created by lilian on 04/10/2021.
//

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Map.h"
#include "Pacman.h"
#include "Ghost.h"


class Game {
    private:
        std::string level_file;
        Map map;
        Pacman* pacman;
        std::vector<Ghost*> ghosts;
        glm::mat4 projectionMatrix{};
        Direction keyBuffer = Direction::NONE;

    public:
        explicit Game(const std::string & level_file) : level_file(level_file), map(level_file) {};
        void computeProjectionMatrix();
        void updateProjectionMatrix();
        bool init();
        void loop();

        void checkForKeyPress();

        [[nodiscard]] bool checkForCollision() const;
        [[nodiscard]] bool checkForSuccess() const;
        [[nodiscard]] bool checkForGameEnd() const;
};


#endif //GAME_H
