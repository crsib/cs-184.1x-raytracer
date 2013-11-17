#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "rt/Material.h"
#include "rt/AABB.h"
#include "rt/Hit.h"
#include "rt/Transform.h"

namespace rt {

	class Object
	{
	public:
		Object();
		~Object() {}

		virtual Hit       closestHit( const Ray& ray ) const = 0;
		const AABB&       boundingBox() const { return m_BoundingBox; }
		const Material&   material() const { return m_Material; }
	protected:
		Material   m_Material;
		AABB       m_BoundingBox;
		Transform  m_Trasform;
	};

} // namespace rt

#endif // RT_OBJECT_H
