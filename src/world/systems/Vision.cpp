#include "Vision.h"
#include "percepts/Sight.h"
#include "data/Location.h"
#include "Entity.h"
#include "data/Status.h"

namespace simbio {
    namespace systems {
        void Vision::registerVisionSystem(std::vector<std::vector<organism::Entity>>& chunkGrid, 
            int chunkSize, int chunkCols, int chunkRows) {
            Vision::chunkGrid = &chunkGrid;
            Vision::chunkSize = chunkSize;
            Vision::chunkCols = chunkCols;
            Vision::chunkRows = chunkRows;      
        }

        organism::Sight Vision::computeSightPercept(flecs::entity entity, float dt,
            const organism::Location& location, const organism::Eyes eyes, organism::Status status) {
            simbio::organism::Sight sightPercept;
            // I am temporarily putting a drain on all entity energy here
            // This should be in its own system like "Hunger"
            status.energy -= (0.005f + eyes.size * 0.05) * dt;
            if (eyes.size == 0) return sightPercept;

            float entityX = location.x;
            float entityY = location.y;
            float yaw = location.yaw;
            float halfFOV = Vision::FOV * (PI / 180.0f) * 0.5f;
            float viewDist = eyes.size * Vision::RANGE_MULT;

            float angle1 = yaw - halfFOV;
            if (angle1 > PI || angle1 <= -PI) angle1 = std::atan2(std::sin(angle1), std::cos(angle1));
            float point1X = entityX + viewDist * std::cos(angle1);
            float point1Y = entityY + viewDist * std::sin(angle1);

            float angle2 = yaw + halfFOV;
            if (angle2 > PI || angle2 <= -PI) angle2 = std::atan2(std::sin(angle2), std::cos(angle2));
            float point2X = entityX + viewDist * std::cos(angle2);
            float point2Y = entityY + viewDist * std::sin(angle2);

            float minX = std::min({ entityX, point1X, point2X });
            float minY = std::min({ entityY, point1Y, point2Y });
            float maxX = std::max({ entityX, point1X, point2X });
            float maxY = std::max({ entityY, point1Y, point2Y });

            float ax = 0.0f;
            float diff = std::atan2(std::sin(ax - yaw), std::cos(ax - yaw));
            if (std::fabs(diff) <= halfFOV) {
                float px = entityX + viewDist * std::cos(ax);
                float py = entityY + viewDist * std::sin(ax);
                if (px < minX) minX = px;
                if (px > maxX) maxX = px;
                if (py < minY) minY = py;
                if (py > maxY) maxY = py;
            }

            if (PI - std::fabs(yaw) < halfFOV) {
                int pointX = entityX - viewDist;
                if (pointX < minX) minX = pointX;
            }

            if (std::fabs(0.5f * PI - yaw) < halfFOV) {
                int pointY = entityY - viewDist;
                if (pointY < minY) minY = pointY;
            }

            if (std::fabs(-0.5f * PI - yaw) < halfFOV) {
                int pointY = entityY + viewDist;
                if (pointY > maxY) maxY = pointY;
            }

            if (std::fabs(yaw) < halfFOV) {
                int pointX = entityX + viewDist;
                if (pointX > maxX) maxX = pointX;
            }
            

            int startChunkY = std::floor(minY / Vision::chunkSize);
            int endChunkY = std::floor(maxY / Vision::chunkSize);
            int startChunkX = std::floor(minX / Vision::chunkSize);
            int endChunkX = std::floor(maxX / Vision::chunkSize);
            for (int chunkY = startChunkY; chunkY <= endChunkY; chunkY++) {
                for (int chunkX = startChunkX; chunkX <= endChunkX; chunkX++) {
                    int cX = chunkX;
                    int cY = chunkY;
                    if (cX < 0) cX += chunkCols;
                    else if (cX >= chunkCols) cX -= chunkCols;
                    if (cY < 0) cY += chunkRows;
                    else if (cY >= chunkRows) cY -= chunkRows;
                    auto& bucket = (*Vision::chunkGrid)[cY * Vision::chunkCols + cX];
                    for (auto& targetEntity : bucket) {
                        if (targetEntity.flecsID == entity.id()) continue;
                        float distX = targetEntity.location.x - entityX;
                        float distY = targetEntity.location.y - entityY;
                        float dist2 = distX * distX + distY * distY;
                        if (dist2 > viewDist * viewDist) continue;
                        float angle = std::atan2(distY, distX) - yaw;
                        if (angle > PI || angle <= -PI) angle = std::atan2(std::sin(angle), std::cos(angle));
                        if (std::fabs(angle) > halfFOV) continue;
                        sightPercept.visibleEntities.push_back(targetEntity);
                    }
                }
            }

            return sightPercept;
        } 
    }
}
