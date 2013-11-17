#include "rt/Scene.h"

#include <QtConcurrent>
#include <QFile>

#include <functional>
#include "glm/ext.hpp"

#include "rt/Triangle.h"
#include "rt/Sphere.h"

#include "QDebug"

namespace rt {

	Scene::Scene()
	{
		reset();
	}

	Scene::~Scene()
	{
		reset();
	}

	void Scene::reset()
	{
		m_MaxDepth = 5;
		m_AmbientColor = glm::vec4( 0.2f, 0.2f, 0.2f, 1.0f );

		qDeleteAll(m_Objects);
		m_Objects.clear();
	}

	QFuture<bool> Scene::loadScene(const QString &path, int w, int h, bool override_size)
	{
		return QtConcurrent::run( std::bind( &Scene::loadSceneSynchronous, this,  path, w, h, override_size ) );
	}

	bool Scene::loadSceneSynchronous(const QString &path, int w, int h, bool override_size)
	{
		reset();

		QFile file( path );

		if( !file.open( QFile::ReadOnly ) )
			return false;

		int width = w, height = h;

		QStack< rt::Transform > transform_stack;
		transform_stack.push_back( glm::mat4( 1.0f ) );

		QList< glm::vec3   >    vertices;
		QList< rt::Object* >    objects;
		QList< rt::Light*  >    lights;

		float const_att = 1.0f, linear_att = 0.0f, quadratic_att = 0.0f;

		glm::vec4 ambient( 0.2f, 0.2f, 0.2f, 1.0f );
		glm::vec3 eye, center, up;
		float fovy = float( M_PI_4 );

		rt::Material material;

		while( !file.atEnd() )
		{
			QString line = file.readLine();
			line = line.remove( QRegExp( "#.*" ) ).trimmed();
			if( line.isEmpty() )
				continue ;

			QStringList tokens = line.split( QRegExp( "\\s" ), QString::SkipEmptyParts );

			QString command = tokens.at( 0 );

			if( command == "size" && !override_size )
			{
				width = tokens.at( 1 ).toInt();
				height= tokens.at( 2 ).toInt();
			}
			else if( command == "maxdepth" )
				m_MaxDepth = tokens.at( 1 ).toInt();
			else if( command == "output" )
				m_OutputFileName = tokens.at( 1 );
			else if( command == "camera" )
			{
				eye    = glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() );
				center = glm::vec3( tokens.at( 4 ).toFloat(), tokens.at( 5 ).toFloat(), tokens.at( 6 ).toFloat() );
				up     = glm::vec3( tokens.at( 7 ).toFloat(), tokens.at( 8 ).toFloat(), tokens.at( 9 ).toFloat() );
				fovy   = tokens.at( 10 ).toFloat();
			}
			else if( command == "translate" )
			{
				transform_stack.back() =  transform_stack.back() *
						glm::translate( glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ) );
			}
			else if( command == "rotate" )
			{
				transform_stack.back() =  transform_stack.back() *
						glm::rotate( tokens.at( 4 ).toFloat(), glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ) );
			}
			else if( command == "scale" )
			{
				transform_stack.back() =  transform_stack.back() *
						glm::scale( glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ) );
			}
			else if( command == "pushTransform")
				transform_stack.push_back( transform_stack.back() );
			else if( command == "popTransform" )
				transform_stack.pop_back();
			else if( command == "attenuation" )
			{
				const_att     = tokens.at( 1 ).toFloat();
				linear_att    = tokens.at( 2 ).toFloat();
				quadratic_att = tokens.at( 3 ).toFloat();
			}
			else if( command == "ambient" )
				material.Ambient = glm::vec4( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat(), 1.0f );
			else if( command == "directional" )
			{
				lights.push_back(
							new Light( glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ),
									   glm::vec3( tokens.at( 4 ).toFloat(), tokens.at( 5 ).toFloat(), tokens.at( 6 ).toFloat() ),
									   glm::vec3( const_att, linear_att, quadratic_att ), transform_stack.back(), false )
							);
			}
			else if( command == "point" )
			{
				lights.push_back(
							new Light( glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ),
									   glm::vec3( tokens.at( 4 ).toFloat(), tokens.at( 5 ).toFloat(), tokens.at( 6 ).toFloat() ),
									   glm::vec3( const_att, linear_att, quadratic_att ), transform_stack.back(), true )
							);
			}
			else if( command == "diffuse" )
				material.Diffuse  = glm::vec4( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat(), 1.0f );
			else if( command == "specular" )
				material.Specular = glm::vec4( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat(), 1.0f );
			else if( command == "emission" )
				material.Emission = glm::vec4( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat(), 1.0f );
			else if( command == "shininess" )
				material.Shininess = tokens.at( 1 ).toFloat();
			else if( command == "vertex" )
				vertices.append( glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ) );
			else if( command == "tri" )
				objects.append( new Triangle(
						vertices[ tokens.at( 1 ).toInt() ],vertices[ tokens.at( 2 ).toInt() ], vertices[ tokens.at( 3 ).toInt() ],
						transform_stack.back(), material
						) );
			else if( command == "sphere" )
				objects.append( new Sphere(
						glm::vec3( tokens.at( 1 ).toFloat(), tokens.at( 2 ).toFloat(), tokens.at( 3 ).toFloat() ), tokens.at( 4 ).toFloat(),
						material, transform_stack.back()
						) );
		}

		//
		m_Camera = Camera( eye, center, up, fovy, width, height );
		m_AmbientColor = ambient;
		m_Lights       = lights;

		m_Objects = objects;

		return true;
	}

	QRgb Scene::evaluatePixel(int i, int j) const
	{
		rt::Ray ray = m_Camera.ray( i, j );
		QList<rt::Object*> objects;
		objects.reserve( m_Objects.size() );

		rt::Hit hit;

		buildObjectList( ray, objects );

		if( closestHit( ray, objects, hit ) )
		{
			glm::vec4 color = evaluateHit( m_Camera.eye(), ray, hit, 1 );
			return qRgb( int( color.x * 255.0f ), int( color.y * 255.0f ), int( color.z * 255.0f ) );
		}

		return qRgb( 0, 0, 0 );
	}

	void Scene::buildObjectList(const Ray &ray, QList<rt::Object*> &objects) const
	{
		float min = 0.0f,max = 0.0f;
		for( int i = 0; i < m_Objects.size(); ++i )
			if( m_Objects[i]->boundingBox().intersect( ray, min, max ) )
				objects.push_back( m_Objects[i] );
	}

	bool Scene::closestHit(const Ray &ray, const QList<rt::Object*> &objects, Hit& hit) const
	{
		for( int i = 0; i < objects.size(); ++i )
		{
			Hit new_hit = objects[ i ]->closestHit( ray );
			if( new_hit.Distance < hit.Distance )
				hit = new_hit;
		}

		return !!hit.Obj;
	}

	glm::vec4 Scene::evaluateHit( const glm::vec3& eye, const Ray &ray, const Hit &hit, int depth) const
	{
		glm::vec4 light_intensity;
		glm::vec4 recursive_intensity;
		// Lighting pass
		for( int i = 0; i < m_Lights.size(); ++i )
		{
			rt::Light* l = m_Lights[i];
			rt::Ray light_ray = l->rayToLight( hit.Point, hit.Normal );

			QList<rt::Object*> objects;
			objects.reserve( m_Objects.size() );

			rt::Hit light_hit;

			buildObjectList( light_ray, objects );
			bool occluded = closestHit( light_ray, objects, light_hit );

			if( !occluded || light_hit.Distance >= l->distToLight( hit.Point ) )
				light_intensity += l->brdf( eye, hit.Point, hit.Normal, hit.Obj->material() );
		}

		if( hit.Obj->material().Specular.length() >= 0.001f && depth < m_MaxDepth )
		{
			glm::vec3 new_dir = glm::reflect( glm::vec3(ray.direction), hit.Normal );
			glm::vec3 new_origin = hit.Point + new_dir * 0.0005f;
			Ray       rec_ray( new_origin, glm::vec4( new_dir, 0.0f ) );

			QList<rt::Object*> objects;
			objects.reserve( m_Objects.size() );

			rt::Hit rec_hit;

			buildObjectList( rec_ray, objects );
			if( closestHit( rec_ray, objects, rec_hit ) )
				recursive_intensity += hit.Obj->material().Specular * evaluateHit( hit.Point, rec_ray, rec_hit, depth + 1 );
		}

		return hit.Obj->material().Ambient + hit.Obj->material().Emission + light_intensity + recursive_intensity;
	}

} // namespace rt
