#include "World.h"

using namespace hd;

void physics2d::World::CreateWorld(WorldInfo worldInfo) {
    if (m_physicsWorld) return;
    
    m_worldInfo = worldInfo;
    b2Vec2 gravity(worldInfo.gravity[0], worldInfo.gravity[1]);
	m_physicsWorld = new b2World(gravity); // , worldInfo.allowSleep);
}


void physics2d::World::Update() {
    float proportion;
    float interval = 0.02f;
        
    if (m_physicsWorld == NULL) return;
        
    //this->ResetContactPoints();
        
    // Paused
    if (true) { //IsEnabled()) {
        proportion = hd::clamp(interval, 0.02f, 2.0f * m_worldInfo.fixedFrameDuration);
        
        // Try it twice...
        m_physicsWorld->Step(0.5f * proportion * m_worldInfo.simulationRateMultiplier,
                             m_worldInfo.iterationsPerUpdate,
                             m_worldInfo.iterationsPerUpdate);
        
        m_physicsWorld->Step(0.5f * proportion * m_worldInfo.simulationRateMultiplier,
                             m_worldInfo.iterationsPerUpdate,
                             m_worldInfo.iterationsPerUpdate);
    }
}


void physics2d::World::SetGravityVector(float x, float y) {
	hdAssert(m_physicsWorld);
	m_physicsWorld->SetGravity(b2Vec2(x,y));
}


b2Body * physics2d::World::CreateBody(BodyInfo bodyInfo) {
    if (bodyInfo.configuration == physics2d::VertexConfiguration::Rectangle) {
        return Rectangle(bodyInfo);
    } else if (bodyInfo.configuration == VertexConfiguration::Circle) {
        return Circle(bodyInfo);
    } else {
        return NULL;
    }
}


b2World * physics2d::World::GetPhysicsWorld() const {
    return m_physicsWorld;
}


#pragma mark - Protected

physics2d::World::World(unsigned int ID) : Component(ID),
  m_physicsWorld(NULL),
  m_worldInfo() {}


b2Body * physics2d::World::Rectangle(BodyInfo bodyInfo) {
    hdAssert(bodyInfo.configuration == physics2d::VertexConfiguration::Rectangle);
    
    if (!m_physicsWorld) {
        return NULL;
    }
    
    b2BodyDef bd;
    bd.position = b2Vec2(bodyInfo.position[0] / PHYSICS_SIZE_FACTOR,
                         bodyInfo.position[1] / PHYSICS_SIZE_FACTOR);
    bd.type = (bodyInfo.isStatic) ? b2_staticBody : b2_dynamicBody;
    b2Body *outBody = m_physicsWorld->CreateBody(&bd);
        
    b2FixtureDef fd;
    if (bodyInfo.groupIndex != 0) fd.filter.groupIndex = bodyInfo.groupIndex;
    if (bodyInfo.categoryBits > 0) fd.filter.categoryBits = bodyInfo.categoryBits;
    if (bodyInfo.maskBits > 0) fd.filter.maskBits = bodyInfo.maskBits;
    
    fd.isSensor = bodyInfo.isSensor;
    fd.friction = bodyInfo.friction;
    fd.density = bodyInfo.density;
    fd.restitution = bodyInfo.restitution;
    
    const unsigned int vertexCount = 4;
    Vec3 vertices[vertexCount];
    vertices[0].Set(bodyInfo.aabb.lower[0],
                    bodyInfo.aabb.lower[1],
                    0.0f);
    vertices[1].Set(bodyInfo.aabb.lower[0],
                    bodyInfo.aabb.upper[1],
                    0.0f);
    vertices[2].Set(bodyInfo.aabb.upper[0],
                    bodyInfo.aabb.upper[1],
                    0.0f);
    vertices[3].Set(bodyInfo.aabb.upper[0],
                    bodyInfo.aabb.lower[1],
                    0.0f);
    
    b2Vec2 polygonVertices[b2_maxPolygonVertices];
    
    // Process the vertices in CCW order:
    if (hd::IsConvexCCW(vertices, vertexCount)) {
        hd::log::i("hd2DBodyFromVertices: CCW\n");
        for (int i = 0; i < vertexCount; ++i) {
            Vec3 v = vertices[i];
            polygonVertices[i] = b2Vec2(v[0] / PHYSICS_SIZE_FACTOR,
                                        v[1] / PHYSICS_SIZE_FACTOR) - bd.position;
        }
    } else if (hd::IsConvexCW(vertices, vertexCount)) {
        hd::log::i("hd2DBodyFromVertices: NOT CCW\n");
        // reverse them so they are in convex order.
        for (int i = 0; i < vertexCount; ++i) {
            Vec3 v = vertices[i];
            polygonVertices[vertexCount - 1 - i] = b2Vec2(v[0] / PHYSICS_SIZE_FACTOR,
                                                          v[1] / PHYSICS_SIZE_FACTOR) - bd.position;
        }
    } else {
        return NULL;
    }
    
    b2PolygonShape polygon;
    polygon.Set(polygonVertices, vertexCount);
    fd.shape = &polygon;
    
    // Double check if the created centroid is broken - may
    // be the case with concave polygons.
    if (!physics2d::World::IsTriangleValid(&polygon)) {
        hd::log::i("hd2DBodyFromVertices: INVALID Triangle. Coordinates:\n");
        return NULL;
    }
    
    outBody->CreateFixture(&fd);
    
    return outBody;
}


#pragma mark - Private
b2Body * physics2d::World::Circle(BodyInfo bodyInfo) {
    hdAssert(bodyInfo.configuration == physics2d::VertexConfiguration::Circle);
    
    if (!m_physicsWorld) {
        return NULL;
    }
    
	if (bodyInfo.radius <= b2_epsilon) {
		hd::log::i("h2CirclePhysicsBody: Can't create circular physics body with a radius smaller than FLT_EPSILON\n");
		return NULL;
	}
	
	b2BodyDef bd;
	bd.position.Set(bodyInfo.position[0] / PHYSICS_SIZE_FACTOR,
                    bodyInfo.position[1] / PHYSICS_SIZE_FACTOR);
	bd.type = (bodyInfo.isStatic) ? b2_staticBody : b2_dynamicBody;
	bd.allowSleep = false;
	b2Body *outBody = m_physicsWorld->CreateBody(&bd);
	
	b2CircleShape circleShape;
	circleShape.m_radius = bodyInfo.radius / PHYSICS_SIZE_FACTOR;
	
	b2FixtureDef fd;
	fd.shape = &circleShape;
	fd.isSensor = bodyInfo.isSensor;
	fd.density = bodyInfo.density;
	fd.friction = bodyInfo.friction;
	fd.restitution = bodyInfo.restitution;
	
	if (bodyInfo.groupIndex != 0) fd.filter.groupIndex = bodyInfo.groupIndex;
	if (bodyInfo.categoryBits > 0) fd.filter.categoryBits = bodyInfo.categoryBits;
	if (bodyInfo.maskBits > 0) fd.filter.maskBits = bodyInfo.maskBits;
	
	outBody->CreateFixture(&fd);
    
    return outBody;
}


/*
 b2Body * physics2d::World::MultiShape(BodyInfo bodyInfo)
 {
 if (vertexCount <= 2)
 {
 hd::log::i("hd2DMultiShapeBodyFromVertices: Physics Body not created because the number of vertices specified (%d) was not in range", vertexCount);
 *outBody = NULL;
 return;
 }
 
 b2BodyDef bd;
 bd.position = b2Vec2(position.x, position.y);
 bd.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
 bd.allowSleep = false;
 *outBody = physicsWorld->CreateBody(&bd);
 
 hdVec2 a, b, pivot;
 b2Vec2 bodyCenter = b2Vec2(position.x, position.y);
 pivot = hdVec3toVec2(vertices[0]);
 
 // Create a triangle fan from the coordinates.
 for (int i = 1; i < vertexCount-1; ++i)
 {
 a = hdVec3toVec2(vertices[i]);
 b = hdVec3toVec2(vertices[i+1]);
 
 b2PolygonShape polygon;
 b2Vec2 triangleVertices[3];
 triangleVertices[0] = b2Vec2(pivot.x, pivot.y) - bodyCenter;
 triangleVertices[1] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
 
 b2FixtureDef fd;
 fd.shape = &polygon;
 //fd.isSensor = isSensor;
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = restitution;
 
 if (hdIsCentroidValid(&polygon) == false)
 {
 triangleVertices[1] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
 }
 
 
 
 if (hdIsTriangleValid(triangleVertices, 3))
 {
 polygon.Set(triangleVertices, 3);
 (*outBody)->CreateFixture(&fd);
 }
 }
 }*/


/*
 b2Body * physics2d::World::TriangleStrip(BodyInfo bodyInfo)
 {
 if (vertexCount <= 2)
 {
 hd::log::i("hd2DTriangleStripPhysicsBody: Physics Body not created because the number of vertices specified (%d) was not in range", vertexCount);
 *outBody = NULL;
 return;
 }
 
 b2BodyDef bd;
 bd.position = b2Vec2(position.x, position.y);
 bd.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
 *outBody = physicsWorld->CreateBody(&bd);
 
 b2Vec2 bodyCenter = b2Vec2(position.x, position.y);
 
 // Create a triangle STRIP from the coordinates.
 for (int i = 0; i < vertexCount-2; ++i)
 {
 b2PolygonShape polygon;
 b2Vec2 triangleVertices[3];
 triangleVertices[0] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
 triangleVertices[1] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(vertices[i+2].x, vertices[i+2].y) - bodyCenter;
 
 b2FixtureDef fd;
 fd.shape = &polygon;
 fd.isSensor = isSensor;
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = 0.15f;
 
 b2Vec2 centroid; centroid.Set(0,0);
 if (hdIsCentroidValid(centroid, triangleVertices, 3) == false)
 {
 triangleVertices[1] = b2Vec2(vertices[i+2].x, vertices[i+2].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
 }
 
 
 if (hdIsTriangleValid(triangleVertices, 3))
 {
 polygon.Set(triangleVertices, 3);
 (*outBody)->CreateFixture(&fd);
 }
 }
 }*/


/*
 * REFACTOR ME: Too much code replication.
 */
/*
 void hd2DMeshPhysicsBody(b2Body ** outBody,
 const hdVec3& position, void ** polygons, const unsigned int polygonCount,
 const float friction, const float density, const float restitution,
 bool isStatic, bool isSensor, int groupIndex, int categoryBits, int maskBits)
 {
 if (polygonCount == 0)
 {
 *outBody = NULL;
 return;
 }
 
 b2BodyDef bd;
 bd.position = b2Vec2(position.x, position.y);
 bd.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
 bd.allowSleep = false;
 *outBody = physicsWorld->CreateBody(&bd);
 
 
 hdVec2 a, b, pivot;
 b2Vec2 bodyCenter = b2Vec2(position.x, position.y);
 
 for (unsigned polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
 {
 hdMeshPolygon *polygon = (hdMeshPolygon *)polygons[polygonIndex];
 
 if (polygon->GetVertexCount() <= 2) continue;
 
 if (polygon->GetPolygonType() == GL_TRIANGLE_FAN)
 {
 pivot = hdVec3toVec2(polygon->GetVertices()[0]);
 
 for (uint32 i = 1; i < polygon->GetVertexCount()-1; ++i)
 {
 b2PolygonShape polygonShape;
 b2Vec2 triangleVertices[3];
 triangleVertices[0] = b2Vec2(pivot.x, pivot.y) - bodyCenter;
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i].x, polygon->GetVertices()[i].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 
 b2Vec2 centroid; centroid.Set(0,0);
 if (hdIsCentroidValid(centroid, triangleVertices, 3) == false)
 {
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i].x, polygon->GetVertices()[i].y) - bodyCenter;
 }
 
 b2FixtureDef fd;
 fd.shape = &polygonShape;
 fd.isSensor = isSensor;
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = restitution;
 
 if (hdIsTriangleValid(triangleVertices, 3))
 {
 polygonShape.Set(triangleVertices, 3);
 (*outBody)->CreateFixture(&fd);
 }
 }
 }
 else if (polygon->GetPolygonType() == GL_TRIANGLE_STRIP)
 {
 for (uint32 i = 0; i < polygon->GetVertexCount()-2; ++i)
 {
 b2PolygonShape polygonShape;
 b2Vec2 triangleVertices[3];
 triangleVertices[0] = b2Vec2(polygon->GetVertices()[i].x, polygon->GetVertices()[i].y) - bodyCenter;
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i+2].x, polygon->GetVertices()[i+2].y) - bodyCenter;
 
 b2Vec2 centroid; centroid.Set(0,0);
 if (hdIsCentroidValid(centroid, triangleVertices, 3) == false)
 {
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i+2].x, polygon->GetVertices()[i+2].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 }
 
 b2FixtureDef fd;
 fd.shape = &polygonShape;
 fd.isSensor = isSensor;
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = restitution;
 
 if (hdIsTriangleValid(triangleVertices, 3))
 {
 polygonShape.Set(triangleVertices, 3);
 (*outBody)->CreateFixture(&fd);
 }
 }
 }
 else if (polygon->GetPolygonType() == GL_TRIANGLES)
 {
 for (uint32 i = 0; i < polygon->GetVertexCount(); i += 3)
 {
 b2PolygonShape polygonShape;
 b2Vec2 triangleVertices[3];
 triangleVertices[0] = b2Vec2(polygon->GetVertices()[i].x, polygon->GetVertices()[i].y) - bodyCenter;
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i+2].x, polygon->GetVertices()[i+2].y) - bodyCenter;
 
 b2Vec2 centroid; centroid.Set(0,0);
 if (hdIsCentroidValid(centroid, triangleVertices, 3) == false)
 {
 triangleVertices[1] = b2Vec2(polygon->GetVertices()[i+2].x, polygon->GetVertices()[i+2].y) - bodyCenter;
 triangleVertices[2] = b2Vec2(polygon->GetVertices()[i+1].x, polygon->GetVertices()[i+1].y) - bodyCenter;
 }
 
 b2FixtureDef fd;
 fd.shape = &polygonShape;
 fd.isSensor = isSensor;
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = restitution;
 
 if (hdIsTriangleValid(triangleVertices, 3))
 {
 polygonShape.Set(triangleVertices, 3);
 (*outBody)->CreateFixture(&fd);
 }
 }
 }
 }
 }*/


/*
 
 b2Body * physics2d::World::SingleShape(BodyInfo bodyInfo)
 {
 if (vertexCount < 3 || vertexCount > b2_maxPolygonVertices)
 {
 hd::log::i("hd2DBodyFromVertices: Physics Body not created because the number of vertices specified (%d) was not in range", vertexCount);
 return NULL;
 }
 
 b2BodyDef bd;
 bd.position = b2Vec2(position.x, position.y); //position.x, position.y);
 bd.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
 b2Body *outBody = physicsWorld->CreateBody(&bd);
 
 b2PolygonShape polygon;
 b2Vec2 polygonVertices[b2_maxPolygonVertices];
 
 b2FixtureDef fd;
 fd.shape = &polygon;
 fd.isSensor = isSensor;
 
 if (groupIndex != 0) fd.filter.groupIndex = groupIndex;
 if (categoryBits > 0) fd.filter.categoryBits = categoryBits;
 if (maskBits > 0) fd.filter.maskBits = maskBits;
 
 fd.friction = friction;
 fd.density = density;
 fd.restitution = restitution;
 
 // Process the vertices in CCW order:
 if (hdIsConvexCCW(vertices, vertexCount))
 {
 hd::log::i("hd2DBodyFromVertices: CCW\n");
 for (int i = 0; i < vertexCount; ++i)
 {
 polygonVertices[i] = b2Vec2(vertices[i].x, vertices[i].y) - bd.position;
 }
 }
 else if (physics2d::World::IsConvexCW(vertices, vertexCount))
 {
 hd::log::i("hd2DBodyFromVertices: NOT CCW\n");
 // reverse them so they are in convex order.
 for (int i = 0; i < vertexCount; ++i)
 {
 polygonVertices[vertexCount - 1 - i] = b2Vec2(vertices[i].x, vertices[i].y) - bd.position;
 }
 }
 else
 {
 *outBody = NULL;
 return;
 }
 polygon.Set(polygonVertices, vertexCount);
 
 
 // Double check if the created centroid is broken - may
 // be the case with concave polygons.
 if (!physics2d::World::IsTriangleValid(&polygon))
 {
 hd::log::i("hd2DBodyFromVertices: INVALID Triangle. Coordinates:\n");
 for (int i = 0; i < vertexCount; ++i)
 {
 hd::log::i("\t%d: (x=%f, y=%f, z=%f)\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
 }
 
 return NULL;
 }
 
 (*outBody)->CreateFixture(&fd);
 }*/


bool physics2d::World::IsTriangleValid(b2PolygonShape * polygon) {
	return physics2d::World::IsTriangleValid(polygon->m_vertices, polygon->GetVertexCount());
}


bool physics2d::World::IsTriangleValid(b2Vec2 * vertices, unsigned int vertexCount) {
	b2Vec2 _normals[b2_maxPolygonVertices];
	b2Vec2 _vertices[b2_maxPolygonVertices];
	b2Vec2 _centroid; _centroid.Set(0,0);
	int _vertexCount;
	
	_vertexCount = vertexCount;
	
	// Copy vertices.
	for (int32 i = 0; i < _vertexCount; ++i) {
		_vertices[i] = vertices[i];
	}
	
	for (int32 i = 0; i < _vertexCount; ++i) {
		int32 i1 = i;
		int32 i2 = i + 1 < _vertexCount ? i + 1 : 0;
		b2Vec2 edge = _vertices[i2] - _vertices[i1];
		
		if (edge.LengthSquared() <= b2_epsilon * b2_epsilon) return false;
		
		_normals[i] = b2Cross(edge, 1.0f);
		_normals[i].Normalize();
	}
	
	// Compute the polygon centroid.
	if (!physics2d::World::IsCentroidValid(_centroid, vertices, vertexCount)) return false;
	
	// Compute the oriented bounding box.
	//	ComputeOBB(&_obb, _vertices, _vertexCount);
	
	// Create core polygon shape by shifting edges inward.
	// Also compute the min/max radius for CCD.
	for (int32 i = 0; i < _vertexCount; ++i) {
		int32 i1 = i - 1 >= 0 ? i - 1 : _vertexCount - 1;
		int32 i2 = i;
		
		b2Vec2 n1 = _normals[i1];
		b2Vec2 n2 = _normals[i2];
		b2Vec2 v = _vertices[i] - _centroid;;
		
		b2Vec2 d;
		d.x = b2Dot(n1, v) - b2_linearSlop;
		d.y = b2Dot(n2, v) - b2_linearSlop;
		
		// Shifting the edge inward by b2_toiSlop should
		// not cause the plane to pass the centroid.
		
		// Your shape has a radius/extent less than b2_toiSlop.
		if((d.x >= 0.0f) == false) return false;
		if ((d.y >= 0.0f) == false) return false;
	}
	return true;
}


bool physics2d::World::IsCentroidValid(b2PolygonShape * polygon) {
	b2Vec2 centroid; centroid.Set(0,0);
	return physics2d::World::IsCentroidValid(centroid, polygon);
}


bool physics2d::World::IsCentroidValid(b2Vec2& c, b2PolygonShape * polygon) {
	return physics2d::World::IsCentroidValid(c, polygon->m_vertices, polygon->GetVertexCount());
}


// HACK HACK HACK
//
// determine if the co-ordinates in the polygon will result in
// a valid b2 centroid.
//
// This code ripped from box2D
bool physics2d::World::IsCentroidValid(b2Vec2& c, b2Vec2 * vertices, unsigned int vertexCount) {
	if (vertexCount < 3) return false;
	
	float32 area = 0.0f;
	
	// pRef is the reference point for forming triangles.
	// It's location doesn't change the result (except for rounding error).
	b2Vec2 pRef(0.0f, 0.0f);
	
	const float32 inv3 = 1.0f / 3.0f;
	
	for (uint32 i = 0; i < vertexCount; ++i) {
		// Triangle vertices.
		b2Vec2 p1 = pRef;
		b2Vec2 p2 = vertices[i];
		b2Vec2 p3 = i + 1 < vertexCount ? vertices[i+1] : vertices[0];
		
		b2Vec2 e1 = p2 - p1;
		b2Vec2 e2 = p3 - p1;
		
		float32 D = b2Cross(e1, e2);
		
		float32 triangleArea = 0.5f * D;
		area += triangleArea;
		
		// Area weighted centroid
		c += triangleArea * inv3 * (p1 + p2 + p3);
	}
	
	c *= 1.0f / area;
	return (area > b2_epsilon);
}
