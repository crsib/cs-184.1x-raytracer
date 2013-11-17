#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include <glm/glm.hpp>

#include "rt/Transform.h"
#include "rt/Ray.h"

namespace rt {

	class Camera
	{
	public:
		Camera() {}
		Camera( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, float fovy, int width, int height );

		Camera( const Camera& rhs )
			: m_CameraTransform( rhs.m_CameraTransform ),
			  m_UVec( rhs.m_UVec ), m_VVec( rhs.m_VVec ), m_WVec( rhs.m_WVec ), m_Eye( rhs.m_Eye ),
			  m_VerticalPixels( rhs.m_VerticalPixels ),	  m_HorizontalPixels( rhs.m_HorizontalPixels ),
			  m_Width( rhs.m_Width ), m_Height( rhs.m_Height )
		{}
		Camera& operator = ( const Camera& rhs )
		{
			m_CameraTransform = rhs.m_CameraTransform;
			m_UVec = rhs.m_UVec; m_VVec = rhs.m_VVec; m_WVec = rhs.m_WVec; m_Eye = rhs.m_Eye;
			m_VerticalPixels  = rhs.m_VerticalPixels; m_HorizontalPixels= rhs.m_HorizontalPixels;
			m_Width = rhs.m_Width; m_Height = rhs.m_Height;
			return *this;
		}

		const Transform& transform() const { return m_CameraTransform; }
		Ray              ray( int i, int j ) const
		{
			const float half_width = float(m_Width) / 2.0f;
			const float half_height= float(m_Height)/ 2.0f;

			const glm::vec3 dirn =
					m_HorizontalPixels * ( i + 0.5f - half_width ) / half_width * m_UVec +
					m_VerticalPixels * ( half_height - j - 0.5f ) / half_height * m_VVec -
					m_WVec;

			return Ray( m_Eye,  glm::vec4( dirn, 0.0f ) );
		}

		int          width() const { return m_Width; }
		int          height() const { return m_Height; }

		glm::vec3    eye() const { return m_Eye; }
	private:
		Transform    m_CameraTransform;

		glm::vec3    m_UVec;
		glm::vec3    m_VVec;
		glm::vec3    m_WVec;
		glm::vec3    m_Eye;

		float        m_VerticalPixels;
		float        m_HorizontalPixels;
		int          m_Width;
		int          m_Height;
	};

} // namespace rt

#endif // RT_CAMERA_H
