
#include "./world.hpp"

#include <cassert>

void World::add(Hittable *h){
	assert(h != nullptr);
	objects_.push_back(h);
}

void World::clear(){
	for(Hittable *curr: objects_)
		delete curr;
	
	objects_.clear();
}

bool World::hit(const Rayf& r, float tMin, float tMax, Hit& hit) const {
	Hit lastHit;
	bool didHit = false;
	float minDistance = tMax;
	
	for(const Hittable *curr: objects_){
		if( curr->hit(r, tMin, minDistance, lastHit) ){
			didHit = true;
			minDistance = lastHit.t;
			hit = lastHit;
		}
	}
	
	return didHit;
}
