#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include "rt/Transform.h"
#include "rt/Camera.h"
#include "rt/Ray.h"
#include "rt/Material.h"

namespace rt {

	class Light
	{
	public:
		Light()
			: m_IsPointLight( false )
		{}

		Light( const glm::vec3& pos, const glm::vec3& color, const glm::vec3& attenuation, const Transform& tr, bool is_point )
			: m_LightColor( color ),
			  m_Attenuation( attenuation ), m_IsPointLight( is_point )
		{
			if( is_point )
				m_LightPosition = ( tr.transform * glm::vec4( pos, 1.0f ) ).swizzle( glm::X, glm::Y, glm::Z );
			else
				m_LightPosition = ( tr.transform * glm::vec4( pos, 0.0f ) ).swizzle( glm::X, glm::Y, glm::Z );
		}

		Light( const Light& rhs )
			: m_LightPosition( rhs.m_LightPosition ), m_LightColor( rhs.m_LightColor ),
			  m_Attenuation( rhs.m_Attenuation )/*, m_Transform( rhs.m_Transform )*/, m_IsPointLight( rhs.m_IsPointLight )
		{}

		Light& operator = ( const Light& rhs )
		{
			m_LightPosition = rhs.m_LightPosition; m_LightColor = rhs.m_LightColor;
			m_Attenuation = rhs.m_Attenuation; m_IsPointLight = rhs.m_IsPointLight;
			//m_Transform = rhs.m_Transform;
			return *this;
		}

		Ray   rayToLight( const glm::vec3& hit_pt, const glm::vec3&  ) const
		{
			const glm::vec3 direction = glm::normalize( m_IsPointLight ? glm::vec3( m_LightPosition - hit_pt ) : m_LightPosition );
			const glm::vec3 origin = hit_pt + direction * 0.0005f;

			return Ray( origin, glm::vec4( direction, 0.0f ) );
		}

		float  distToLight( const glm::vec3& hit_pt ) const
		{
			return m_IsPointLight ? glm::distance( m_LightPosition, hit_pt ) : std::numeric_limits<float>::max();
		}

		glm::vec4 brdf( const glm::vec3& eye, const glm::vec3& hit_pt, const glm::vec3& hit_normal, const Material& mat ) const
		{
			const glm::vec3 direction = glm::normalize( m_IsPointLight ? glm::vec3( m_LightPosition - hit_pt ) : m_LightPosition );

			const float d = m_IsPointLight ? glm::distance( m_LightPosition, hit_pt ) : 0.0f;
			const float att =  m_IsPointLight ? 1.0f / glm::dot( m_Attenuation, glm::vec3( 1.0f, d, d * d ) ) : 1.0f;

			const glm::vec3 diffuse = glm::vec3( mat.Diffuse ) * glm::max( 0.0f, glm::dot( hit_normal, direction ) );

			const glm::vec3 eyedirn = glm::normalize(eye - hit_pt);
			const glm::vec3 half = glm::normalize( eyedirn + direction );
			const float     sterm= glm::pow( glm::max( glm::dot( half, hit_normal ), 0.0f), mat.Shininess );

			return glm::vec4( att * m_LightColor * ( diffuse + glm::vec3(mat.Specular) * sterm ), 0.0f );
		}
	private:
		glm::vec3  m_LightPosition;
		glm::vec3  m_LightColor;
		glm::vec3  m_Attenuation;

		//Transform  m_Transform;

		bool       m_IsPointLight;
	};

} // namespace rt

#endif // RT_LIGHT_H
