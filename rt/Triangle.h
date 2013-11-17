#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "rt/Object.h"

namespace rt {

	class Triangle : public Object
	{
	public:
		Triangle();
		Triangle( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const Transform& tr, const Material& m )
			: m_V0( tr.transformed( v0 ) ), m_V1( tr.transformed( v1 ) ), m_V2( tr.transformed( v2 ) )
		{
			const glm::vec3 n = glm::cross( m_V1 - m_V0, m_V2 - m_V0 );
			m_N0 = m_N1 = m_N2 = glm::normalize ( n );

			m_Trasform = tr;
			m_Material = m;

			m_BoundingBox = AABB().expand( m_V0 ).expand( m_V1 ).expand( m_V2 );
		}

		// Object interface
	public:
		Hit closestHit(const Ray &ray) const;

	private:
		glm::vec3     m_V0, m_V1, m_V2;
		glm::vec3     m_N0, m_N1, m_N2;
	};

} // namespace rt

#endif // RT_TRIANGLE_H
