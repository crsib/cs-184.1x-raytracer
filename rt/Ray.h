#ifndef RT_RAY_H
#define RT_RAY_H

#include "glm/glm.hpp"

namespace rt {

	class Ray
	{
	public:
		Ray() {}

		Ray( const Ray& rhs )
			: origin( rhs.origin ), direction( rhs.direction )
		{}
		Ray& operator = ( const Ray& rhs )
		{
			origin    = rhs.origin; direction = rhs.direction;
			return *this;
		}

		Ray( const glm::vec3& o, const glm::vec4& d )
			: origin( o ), direction( glm::normalize( d ) )
		{}

		glm::vec3  point( float t ) const
		{
			return origin + t * glm::vec3( direction.x, direction.y, direction.z );
		}

		glm::vec3  origin;
		glm::vec4  direction;

	};

} // namespace rt

#endif // RT_RAY_H
