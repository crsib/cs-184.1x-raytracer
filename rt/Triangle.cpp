#include "Triangle.h"

#include <QDebug>

namespace rt {

	Triangle::Triangle()
	{
	}

	Hit Triangle::closestHit(const Ray &ray) const
	{
		const glm::vec3 p = ray.origin;
		const glm::vec3 d = glm::vec3(ray.direction);

		const glm::vec3 E1 = m_V1 - m_V0;
		const glm::vec3 E2 = m_V2 - m_V0;

		const glm::vec3 T = p - m_V0;
		const glm::vec3 P = glm::cross( d, E2 );
		const glm::vec3 Q = glm::cross( T, E1 );

		const float scale = 1.0f / glm::dot( P, E1 );

		const float u = glm::dot( P, T ) * scale;
		const float v = glm::dot( Q, d ) * scale;
		const float w = 1.0f - u - v;
		const float t = glm::dot( Q, E2 ) * scale;

		if( t >= 0 && u >= 0.0f && v >= 0.0f && w >= 0.0f )
		{
			const glm::vec3 pt = w * m_V0 + u * m_V1 + v * m_V2;
			const glm::vec3 n  = w * m_N0 + u * m_N1 + v * m_N2;

			return Hit( t, this, p + d*t, glm::normalize(n) );
		}

		return Hit();
	}
} // namespace rt


