#ifndef RT_AABB_H
#define RT_AABB_H

#include "glm/glm.hpp"
#include "rt/Ray.h"

#include <limits>

namespace rt {

	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;

		AABB()
			: min( std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() ),
			  max( -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() )
		{}

		AABB( const AABB& rhs )
			: min( rhs.min ), max( rhs.max )
		{}

		AABB& operator = ( const AABB& rhs )
		{
			min = rhs.min;
			max = rhs.max;
			return *this;
		}

		AABB& expand( const glm::vec3& pt )
		{
			min = glm::min( min, pt );
			max = glm::max( max, pt );
			return *this;
		}

		float        size() const { return glm::length( min - max ); }
		glm::vec3    center() const { return (min + max) * 0.5f; }

		bool         overlaps( const AABB& rhs ) const
		{
			return !( min.x > rhs.max.x ||
					  min.y > rhs.max.y ||
					  min.z > rhs.max.z ||
					  max.x < rhs.min.x ||
					  max.y < rhs.min.y ||
					  max.z < rhs.min.z );

		}

		bool          contains( const AABB& rhs ) const
		{
			return min.x >= rhs.min.x && min.y >= rhs.min.y && min.z >= rhs.min.z &&
					max.x <= rhs.max.x && max.y <= rhs.max.y && max.z <= rhs.max.z;
		}

		bool          contains( const glm::vec3& pt ) const
		{
			return  min.x <= pt.x && pt.x <= max.x &&
					min.y <= pt.y && pt.y <= max.y &&
					min.z <= pt.z && pt.z <= max.z;
		}

		bool          intersect( const Ray& ray, float& tmin, float& tmax ) const
		{
			const glm::vec3 inv_dir = glm::vec3( 1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z );
			const float lo = inv_dir.x*(min.x - ray.origin.x);
			const float hi = inv_dir.x*(max.x - ray.origin.x);
			tmin  = glm::min(lo, hi);
			tmax = glm::max(lo, hi);

			const float lo1 = inv_dir.y*(min.y - ray.origin.y);
			const float hi1 = inv_dir.y*(max.y - ray.origin.y);
			tmin  = glm::max(tmin, glm::min(lo1, hi1));
			tmax = glm::min(tmax, glm::max(lo1, hi1));

			const float lo2 = inv_dir.z*(min.z - ray.origin.z);
			const float hi2 = inv_dir.z*(max.z - ray.origin.z);
			tmin  = glm::max(tmin, glm::min(lo2, hi2));
			tmax = glm::min(tmax, glm::max(lo2, hi2));

			return (tmin <= tmax) && (tmax > 0.f);
		}
	};

} // namespace rt

#endif // RT_AABB_H
