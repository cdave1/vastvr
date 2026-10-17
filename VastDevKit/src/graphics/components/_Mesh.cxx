/*
 *  hdMesh.cpp
 *  LevelEditor
 *
 *  Created by david on 3/07/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdMesh.h"


#if defined __APPLE_CC__ && __APPLE_CC__ < 5465
typedef GLvoid (*GLUTesselatorFunction) (...);
#elif defined WIN32 && !defined __CYGWIN__
typedef GLvoid (HDCALLBACK *GLUTesselatorFunction) ();
#else
typedef GLvoid (*GLUTesselatorFunction) ();
#endif


void HDCALLBACK hdMesh::hdglErrorTess(GLenum errCode, hdMesh * mesh)
{
    //mesh->Error(errCode);
	printf("Mesh error: %d %s\n", errCode, gluErrorString(errCode));
}


void HDCALLBACK hdMesh::hdglVertexTess(void * data, hdMesh * polygon)
{
	GLdouble *ptr;
	ptr = (GLdouble *) data;
	printf("Adding mesh vertex: %3.5f, %3.5f, %3.5f\n", ptr[0], ptr[1], ptr[2]);//vertex->x, vertex->y, vertex->z);
    polygon->AddPointInternal(ptr[0], ptr[1], ptr[2]);
}


void HDCALLBACK hdMesh::hdglCombineTess(GLdouble coords[3], GLdouble * vertex_data[4], GLfloat weight[4], GLdouble ** outData, hdMesh * mesh)
{
	printf("AAA: Combining! %3.5f, %3.5f, %3.5f\n", coords[0], coords[1], coords[2]);
    //const float * vertex = static_cast<const float *>(coords);
	//hdVec3 *vertex = (hdVec3 *)malloc(sizeof(hdVec3));
	//vertex->Set(coords[0], coords[1], coords[2]);
    //*outData = (float *)mesh->Combine(vertex->x, vertex->y, vertex->z);
	
	
	GLdouble *vertex;
	
	vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));
	//
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	
	//mesh->Combine(coords[0], coords[1], coords[2]);
	
	//for (int i = 3; i < 6; i++)
	//{
	//	vertex[i] = weight[0] * vertex_data[0][i] +
	//		weight[1] * vertex_data[1][i] +
	//		weight[2] * vertex_data[2][i] +
	//		weight[3] * vertex_data[3][i];
	//}
	*outData = vertex; //(float *)mesh->Combine(vertex[0], vertex[1], vertex[2]);
	
}


void HDCALLBACK hdMesh::hdglBeginTess(GLenum type, hdMesh * mesh)
{
    mesh->Begin(type);
}


void HDCALLBACK hdMesh::hdglEndTess(hdMesh * mesh)
{
    mesh->End();
}



void HDCALLBACK hdMesh::hdglBoundaryBeginTess(GLenum type, hdMesh * mesh)
{
	mesh->BeginContour();
}


void HDCALLBACK hdMesh::hdglBoundaryVertexTess(void * data, hdMesh * mesh)
{
	GLdouble *ptr;
	ptr = (GLdouble *) data;
    mesh->AddContourPoint(ptr[0], ptr[1], ptr[2]);
}


void HDCALLBACK hdMesh::hdglBoundaryEndTess(hdMesh * mesh)
{
	mesh->EndContour();
}





hdMesh::hdMesh(hdGameWorld* gameWorld) : hdPolygon(gameWorld)
{
	m_polygons = new hdSerializablePointerList<hdMeshPolygon, HD_MESH_POLYGON_COUNT>();
	m_contours = new hdSerializablePointerList<hdPolygon, HD_MESH_CONTOUR_COUNT>();
	m_currentContourPolygon = NULL;
	m_currentMeshPolygon = NULL;
	m_gluVectors = NULL;
	m_userType = HD_MESH_USER_TYPE;
}


hdMesh::hdMesh() : hdPolygon(NULL)
{
	m_polygons = new hdSerializablePointerList<hdMeshPolygon, HD_MESH_POLYGON_COUNT>();
	m_contours = new hdSerializablePointerList<hdPolygon, HD_MESH_CONTOUR_COUNT>();
	m_currentContourPolygon = NULL;
	m_currentMeshPolygon = NULL;
	m_gluVectors = NULL;
	m_userType = HD_MESH_USER_TYPE;
}


hdMesh::~hdMesh()
{
	hdDeleteSafely(m_polygons);
	hdDeleteSafely(m_contours);
	hdFreeSafely(m_gluVectors);
}


void hdMesh::Init(hdGameWorld *gameWorld)
{
	((hdPolygon *)this)->Init(gameWorld);
}


void hdMesh::Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis)
{
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						-rotationAxis);
	
	hdRotateVertices(m_vertices->GetItems(), 
					 m_vertices->GetItemCount(), 
					 rotationVector);
	
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						rotationAxis);
	
	this->ResetAABB();
	
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->Rotate(rotationVector, rotationAxis);
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->Rotate(rotationVector, rotationAxis);
	}
}


void hdMesh::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						-scaleAxis);
	
	hdScaleVertices(m_vertices->GetItems(), 
					m_vertices->GetItemCount(), 
					scaleVector);
	
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						scaleAxis);
	
	this->ResetAABB();
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->Scale(scaleVector, scaleAxis);
	}
	
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->Scale(scaleVector, scaleAxis);
	}
}


void hdMesh::MoveBy(const hdVec3& delta)
{
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), delta);
	this->GetTransform().translation = this->GetWorldCenter() + delta;
	this->ResetAABB();
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->MoveBy(delta);
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->MoveBy(delta);
	}
}


void hdMesh::DidModifyVertices()
{
	this->SetStartingVerticesToCurrent();
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->SetStartingVerticesToCurrent();
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->SetStartingVerticesToCurrent();
	}
	
}


void hdMesh::ResetVertices()
{
	((hdPolygon *)this)->ResetVertices();
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->ResetVertices();
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->ResetVertices();
	}
}


const bool hdMesh::AddPolygonMesh(hdMeshPolygon *polygonMesh)
{
	if (!m_polygons) return false;
	
	return m_polygons->Add(polygonMesh);
}


void hdMesh::RemovePolygonMeshAtIndex(const uint32 index)
{
	m_polygons->RemoveAtIndex(index);
}


void hdMesh::GenerateTesselation()
{
//#ifdef LEVEL_EDITOR
	this->GenerateBoundary();
	this->GenerateTesselationPolygons();
//#endif
}


void hdMesh::GenerateBoundary()
{
//#ifdef LEVEL_EDITOR
	m_contours->RemoveAll();
	
	gluVertex_t *initialBoundaryVertices = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * this->GetVertexCount());
	unsigned int vertexCount = this->GetVertexCount();
	for (unsigned i = 0; i < this->GetVertexCount(); ++i)
	{
		hdVec3 vertex = this->GetVertices()[i];
		initialBoundaryVertices[i][0] = vertex.x;
		initialBoundaryVertices[i][1] = vertex.y;
		initialBoundaryVertices[i][2] = vertex.z;
	}
	
	GLUtesselator * tobj = gluNewTess();
	
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)&hdglBoundaryBeginTess);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)&hdglBoundaryVertexTess);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)&hdglCombineTess);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)&hdglBoundaryEndTess);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)&hdglErrorTess);
	
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GLU_TRUE);
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    
	/* First contour is this mesh */
	gluTessNormal(tobj, 0.0f, 0.0f, -1.0f);
    gluTessBeginPolygon(tobj, this);
	gluTessBeginContour(tobj);
	
	for (unsigned i = 0; i < vertexCount; ++i)
	{
		gluTessVertex(tobj, (GLdouble *)initialBoundaryVertices[i], (GLvoid *)initialBoundaryVertices[i]);
	}
	
	gluTessEndContour(tobj);
	
    gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);
	
	free(initialBoundaryVertices);
//#endif
}


void hdMesh::GenerateTesselationPolygons()
{
//#ifdef LEVEL_EDITOR
	gluVertex_t ** contourVertexList = (gluVertex_t **)calloc(1, sizeof(gluVertex_t *) * m_contours->GetItemCount());
	
	GLUtesselator * tobj = gluNewTess();
	
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)&hdglBeginTess);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)&hdglVertexTess);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)&hdglCombineTess);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)&hdglEndTess);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)&hdglErrorTess);
	
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    gluTessNormal(tobj, 0.0f, 0.0f, 1.0f);
    gluTessBeginPolygon(tobj, this);
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		gluTessBeginContour(tobj);
		hdPolygon *contour = m_contours->GetItems()[i];
		contourVertexList[i] = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * contour->GetVertexCount());
		for (unsigned j = 0; j < contour->GetVertexCount(); ++j)
		{
			hdVec3 vertex = contour->GetVertices()[j];
			printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
			contourVertexList[i][j][0] = vertex.x;
			contourVertexList[i][j][1] = vertex.y;
			contourVertexList[i][j][2] = vertex.z;
			
			gluTessVertex(tobj, (GLdouble *)contourVertexList[i][j], (GLvoid *)contourVertexList[i][j]);
		}
		gluTessEndContour(tobj);
	}
	
    gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);
	
	this->ResetAABB();
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->ResetTextureCoords(this->GetAABB());
	}
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		free(contourVertexList[i]);
	}
	free(contourVertexList);
//#endif
}


void hdMesh::SubtractMesh(const hdMesh *mesh)
{
	this->SubtractMeshFromPolygons(mesh);
	this->SubtractMeshFromBoundary(mesh);
}


void hdMesh::SubtractMeshFromBoundary(const hdMesh *mesh)
{
//#ifdef LEVEL_EDITOR
	
	GLUtesselator * tobj = gluNewTess();
	
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)&hdglBoundaryBeginTess);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)&hdglBoundaryVertexTess);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)&hdglCombineTess);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)&hdglBoundaryEndTess);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)&hdglErrorTess);
	
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
	gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GLU_TRUE);
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    
	/* First contour is this mesh */
	gluTessNormal(tobj, 0.0f, 0.0f, -1.0f);
    gluTessBeginPolygon(tobj, this);
	
	gluVertex_t ** contourVertexList = (gluVertex_t **)calloc(1, sizeof(gluVertex_t *) * m_contours->GetItemCount());
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		gluTessBeginContour(tobj);
		hdPolygon *contour = m_contours->GetItems()[i];
		contourVertexList[i] = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * contour->GetVertexCount());
		for (unsigned j = 0; j < contour->GetVertexCount(); ++j)
		{
			hdVec3 vertex = contour->GetVertices()[j];
			printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
			contourVertexList[i][j][0] = vertex.x;
			contourVertexList[i][j][1] = vertex.y;
			contourVertexList[i][j][2] = vertex.z;
			
			gluTessVertex(tobj, (GLdouble *)contourVertexList[i][j], (GLvoid *)contourVertexList[i][j]);
		}
		gluTessEndContour(tobj);
	}
	m_contours->RemoveAll();
	
	/* final contour is the mesh we want to subtract from the first */
	gluTessBeginContour(tobj);
	
	gluVertex_t * meshVertices = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * mesh->GetVertexCount());
	
	for (unsigned i = mesh->GetVertexCount() - 1; i > 0; --i)
	{
		hdVec3 vertex = ((hdMesh *)mesh)->GetVertices()[i];
		printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
		meshVertices[i][0] = vertex.x;
		meshVertices[i][1] = vertex.y;
		meshVertices[i][2] = vertex.z;
		
		gluTessVertex(tobj, (GLdouble *)meshVertices[i], (GLvoid *)meshVertices[i]);
	}
	
	
	gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);
	
	free(meshVertices);
//#endif
}


void hdMesh::SubtractMeshFromPolygons(const hdMesh *mesh)
{
//#ifdef LEVEL_EDITOR
	m_polygons->RemoveAll();
	
	if (m_gluVectors)
	{
		//free(m_gluVectors);
	}
	
	printf("SubtractMesh\n");
	
	//m_gluVectors = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * this->GetVertexCount());
	
	GLUtesselator * tobj = gluNewTess();
	
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)&hdglBeginTess);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)&hdglVertexTess);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)&hdglCombineTess);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)&hdglEndTess);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)&hdglErrorTess);
	
    gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    
	/* First contour is this mesh */
	gluTessNormal(tobj, 0.0f, 0.0f, -1.0f);
    gluTessBeginPolygon(tobj, this);
	
	gluVertex_t ** contourVertexList = (gluVertex_t **)calloc(1, sizeof(gluVertex_t *) * m_contours->GetItemCount());
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		gluTessBeginContour(tobj);
		hdPolygon *contour = m_contours->GetItems()[i];
		contourVertexList[i] = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * contour->GetVertexCount());
		for (unsigned j = 0; j < contour->GetVertexCount(); ++j)
		{
			hdVec3 vertex = contour->GetVertices()[j];
			printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
			contourVertexList[i][j][0] = vertex.x;
			contourVertexList[i][j][1] = vertex.y;
			contourVertexList[i][j][2] = vertex.z;
			
			gluTessVertex(tobj, (GLdouble *)contourVertexList[i][j], (GLvoid *)contourVertexList[i][j]);
		}
		gluTessEndContour(tobj);
	}
	
	/* second contour is the mesh we want to subtract from the first */
	gluTessBeginContour(tobj);
	
	gluVertex_t * meshVertices = (gluVertex_t *)calloc(1, sizeof(gluVertex_t) * mesh->GetVertexCount());
	
	for (unsigned i = mesh->GetVertexCount() - 1; i > 0; --i)
	{
		hdVec3 vertex = ((hdMesh *)mesh)->GetVertices()[i];
		printf("gluTessVertex: %3.5f, %3.5f, %3.5f\n", vertex.x, vertex.y, vertex.z);
		meshVertices[i][0] = vertex.x;
		meshVertices[i][1] = vertex.y;
		meshVertices[i][2] = vertex.z;
		
		gluTessVertex(tobj, (GLdouble *)meshVertices[i], (GLvoid *)meshVertices[i]);
	}
	
	
	gluTessEndContour(tobj);
	
    gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);
	
	this->ResetAABB();
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->ResetTextureCoords(this->GetAABB());
	}
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		free(contourVertexList[i]);
	}
	free(contourVertexList);
	free(meshVertices);
//#endif
}


void hdMesh::DebugDraw()
{
#ifdef LEVEL_EDITOR
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		//glColor4f(0.0f, 0.0f, hdClamp((float)i/(float)m_contours->GetItemCount(), 0.0f, 1.0f), 1.0f);
		glBegin(GL_LINE_STRIP);
		
		for (unsigned j = 0; j < contour->GetVertexCount(); ++j)
		{
			glVertex3f(contour->GetVertices()[j].x,
					   contour->GetVertices()[j].y,
					   contour->GetVertices()[j].z);
		}
		
		glEnd();
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		glColor4f(0.0f, 0.0f, hdClamp((float)i/(float)m_polygons->GetItemCount(), 0.0f, 1.0f), 1.0f);
			glBegin(poly->GetPolygonType());
		//glBegin(GL_POLYGON);
		
		for (unsigned j = 0; j < poly->GetVertexCount(); ++j)
		{
			glVertex3f(poly->GetVertices()[j].x,
					   poly->GetVertices()[j].y,
					   poly->GetVertices()[j].z);
		}
		
		glEnd();
	}
#endif
}


void hdMesh::ResetTextureCoords()
{
	((hdPolygon *)this)->ResetTextureCoords();
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->ResetTextureCoords(this->GetAABB());
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->ResetTextureCoords(this->GetAABB());
	}
}


void hdMesh::SetTextureRepeatX(const float repeatX)
{
	((hdPolygon *)this)->SetTextureRepeatX(repeatX);
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->SetTextureRepeatX(repeatX);
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->SetTextureRepeatX(repeatX);
	}
}


void hdMesh::SetTextureRepeatY(const float repeatY)
{
	((hdPolygon *)this)->SetTextureRepeatY(repeatY);
	
	for (unsigned i = 0; i < m_contours->GetItemCount(); ++i)
	{
		hdPolygon *contour = m_contours->GetItems()[i];
		contour->SetTextureRepeatY(repeatY);
	}
	
	for (unsigned i = 0; i < m_polygons->GetItemCount(); ++i)
	{
		hdMeshPolygon *poly = m_polygons->GetItems()[i];
		poly->SetTextureRepeatY(repeatY);
	}
}


const float * hdMesh::Combine(const float x, const float y, const float z)
{
	return NULL;
}


void hdMesh::Begin(GLenum meshType)
{
	printf("Begin mesh\n");
    m_currentMeshPolygon = new hdMeshPolygon();
	//m_currentMeshPolygon->Init(this->GetWorld());
	m_currentMeshPolygon->SetPolygonType(meshType);
}


void hdMesh::AddPointInternal(const float x, const float y, const float z)
{
    m_currentMeshPolygon->AddPoint(x, y, z);
}


void hdMesh::End()
{
	m_currentMeshPolygon->ResetAABB();
    m_polygons->Add(m_currentMeshPolygon);
}


void hdMesh::BeginContour()
{
	m_currentContourPolygon = new hdPolygon();
}


void hdMesh::AddContourPoint(const float x, const float y, const float z)
{
	m_currentContourPolygon->AddPoint(x, y, z);
}


void hdMesh::EndContour()
{
	m_currentContourPolygon->ResetAABB();
	m_contours->Add(m_currentContourPolygon);
}

