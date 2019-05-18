#ifndef world_h
#define world_h

#include "./hittable.hpp"

#include <vector>
#include <cassert>

class World: public Hittable {
public:
	World(): Hittable(nullptr) {}
	
	~World(){
		clear();
	}
	
	void add(Hittable *h);
	void clear();
	
	bool hit(const Rayf& r, float tMin, float tMax, Hit& hit) const override;
	
private:
	std::vector<Hittable*> objects_;
};

#endif /* world_h */
