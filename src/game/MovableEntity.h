//
// Created by lilian on 04/10/2021.
//

#ifndef MOVABLE_ENTITY_H
#define MOVABLE_ENTITY_H

#include <glm/glm.hpp>

#include "../opengl/opengl_abstraction.hpp"
#include "Map.h"


class MovableEntity {
    protected:
        VertexArray vertexArray;
        VertexBuffer vertexBuffer, textureCoordinates;
        IndexBuffer indexBuffer;
        Program* shader;
        Texture* texture;
        glm::mat4* projectionMatrix = nullptr;

        Position position;
        Direction direction = Direction::NONE;

        void computeVertices();

    public:
        explicit MovableEntity(const Position &);

        [[nodiscard]] Position getPosition() const { return position; };
        [[nodiscard]] Direction getDirection() const { return direction; };
        void setPosition(const Position & pos) { position = pos; };
        void setDirection(const Direction & dir) { direction = dir; center(); };
        void center() { position = position.round(); };
        void setProjectionMatrix(glm::mat4* projMat);

        void move(const Position & diff) { position += diff; };
        void move(float distance = 0.05f) { position.go(direction, distance); };

        virtual void render();
        virtual void useNextSprite() = 0;
        virtual void computeTextureCoordinates() = 0;
};


#endif //MOVABLE_ENTITY_H
