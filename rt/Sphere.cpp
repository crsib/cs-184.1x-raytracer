#include "Sphere.h"

#include <QDebug>

namespace rt {

	Sphere::Sphere(const glm::core::type::vec3 &pos, float r, const Material &m, const Transform &t)
		: m_Center( pos ), m_Radius( r )
	{
		m_Material = m; m_Trasform = t;

		const glm::vec3 extent( r, r, r );

		AABB box;
		box.min = pos - extent;
		box.max = pos + extent;

		glm::vec3 bounds[8];

		bounds[0] = glm::vec3(box.max.x, box.max.y, box.min.z);
		bounds[1] = glm::vec3(box.min.x, box.max.y, box.min.z);
		bounds[2] = glm::vec3(box.min.x, box.min.y, box.min.z);
		bounds[3] = glm::vec3(box.max.x, box.min.y, box.min.z);
		bounds[4] = glm::vec3(box.max.x, box.min.y, box.max.z);
		bounds[5] = glm::vec3(box.max.x, box.max.y, box.max.z);
		bounds[6] = glm::vec3(box.min.x, box.max.y, box.max.z);
		bounds[7] = glm::vec3(box.min.x, box.min.y, box.max.z);

		for( int i = 0; i < 8; ++i )
			m_BoundingBox.expand( m_Trasform.transformed( bounds[i] ) );
	}

	rt::Hit rt::Sphere::closestHit(const Ray &ray) const
	{
		const glm::vec3 p = glm::vec3(m_Trasform.invTransform * glm::vec4( ray.origin, 1.0f )) - m_Center ;
		const glm::vec3 d = glm::vec3(m_Trasform.invTransform * ray.direction);

		const float a = glm::dot( d, d );
		const float b = glm::dot( d, p );
		const float c = glm::dot( p, p ) - m_Radius * m_Radius;

		const float d2= b * b - a * c;

		if( d2 >= 0.0f )
		{
			const float ia = 1.0f / a;
			const float sd = sqrtf( d2 );

			float t =  -b - sd;

			if( t > 0.0f )
			{
				t *= ia;

				const glm::vec3 pos = p + t * d + m_Center;

				return Hit( t, this, m_Trasform.transformed( pos ), glm::normalize( m_Trasform.normalTransform * (pos - m_Center) ) );
			}
		}

		return Hit();
	}

} // namespace rt


