//
// Created by lilian on 04/10/2021.
//

#include "MovableEntity.h"


MovableEntity::MovableEntity(const Position & initialPosition) : position(initialPosition) {
    BufferLayout layout;
    layout.addAttribute<float>(2);
    vertexArray.addBuffer(vertexBuffer, layout);
    vertexArray.addBuffer(textureCoordinates, layout);

    unsigned int indices[6] = {0, 1, 2, 0, 3, 2};
    indexBuffer.setData(6, indices);

    shader = new Program("src/shaders/entity_vertexShader.shader", "src/shaders/entity_fragmentShader.shader");
    texture = new Texture("assets/pacman.png");
    texture->Bind(0);
    shader->setUniform<int>("u_Texture", {0});
}

void MovableEntity::setProjectionMatrix(glm::mat4* projMat) {
    projectionMatrix = projMat;
    shader->setUniformMatrix4("u_Projection", *projectionMatrix);
};

void MovableEntity::computeVertices() {
    float vertices[8] = {
            position.x - 0.5f, position.y - 0.5f,
            position.x + 0.5f, position.y - 0.5f,
            position.x + 0.5f, position.y + 0.5f,
            position.x - 0.5f, position.y + 0.5f,
    };
    vertexBuffer.setData(8*sizeof(float), vertices);
}

void MovableEntity::render() {
    computeVertices();
    computeTextureCoordinates();
    Renderer::Draw(vertexArray, indexBuffer, *shader);
}