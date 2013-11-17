#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "glm/glm.hpp"

namespace rt {

	struct Material
	{
		Material() : Ambient( 0.2f, 0.2f, 0.2, 1.0f ), Shininess( 100.0f ) {}
		Material( const Material& rhs)
			: Diffuse( rhs.Diffuse ), Specular( rhs.Specular ),
			  Emission( rhs.Emission ), Ambient(rhs.Ambient), Shininess( rhs.Shininess )
		{}

		Material& operator = ( const Material& rhs )
		{
			Diffuse = rhs.Diffuse; Specular = rhs.Specular;
			Emission = rhs.Emission; Shininess = rhs.Shininess;
			Ambient = rhs.Ambient;
			return *this;
		}

		glm::vec4  Diffuse;
		glm::vec4  Specular;
		glm::vec4  Emission;
		glm::vec4  Ambient;

		float      Shininess;
	};

} // namespace rt

#endif // RT_MATERIAL_H
