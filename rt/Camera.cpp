#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace rt {

	Camera::Camera(const glm::core::type::vec3 &eye, const glm::core::type::vec3 &center,
				   const glm::core::type::vec3 &up,  float fovy, int width, int height   )
		: m_Eye(eye), m_Width( width ), m_Height( height )
	{
		m_CameraTransform = glm::lookAt( eye, center, up );

		m_VerticalPixels = tanf( glm::radians( fovy ) / 2 );
		m_HorizontalPixels= m_VerticalPixels * float( width ) / float( height );

		m_WVec = glm::normalize( eye - center );
		m_UVec = glm::normalize( glm::cross( up, m_WVec ) );
		m_VVec = glm::cross( m_WVec, m_UVec );
	}

} // namespace rt
