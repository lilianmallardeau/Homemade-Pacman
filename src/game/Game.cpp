//
// Created by lilian on 04/10/2021.
//

#include "Game.h"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../parameters.h"


void Game::computeProjectionMatrix() {
    int width, height;
    glfwGetWindowSize(Renderer::context, &width, &height);
    float mapAspectRatio = (float) map.getWidth() / (float) map.getHeight();
    float windowAspectRatio = (float) width / (float) height;
    float scale, widthBorder, heightBorder;
    if (windowAspectRatio > mapAspectRatio) {
        scale = (float) height / (float) map.getHeight();
        widthBorder = ((float) width - ((float) map.getWidth()*scale))/(2.0f*scale);
        heightBorder = 0;
    } else {
        scale = (float) width / (float) map.getWidth();
        widthBorder = 0;
        heightBorder = ((float) height - ((float) map.getHeight()*scale))/(2.0f*scale);
    }
    projectionMatrix = glm::ortho<float>(
        -1.0f - widthBorder, (float) map.getWidth() + widthBorder,
        -1.0f - heightBorder, (float) map.getHeight() + heightBorder,
        1.0f, -1.0f
    );
    glViewport(0, 0, width, height);
}

void Game::updateProjectionMatrix() {
    map.setProjectionMatrix(&projectionMatrix);
    pacman->setProjectionMatrix(&projectionMatrix);
    std::for_each(ghosts.cbegin(), ghosts.cend(), [this](Ghost* ghost){ghost->setProjectionMatrix(&projectionMatrix);});
}

bool Game::init() {
    srand(time(NULL)); //NOLINT(cert-msc51-cpp, modernize-use-nullptr)
    if (!Renderer::init(map.getWidth()*SCALE, map.getHeight()*SCALE, FULLSCREEN))
        return false;

    computeProjectionMatrix();
    map.init();
    map.setProjectionMatrix(&projectionMatrix);
    pacman = new Pacman(map.getInitialPosition());
    pacman->setProjectionMatrix(&projectionMatrix);
    for (int i = 0; i < GHOSTS_NUMBER; ++i) {
        Position ghostPos;
        do {
            ghostPos = map.getRandomPosition(MapElement::PELLET);
        } while (ghostPos % pacman->getPosition() < MIN_GHOST_INIT_DISTANCE);
        Ghost* ghost = new Ghost(ghostPos);
        ghost->setProjectionMatrix(&projectionMatrix);
        ghosts.push_back(ghost);
    }

    return true;
}

void Game::loop() {
    Renderer::Clear(BACKGROUND_COLOR);
    double currentTime;
    double lastPacmanAnimationUpdate = 0, lastGhostsAnimationUpdate = 0;
    while (!glfwWindowShouldClose(Renderer::context)) {
        glfwPollEvents();
        checkForKeyPress();

        // Update current time
        currentTime = glfwGetTime();

        if (!checkForGameEnd()) {
            // Ghosts
            for (Ghost *ghost: ghosts) {
                if (map.canTurn(ghost->getPosition(), ghost->getDirection())) {
                    Direction newDir;
                    do {
                        newDir = map.getRandomDirection(ghost->getPosition());
                    } while (newDir == Map::getOppositeDirection(ghost->getDirection()));
                    ghost->setDirection(newDir);
                }
                else if (!map.canGo(ghost->getPosition(), ghost->getDirection(), ghost->getDirection()))
                    ghost->setDirection(Map::getOppositeDirection(ghost->getDirection()));
                ghost->move(GHOSTS_SPEED);
            }

            // Pacman
            if (keyBuffer != Direction::NONE) {
                if (map.canGo(pacman->getPosition(), pacman->getDirection(), keyBuffer)) {
                    pacman->setDirection(keyBuffer);
                    keyBuffer = Direction::NONE;
                }
            }
            if (map.canGo(pacman->getPosition(), pacman->getDirection(), pacman->getDirection()))
                pacman->move(PACMAN_SPEED);
            map.removePellet(pacman->getPosition());

            // Pacman and ghosts animation
            if (currentTime - lastPacmanAnimationUpdate > PACMAN_ANIMATION_SPEED) {
                pacman->useNextSprite();
                lastPacmanAnimationUpdate = currentTime;
            }
            if (currentTime - lastGhostsAnimationUpdate > GHOSTS_ANIMATION_SPEED) {
                std::for_each(ghosts.cbegin(), ghosts.cend(), [](Ghost *ghost) { ghost->useNextSprite(); });
                lastGhostsAnimationUpdate = currentTime;
            }

            // Computing and updating the projection matrix in the map and entities objects
            computeProjectionMatrix();
            updateProjectionMatrix();

            // Rendering everything on the screen
            Renderer::Clear();
            map.render();
            pacman->render();
            std::for_each(ghosts.cbegin(), ghosts.cend(), [](Ghost *ghost) { ghost->render(); });

            glfwSwapBuffers(Renderer::context);
        }
    }
    glfwTerminate();
}

void Game::checkForKeyPress() {
    if (glfwGetKey(Renderer::context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Renderer::context, GLFW_TRUE);
    if (glfwGetKey(Renderer::context, GLFW_KEY_UP) == GLFW_PRESS)
        keyBuffer = Direction::UP;
    if (glfwGetKey(Renderer::context, GLFW_KEY_DOWN) == GLFW_PRESS)
        keyBuffer = Direction::DOWN;
    if (glfwGetKey(Renderer::context, GLFW_KEY_LEFT) == GLFW_PRESS)
        keyBuffer = Direction::LEFT;
    if (glfwGetKey(Renderer::context, GLFW_KEY_RIGHT) == GLFW_PRESS)
        keyBuffer = Direction::RIGHT;
}

bool Game::checkForCollision() const {
    return std::any_of(ghosts.cbegin(), ghosts.cend(), [this](Ghost* ghost){
        return ghost->getPosition().round() == pacman->getPosition().round();
    });
}

bool Game::checkForSuccess() const {
    return map.count(MapElement::PELLET) == 0;
}

bool Game::checkForGameEnd() const {
    return checkForCollision() || checkForSuccess();
}
