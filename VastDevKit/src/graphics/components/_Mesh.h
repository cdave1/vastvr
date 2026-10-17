/*
 * A mesh is a composite polygon.
 *
 * In the 2D context of the hdEngine, a mesh has a set of boundary
 * contours and a set of internal polygons.  The mesh is an implementation of
 * the composite pattern.  The subclassed polygon is responsible for AABB
 * calculation and modifying the internal contours and polygons.
 *
 * The internal polygons are used to render the face, while
 * the boundary is used to render the extrusion.
 *
 * NOTE: the boundary property can be used to generate 2D
 * physics outlines from arbitrary meshes. We simply need
 * to render the triangles of the mesh with a z value of 0,
 * and then generate a boundary polygon.
 *
 * The mesh allows us to present any arbitrary shape, so in a way,
 * it is no longer absolutely neccessary to retain shape information
 * apart from cases where physics approximation is required (e.g  where
 * a 10 point star might be better represented by a cylindrical physics
 * object).
 */

#ifndef _HD_GRAPHICS_COMPONENTS_MESH_H_
#define _HD_MESH_H_

#include "hdGraphicsLibrary/hdPolygon.h"
#include "hdGraphicsLibrary/hdMeshPolygon.h"

#ifdef LEVEL_EDITOR
#include "glut/glut.h"
#else
#include "glu.h"
//#define GLenum unsigned int
//#define GLdouble double
#endif


#define HD_MESH_USER_TYPE 0xF00D

#define HD_MESH_CONTOUR_COUNT 64
#define HD_MESH_POLYGON_COUNT 1024
#define MESH_TEMP_VERTEX_COUNT 128

typedef GLdouble gluVertex_t[3];


class hdMeshPolygon;

class hdMesh : public hdPolygon
{
public:
	hdMesh();
	
	hdMesh(hdGameWorld* gameWorld);
	
	~hdMesh();
	
	void Init(hdGameWorld *gameWorld);
	
	void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis);
	
	void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);
	
	void MoveBy(const hdVec3& center);
	
	void DidModifyVertices();
	
	void ResetVertices();

	void GenerateTesselation();
	
	void SubtractMesh(const hdMesh *mesh);
	
	void DebugDraw();
	
	hdSerializablePointerList<hdMeshPolygon, HD_MESH_POLYGON_COUNT> * GetMeshPolygons();
	
	hdSerializablePointerList<hdPolygon, HD_MESH_CONTOUR_COUNT> * GetContourPolygons();
	
	void ResetTextureCoords();
	
	void SetTextureRepeatX(const float repeatX);
	
	void SetTextureRepeatY(const float repeatY);
	
protected:

//#ifdef LEVEL_EDITOR
	
	static void HDCALLBACK hdglErrorTess(GLenum errCode, hdMesh * mesh);
	
	static void HDCALLBACK hdglVertexTess(void * data, hdMesh * mesh);
	
	static void HDCALLBACK hdglCombineTess(GLdouble coords[3], GLdouble * vertex_data[4], GLfloat weight[4], GLdouble ** outData, hdMesh * mesh);
	
	static void HDCALLBACK hdglBeginTess(GLenum type, hdMesh * mesh);
	
	static void HDCALLBACK hdglEndTess(hdMesh * mesh);
	
	/* When GLU_TESS_BOUNDARY_ONLY is true, the following funcs are called: */
	
	static void HDCALLBACK hdglBoundaryVertexTess(void * data, hdMesh * mesh);
	
	static void HDCALLBACK hdglBoundaryBeginTess(GLenum type, hdMesh * mesh);
	
	static void HDCALLBACK hdglBoundaryEndTess(hdMesh * mesh);
	
//#endif
	
	const bool AddPolygonMesh(hdMeshPolygon *polygonMesh);
	
	void RemovePolygonMeshAtIndex(const uint32 index);
	
	const float * Combine(const float x, const float y, const float z);
	
	void Begin(GLenum meshType);
	
	void AddPointInternal(const float x, const float y, const float z);
	
	void End();
	
	void BeginContour();
	
	void AddContourPoint(const float x, const float y, const float z);
	
	void EndContour();
	
	hdMeshPolygon *m_currentMeshPolygon;
	
	hdPolygon *m_currentContourPolygon;
	
	hdSerializablePointerList<hdMeshPolygon, HD_MESH_POLYGON_COUNT> * m_polygons;
	
	hdSerializablePointerList<hdPolygon, HD_MESH_CONTOUR_COUNT> * m_contours;
	
	gluVertex_t * m_gluVectors;
	
private:
	
	void SubtractMeshFromBoundary(const hdMesh *mesh);
	
	void SubtractMeshFromPolygons(const hdMesh *mesh);
	
	
	void GenerateBoundary();
	
	void GenerateTesselationPolygons();
	
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdPolygon>(*this);
		ar & m_polygons;
		if (version >= 2)
		{
			ar & m_contours;
		}
	}
	
};

BOOST_CLASS_VERSION(hdMesh, 2)


inline hdSerializablePointerList<hdMeshPolygon, HD_MESH_POLYGON_COUNT> * hdMesh::GetMeshPolygons()
{
	return m_polygons;
}


inline hdSerializablePointerList<hdPolygon, HD_MESH_CONTOUR_COUNT> * hdMesh::GetContourPolygons()
{
	return m_contours;
}


#endif