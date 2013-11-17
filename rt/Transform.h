#ifndef RT_TRANSFORM_H
#define RT_TRANSFORM_H

#include "glm/glm.hpp"

namespace rt {

	struct Transform
	{
	public:
		Transform() {}
		Transform( const glm::mat4& m )
			: transform( m ), invTransform( glm::inverse( m ) ), normalTransform( glm::transpose(glm::inverse(glm::mat3(m) ) ) )
		{}

		Transform( const Transform& rhs )
			: transform( rhs.transform ), invTransform( rhs.invTransform ), normalTransform( rhs.normalTransform )
		{}

		Transform& operator = ( const Transform& rhs )
		{
			transform = rhs.transform;
			invTransform = rhs.invTransform;
			normalTransform = rhs.normalTransform;
			return *this;
		}

		Transform operator * ( const Transform& rhs ) const
		{
			return Transform( transform * rhs.transform, rhs.invTransform * invTransform );
		}

		Transform& operator *= ( const Transform& rhs )
		{
			transform = transform * rhs.transform;
			invTransform = rhs.invTransform * invTransform;
			normalTransform = glm::transpose( glm::inverse( glm::mat3( transform ) ) );
			return *this;
		}

		glm::vec3  transformed( const glm::vec3& v ) const { return glm::vec3( transform * glm::vec4( v, 1.0f ) ); }

		glm::mat4  transform;
		glm::mat4  invTransform;
		glm::mat3  normalTransform;
	private:
		Transform( const glm::mat4& m, const glm::mat4& i )
			: transform( m ), invTransform( i )
		{ normalTransform = glm::transpose( glm::inverse( glm::mat3(m) ) ); }
	};

} // namespace rt

#endif // RT_TRANSFORM_H
