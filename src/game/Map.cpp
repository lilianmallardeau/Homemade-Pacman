//
// Created by lilian on 04/10/2021.
//

#include "Map.h"

#include <iostream>
#include <fstream>

#include "../parameters.h"


Map::Map(const std::string &filename) {
    loadMap(filename);
}

void Map::init() {
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer();
    wallsIndexBuffer = new IndexBuffer();
    pelletsIndexBuffer = new IndexBuffer();
    tunnelsIndexBuffer = new IndexBuffer();

    shader = new Program("src/shaders/map_vertexShader.shader", "src/shaders/map_fragmentShader.shader");

    computeVertices();
    vertexBuffer->setData(vertices.size()*sizeof(float), vertices.data());
    BufferLayout layout;
    layout.addAttribute<float>(2);
    vertexArray->addBuffer(*vertexBuffer, layout);

    computeIndexes(MapElement::WALL);
    wallsIndexBuffer->setData(indices[MapElement::WALL].size(), indices[MapElement::WALL].data());
}

void Map::loadMap(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string firstline;
    std::getline(file, firstline);
    sscanf(firstline.c_str(), "%ux%u", &width, &height); //NOLINT(cert-err34-c)
    map.resize(width, std::vector<MapElement>(height, MapElement::UNDEFINED));
    char c;
    MapPosition pos{0, height-1};
    while (file.get(c)) {
        if (c == '\n') {
            pos = {0, pos.y-1};
            continue;
        }
        if ('0' <= c && c <= '2') {
            map[pos.x][pos.y] = static_cast<MapElement>(c);
            if (c == '2')
                initialPosition = pos;
            ++pos.x;
        }
    }
}

void Map::computeVertices() {
    vertices.clear();
    vertices.reserve(2 * (width+1) * (height+1));
    for (unsigned int y = 0; y <= height; ++y) {
        for (unsigned int x = 0; x <= width; ++x) {
            vertices.push_back((float) x - .5f);
            vertices.push_back((float) y - .5f);
        }
    }
}

void Map::computeIndexes(MapElement type) {
    this->indices[type].clear();
    this->indices.reserve(6 * count(type));
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            if ((*this)[{x, y}] == type) {
                this->indices[type].push_back(y * (width+1) + x);
                this->indices[type].push_back(y * (width+1) + (x+1));
                this->indices[type].push_back((y+1) * (width+1) + (x+1));
                this->indices[type].push_back(y * (width+1) + x);
                this->indices[type].push_back((y+1) * (width+1) + x);
                this->indices[type].push_back((y+1) * (width+1) + (x+1));
            }
        }
    }
}

bool Map::removePellet(const MapPosition &position) {
    if ((*this)[position] == MapElement::PELLET) {
        map[position.x][position.y] = MapElement::TUNNEL;
        return true;
    }
    return false;
}

bool Map::canGo(const MapPosition & position, Direction direction) const {
    MapElement next = (*this)[position.move(direction)];
    return next == MapElement::TUNNEL || next == MapElement::PELLET;
}

bool Map::canGo(const Position & position, Direction currentDirection, Direction newDirection) const {
    if (newDirection != currentDirection && newDirection != Map::getOppositeDirection(currentDirection))
        return position.isCentered() && canGo(MapPosition(position), newDirection);
    else if (newDirection == currentDirection) {
        if (position.isCentered())
            return canGo(MapPosition(position), newDirection);
        else
            return !position.isHalfSquareWay(currentDirection) || canGo(MapPosition(position), newDirection);
    }
    else return newDirection == Map::getOppositeDirection(currentDirection);
}

bool Map::canTurn(const MapPosition &position, Direction currentDirection) const {
    if (canGo(position, Direction::UP) && Direction::UP != currentDirection && Direction::UP != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::DOWN) && Direction::DOWN != currentDirection && Direction::DOWN != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::LEFT) && Direction::LEFT != currentDirection && Direction::LEFT != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::RIGHT) && Direction::RIGHT != currentDirection && Direction::RIGHT != getOppositeDirection(currentDirection)) return true;
    return false;
}

bool Map::canTurn(const Position &position, Direction currentDirection) const {
    return position.isCentered() && canTurn(MapPosition(position), currentDirection);
}

unsigned int Map::count(MapElement element) const {
    unsigned int count = 0;
    for (unsigned int x = 0; x < width; ++x)
        for (unsigned int y = 0; y < height; ++y)
            if ((*this)[{x, y}] == element)
                ++count;
    return count;
}

unsigned int Map::pelletsCount() const {
    return count(MapElement::PELLET);
}

void Map::setProjectionMatrix(glm::mat4* projMat) {
    projectionMatrix = projMat;
    shader->setUniformMatrix4("u_Projection", *projectionMatrix);
};

MapPosition Map::getRandomPosition(MapElement type) const {
    if (!count(type))
        throw std::runtime_error("Trying to get random position of non-present map element");
    MapPosition pos;
    do {
        pos = {rand() % width, rand() % height}; //NOLINT(cert-msc50-cpp)
    } while ((*this)[pos] != type);
    return pos;
}

Direction Map::getRandomDirection(const MapPosition &pos) const {
    Direction direction;
    do {
        direction = static_cast<Direction>(rand() % 4); //NOLINT(cert-msc50-cpp)
    } while (!canGo(pos, direction));
    return direction;
}

Direction Map::getOppositeDirection(Direction dir) {
    return static_cast<Direction>(((int) dir + 2) % 4);
}

MapElement Map::operator[](const MapPosition & position) const {
    if (position.x < width && position.y < height)
        return map[position.x][position.y];
    return MapElement::UNDEFINED;
}

void Map::render() {
    computeIndexes(MapElement::PELLET);
    computeIndexes(MapElement::TUNNEL);
    pelletsIndexBuffer->setData(indices[MapElement::PELLET].size(), indices[MapElement::PELLET].data());
    tunnelsIndexBuffer->setData(indices[MapElement::TUNNEL].size(), indices[MapElement::TUNNEL].data());

    shader->setUniform<int>("u_DrawPellet", {false});

    // Walls
    shader->setUniform<float>("u_Color", {WALLS_COLOR, 1.f});
    Renderer::Draw(*vertexArray, *wallsIndexBuffer, *shader);

    // Tunnels
    shader->setUniform<float>("u_Color", {TUNNEL_COLOR, 1.f});
    Renderer::Draw(*vertexArray, *tunnelsIndexBuffer, *shader);
    Renderer::Draw(*vertexArray, *pelletsIndexBuffer, *shader);

    // Pellets
    shader->setUniform<float>("u_Color", {PELLETS_COLOR, 1.f});
    shader->setUniform<int>("u_DrawPellet", {true});
    Renderer::Draw(*vertexArray, *pelletsIndexBuffer, *shader);
}
