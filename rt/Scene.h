#ifndef RT_SCENE_H
#define RT_SCENE_H

#include "rt/Camera.h"
#include "rt/Light.h"
#include "rt/Object.h"
#include "rt/Hit.h"

#include <QtCore/QFuture>
#include <QtGui/QRgb>

namespace rt {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void               reset();

		QFuture<bool>      loadScene( const QString& path, int w, int h, bool override_size );
		bool               loadSceneSynchronous( const QString& path, int w, int h, bool override_size );

		int                sceneWidth() const { return m_Camera.width(); }
		int                sceneHeight() const { return m_Camera.height(); }

		QString            outputFileName() const { return m_OutputFileName; }

		QRgb               evaluatePixel( int i, int j ) const;
	private:
		void               buildObjectList( const rt::Ray& ray, QList<rt::Object*>& objects ) const;
		bool               closestHit(const rt::Ray& ray, const QList<rt::Object *> &objects, Hit &hit ) const;
		glm::vec4          evaluateHit(const glm::vec3 &eye, const rt::Ray& ray, const rt::Hit& hit, int depth ) const;

		Camera             m_Camera;

		int                m_MaxDepth;
		QString            m_OutputFileName;

		glm::vec4          m_AmbientColor;

		QList<rt::Object*> m_Objects;
		QList<rt::Light* > m_Lights;
	};

} // namespace rt

#endif // RT_SCENE_H
