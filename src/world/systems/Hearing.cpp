#include "Hearing.h"
#include <cmath>
#include "data/Status.h"
#include "raylib.h"

namespace biosim {
	namespace systems {
		using namespace organism;
		void Hearing::registerHearingSystem(World& world) {
			world.flecsWorld.system<Body, Legs, Location, Velocity>().each(
				[&](flecs::entity source, const Body& body, 
					const Legs& legs, const Location& location, const Velocity& velocity) {
					float speed2 = velocity.x * velocity.x + velocity.y * velocity.y;
					if (speed2 < 0.0001f) return;

					float radius = (body.size + legs.size) * 10.0f;

					int startChunkY = std::floor((location.y - radius) / World::CHUNK_SIZE);
					int endChunkY = std::floor((location.y + radius) / World::CHUNK_SIZE);
					int startChunkX = std::floor((location.x - radius) / World::CHUNK_SIZE);
					int endChunkX = std::floor((location.x + radius) / World::CHUNK_SIZE);
					for (int chunkY = startChunkY; chunkY <= endChunkY; ++chunkY) {
						for (int chunkX = startChunkX; chunkX <= endChunkX; ++chunkX) {
							int cX = chunkX;
							int cY = chunkY;
							if (cX < 0) cX += world.chunkCols;
							else if (cX >= world.chunkCols) cX -= world.chunkCols;
							if (cY < 0) cY += world.chunkRows;
							else if (cY >= world.chunkRows) cY -= world.chunkRows;
							auto& bucket = world.chunkGrid[cY * world.chunkCols + cX];
							for (auto& entity : bucket) {
								if (entity.flecsID == source.id()) continue;
								flecs::entity listener = world.flecsWorld.entity(entity.flecsID);
								
								const Ears* ears = listener.try_get<Ears>();
								if (!ears || ears->size == 0.0f) continue;

								float range = radius * ears->size / 5.0f;
								float distX = entity.location.x - location.x;
								float distY = entity.location.y - location.y;
								float dist2 = distX * distX + distY * distY;
								if (dist2 > range * range) continue;

								float volume = radius - std::sqrt(dist2);
								float freq = 180.0f + body.size * 8.0f + legs.size * 4.0f;
								float direction = std::atan2(distY, distX) - entity.location.yaw;
								if (std::fabs(direction) > PI) direction = std::atan2(std::sin(direction), std::cos(direction));

								Sound* heard = listener.try_get_mut<Sound>();
								if (heard == nullptr) listener.set<Sound>({ volume, freq, direction });
								else if (heard->volume < 0.0001f) {
									heard->volume = volume;
									heard->freq = freq;
									heard->direction = direction;
								} else  {
									float sound1X = heard->volume * std::cos(heard->direction);
									float sound1Y = heard->volume * std::sin(heard->direction);
									float sound2X = volume * std::cos(direction);
									float soundY2 = volume * std::sin(direction);
									float soundX = sound1X + sound2X;
									float soundY = sound1Y + soundY2;
									heard->volume = std::sqrt(soundX * soundX + soundY * soundY);
									heard->freq = (heard->freq + freq) / 2;
									heard->direction = std::atan2(soundY, soundX);
								}
							}
						}
					}
				}
			);
		}
	}
}
