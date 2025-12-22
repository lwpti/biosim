#include "Vision.h"
#include "data/Location.h"
#include "data/Status.h"
#include "organs/Eyes.h"
#include "percepts/Sight.h"
#include "flecs.h"
#include "raylib.h"

namespace simbio {
    namespace systems {
        void Vision::registerVisionSystem(World& world) {
            using namespace simbio::organism;
            world.flecsWorld.system<Location, Eyes, Status>().kind(flecs::PostLoad).each([&](flecs::entity entity,
                const Location& location, const Eyes eyes, Status status) {
                Sight sight;

                status.energy -= VISION_ENERGY_COST * eyes.size / Eyes::MAX_SIZE * world.timeStep;
                
                if (eyes.size == 0) return;

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
                    float pointX = entityX - viewDist;
                    if (pointX < minX) minX = pointX;
                }

                if (std::fabs(0.5f * PI - yaw) < halfFOV) {
                    float pointY = entityY - viewDist;
                    if (pointY < minY) minY = pointY;
                }

                if (std::fabs(-0.5f * PI - yaw) < halfFOV) {
                    float pointY = entityY + viewDist;
                    if (pointY > maxY) maxY = pointY;
                }

                if (std::fabs(yaw) < halfFOV) {
                    float pointX = entityX + viewDist;
                    if (pointX > maxX) maxX = pointX;
                }
                
                world.forEntitiesInBox(minX, minY, maxX, maxY, [&](organism::Entity& targetEntity) {
                    if (targetEntity.flecsID == entity.id()) return;
                    Vector2 dist = world.distance(location, targetEntity.location);
                    float dist2 = dist.x * dist.x + dist.y * dist.y;
                    if (dist2 > viewDist * viewDist) return;
                    float angle = std::atan2(dist.y, dist.x) - yaw;
                    if (angle > PI || angle <= -PI) angle = std::atan2(std::sin(angle), std::cos(angle));
                    if (std::fabs(angle) > halfFOV) return;
                    float yawX = std::cos(location.yaw);
					float yawY = std::sin(location.yaw);
					Velocity relativeV{ 
					    targetEntity.velocity.x * yawX + targetEntity.velocity.y * yawY, 
                        -targetEntity.velocity.x * yawY + targetEntity.velocity.y * yawX 
					};
                    sight.visibleEntities.push_back({ targetEntity, relativeV, std::sqrt(dist2), angle });
                });

                if (Sight* s = entity.try_get_mut<Sight>()) s->visibleEntities = sight.visibleEntities;
                else entity.set<Sight>(sight);
            });
        }
    }
}
