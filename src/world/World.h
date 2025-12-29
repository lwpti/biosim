#pragma once

#include "flecs.h"
#include "data/Data.h"
#include "Entity.h"
#include "Organism.h"
#include <vector>
#include <functional>
#include <raylib.h>

namespace biosim {
    class World {
        public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int WIDTH = 1024;
        static constexpr int HEIGHT = 1024;

        flecs::world flecsWorld;

        int simulationSpeed = 1;

        const float timeStep;
        const int chunkCols;
        const int chunkRows;

        std::vector<std::vector<organism::Entity>> chunkGrid;

        World(float timeStep);

        bool spawnFlower(float size, float x, float y);
        void progress();
        static Vector2 distance(const biosim::organism::Location& from, const biosim::organism::Location& to);
        static Vector2 distance(float fromX, float fromY, float toX, float toY);

        template <typename Brain>
        bool spawnOrganism(organism::Organism<Brain>& organism, float x, float y) {
            using namespace organism;
            
            flecs::entity entity = organism.create();
            const Body* body = entity.try_get<Body>();

            if (!body) return false;
            if (overlapsEntity(x, y, body->size)) return false;

            int chunkX = std::min((int)(x / CHUNK_SIZE), chunkCols - 1);
            int chunkY = std::min((int)(y / CHUNK_SIZE), chunkRows - 1);
            int chunk = chunkY * chunkCols + chunkX;

            entity.set<Location>({ x, y, 0.0f, chunk });
            entity.set<Velocity>({ 0.0f, 0.0f });
            float sizeRatio = body->size / Body::MAX_SIZE;
            entity.set<Status>({ sizeRatio * Status::MAX_HEALTH, sizeRatio * Status::MAX_ENERGY });

            Organs organs;
            organs.body = *body;
            if (entity.has<Arms>()) organs.arms = entity.get<Arms>();
            if (entity.has<Ears>()) organs.ears = entity.get<Ears>();
            if (entity.has<Eyes>()) organs.eyes = entity.get<Eyes>();
            if (entity.has<Legs>()) organs.legs = entity.get<Legs>();
            if (entity.has<Mouth>()) organs.mouth = entity.get<Mouth>();

            chunkGrid[chunk].emplace_back(
                entity.id(), 
                entity.has<Color>() ? entity.get<Color>() : Color{ 255, 255, 255 },
                entity.get<Location>(),
                entity.get<Status>(),
                entity.get<Velocity>(),
                organs
            );

            return true;
        }

        void forNearbyEntities(biosim::organism::Location location,
             const std::function<void(organism::Entity&)>& doThis);

        void forEntitiesInBox(float minX, float minY, float maxX, float maxY, 
            const std::function<void(organism::Entity&)>& doThis);

        private:
        bool overlapsEntity(float x, float y, float radius);
    };
}