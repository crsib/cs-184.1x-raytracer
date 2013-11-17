#ifndef RT_HIT_H
#define RT_HIT_H

#include <limits>

namespace rt {

	class Object;

	struct Hit
	{
	public:
		Hit() : Distance( std::numeric_limits< float >::max() ), Obj( NULL ) {}

		Hit( const Hit& rhs )
			: Distance( rhs.Distance ), Obj( rhs.Obj ),
			  Point( rhs.Point ), Normal( rhs.Normal )
		{}

		Hit& operator = ( const Hit& rhs )
		{
			Distance = rhs.Distance; Obj = rhs.Obj;
			Point = rhs.Point; Normal = rhs.Normal;
			return *this;
		}

		float     Distance;

		const Object*	  Obj;
		glm::vec3 Point;
		glm::vec3 Normal;

		Hit( float t, const rt::Object* o, const glm::vec3& p, const glm::vec3& n )
			: Distance( t ), Obj( o ), Point( p ), Normal( n )
		{}
	};

} // namespace rt

#endif // RT_HIT_H
