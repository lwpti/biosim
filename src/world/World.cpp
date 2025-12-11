#include "World.h"
#include "Entity.h"

namespace simbio {
    World::World(int width, int height, float timeStep) 
        : width(width - width % CHUNK_SIZE), height(height - height % CHUNK_SIZE), 
        timeStep(timeStep), chunkCols(width / CHUNK_SIZE), 
        chunkRows(height / CHUNK_SIZE), chunkGrid(chunkCols * chunkRows) {
    }

    bool World::spawnFlower(float size, float x, float y) {
        using namespace organism;

        if (overlapsEntity(x, y, size)) return false;

        int chunkX = std::min((int)(x / CHUNK_SIZE), chunkCols - 1);
        int chunkY = std::min((int)(y / CHUNK_SIZE), chunkRows - 1);
        int chunk = chunkY * chunkCols + chunkX;

        Location location{ x, y, 0.0f, chunk };

        flecs::entity flecsEntity = flecsWorld.entity();
        flecsEntity.set<Location>({ x, y, 0.0f, chunk });
        flecsEntity.set<Color>(Flower::FLOWER_COLOR);
        flecsEntity.set<Flower>({ size });

        Organs organs;
        organs.flower = Flower{size};
        Entity entity;
        entity.flecsID = flecsEntity.id();
        entity.color = Flower::FLOWER_COLOR;
        entity.location = location;
        entity.organs = organs;

        chunkGrid[chunk].push_back(entity);
        return true;
    }

    void World::progress() {
        for (int i = 0; i < simulationSpeed; ++i) flecsWorld.progress(timeStep);
    }

    void World::forNearbyEntities(simbio::organism::Location location, 
        const std::function<void(organism::Entity&)>& doThis) {
        const int chunkX = location.chunk % chunkCols;
        const int chunkY = location.chunk / chunkCols;
        for (int dy = -1; dy <= 1; ++dy) {
            int nearbyChunkY = chunkY + dy;
            if (nearbyChunkY < 0) nearbyChunkY += chunkRows;
            if (nearbyChunkY >= chunkRows) nearbyChunkY -= chunkRows;

            for (int dx = -1; dx <= 1; ++dx) {
                int nearbyChunkX = chunkX + dx;
                if (nearbyChunkX < 0) nearbyChunkX += chunkCols;
                if (nearbyChunkX >= chunkCols) nearbyChunkX -= chunkCols;

                auto& bucket = chunkGrid[nearbyChunkY * chunkCols + nearbyChunkX];
                for (auto& entity : bucket) {
                    doThis(entity);
                }
            }
        }
    }

    void World::forEntitiesInBox(float minX, float minY, float maxX, float maxY, 
        const std::function<void(organism::Entity&)>& doThis) {
        int startChunkY = std::floor(minY / CHUNK_SIZE);
        int endChunkY = std::floor(maxY / CHUNK_SIZE);
        int startChunkX = std::floor(minX / CHUNK_SIZE);
        int endChunkX = std::floor(maxX / CHUNK_SIZE);
        for (int chunkY = startChunkY; chunkY <= endChunkY; ++chunkY) {
            for (int chunkX = startChunkX; chunkX <= endChunkX; ++chunkX) {
                int cX = chunkX;
                int cY = chunkY;
                if (cX < 0) cX += chunkCols;
                else if (cX >= chunkCols) cX -= chunkCols;
                if (cY < 0) cY += chunkRows;
                else if (cY >= chunkRows) cY -= chunkRows;
                auto& bucket = chunkGrid[cY * chunkCols + cX];
                for (auto& entity : bucket) {
                    doThis(entity);
                }
            }
        }
    }

    bool World::overlapsEntity(float x, float y, float radius) {
        int chunkX = (int)(x / CHUNK_SIZE);
        int chunkY = (int)(y / CHUNK_SIZE);
        int chunk = chunkY * chunkCols + chunkX;

        for (int dy = -1; dy <= 1; ++dy) {
            int adjY = chunkY + dy;
            adjY = adjY < 0 ? chunkRows - 1 : adjY;
            adjY = adjY >= chunkRows ? 0 : adjY;

            for (int dx = -1; dx <= 1; ++dx) {
                int adjX = chunkX + dx;
                adjX = adjX < 0 ? chunkCols - 1 : adjX;
                adjX = adjX >= chunkCols ? 0 : adjX;

                auto& bucket = chunkGrid[adjY * chunkCols + adjX];
                for (int i = 0; i < bucket.size(); ++i) {
                    float distanceX = bucket[i].location.x - x;
                    float distanceY = bucket[i].location.y - y;
                    float minDistance = radius + 2.0f + bucket[i].getSize() * 0.5f;

                    if (distanceX * distanceX + distanceY * distanceY <= minDistance * minDistance) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
}