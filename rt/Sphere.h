#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "rt/Object.h"

namespace rt {

	class Sphere : public Object
	{
	public:
		Sphere( const glm::vec3& pos, float r, const Material& m, const Transform& t);

	private:
		glm::vec3   m_Center;
		float       m_Radius;

		// Object interface
	public:
		Hit closestHit(const Ray &ray) const;
	};

} // namespace rt

#endif // RT_SPHERE_H
