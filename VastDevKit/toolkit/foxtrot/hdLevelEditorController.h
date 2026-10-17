/*
 
 The level editor controller responds to item creation events on the editor area.
 
 The following events are most relevant at the moment
 - mouse down: 
	- if "Create mode": create a temporary game object
	- if "Cursor mode": select the game object currently
						under the user's cursor.
 - mouse move: draw a box between the start and end point.
	- create a temporary game object.
 - mouse up:
	- create a new totem block; if there are no overlaps
	- create a new game object.
 
 Methods:
 - Draw: draws all the game objects created thus far, called 
	as a step method by the main thread.
 
 */


#ifndef HD_LEVELEDITOR_CONTROLLER_H
#define HD_LEVELEDITOR_CONTROLLER_H

//#include <Box2D/Box2D.h>

#include <fstream>

#include <GLUT/glut.h>

#include "hdk.h"
#include "hd2DGame.h"

#include "hdLevelEditorUtilities.h"


#define TOTEM_WORLD_NEW
#define TOTEM_WORLD_TEXTFILE_EXTENSION ".hdw"
#define FILE_PATH_LEN 1024
#define LEVEL_EDITOR_MAX_SELECTED_OBJECTS 1024


// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


class b2World;
class hd2DWorld;

struct Settings;

typedef enum {
	e_interfacePaletteModeCursor = 0,
	e_interfacePaletteModeAddTotemBlock,
	e_interfacePaletteModeTotemShape,
	e_interfacePaletteModeTotemJoint,
	e_interfacePaletteModeTotemJack,
	e_interfacePaletteModeLayerPolygonQuad,
	e_interfacePaletteModeLayerPolygonQuadActualSize,
	e_interfacePaletteModeLayerPolygon,
	e_interfacePaletteModeLayerParticle,
	e_interfacePaletteModeErase,
	e_interfacePaletteModeTotemEvent,
	e_interfacePaletteModeCount
} e_interfacePaletteMode;


typedef enum {
	e_interfaceLayerModeGameLayer = 0x0001,
	e_interfaceLayerModeBackground = 0x0002,
	e_interfaceLayerModeMidground = 0x0004,
	e_interfaceLayerModeForeground = 0x0008,
} e_interfaceLayerMode;


typedef enum
{
	e_dragStyleMove = 0,
	e_dragStyleRotate,
	e_dragStyleScale,
} e_dragStyle;


typedef enum
{
	e_drawingStyleEditorFull = 0,
	e_drawingStyleEditorWireframe,
	e_drawingStylePreview,
	e_drawingStyleCount,
} e_drawingStyle;



struct Settings
{
	Settings() :
	hz(60.0f),
	
	interfacePaletteMode(e_interfacePaletteModeCursor),
	interfaceLayerMode((int)e_interfaceLayerModeGameLayer),
	
	copiedBlockCount(0),
	copiedJointCount(0),
	//selectedGameObject(NULL),
	//copiedGameObject(NULL),
	//
	
	screenLower(0.0f, 0.0f),
	screenUpper(0.0f, 0.0f),
	newTotemShapeType(e_hd2DShapeTypeRectangle),
	newTotemJointType(e_hd2DJointTypeDistance),
	newTotemBlockType(e_hd2DBlockTypeNormal),
	newTotemEventType(e_hd2DEventTypeFireGun),
	iterationCount(10),
	drawShapes(1),
	drawJoints(1),
	testPhysics(0),
	newTexturePath(0),
	deleteButtonTaps(0),
	isPerspectiveProjection(false),
	pixelWidth(500),
	pixelHeight(500),
	showCosmetics(true),
	showGround(true),
	showPhysics(true)
	{
		newPolygonTint[0] = 1.0f;
		newPolygonTint[1] = 1.0f;
		newPolygonTint[2] = 1.0f;
		newPolygonTint[3] = 1.0f;
		//copiedGameObject = NULL;
		//copiedBlocks = 0;
	}
	
	float32 hz;
	e_interfacePaletteMode interfacePaletteMode;
	int interfaceLayerMode;
	
	int copiedBlockCount;
	int copiedJointCount;
	hdGameObject* DEPRECATEDselectedGameObject;
	hdGameObject* copiedGameObject;
	hd2DBlock** copiedBlocks;
	hd2DJoint** copiedJoints;
	
	
	hdVec2 screenLower;
	hdVec2 screenUpper;
	e_hd2DShapeType newTotemShapeType;
	e_hd2DJointType newTotemJointType;
	e_hd2DBlockType newTotemBlockType;
	e_hd2DEventType newTotemEventType;
	int32 iterationCount;
	int32 drawShapes;
	int32 drawJoints;
	int32 testPhysics;
	char* newTexturePath;
	
	float newPolygonTint[4];
	int deleteButtonTaps;
	bool isPerspectiveProjection;
	
	float32 pixelWidth;
	float32 pixelHeight;
	
	bool showCosmetics;
	bool showGround;
	bool showPhysics;
	
	
	
	
};

class hdLevelEditorController
{
	 
	
public:
	hdLevelEditorController();
	~hdLevelEditorController();
	
	/**
	 * Saving/delete world files
	 */
	bool NewWorld();
	bool SaveCurrentWorld();
	bool SaveCurrentWorldTo(const char *destPath);
	bool LoadWorld(const char *path);
	bool CanSaveCurrentWorldToExistingFile();
	
	/**
	 * Input settings and functions
	 *
	 * TODO: device agnostic to support multitouch?
	 */
	void StartPan(const float32 x, const float32 y);
	void Pan(const float32 deltaX, float32 deltaY);
	void MouseDown(bool shiftKeyDown, const int x, const int y);
	void MouseDrag(const int x, const int y);
	void MouseUp(bool shiftKeyDown);
	
	void SetDragStyleRotate();
	void SetDragStyleScale();
	void SetDragStyleMove();
	void Zoom(const float32 delta);
	void TogglePerspectiveProjection();
	void SetPerspectiveProjection();
	void SetOrthoProjection();
	
	bool isPhysicsOn();
	void PhysicsOn();
	void PhysicsOff();
	
	void SetPaletteTexturePath(const char *texturePath);
	const char * GetPaletteTexturePath() const;
	
	/**
	 * The texture name is used as a lookup key for the pak file system.
	 */
	void SetPaletteTextureName(const char *textureName);
	const char * GetPaletteTextureName() const;
	
	void SetPaletteTint(const float r, const float g, const float b, const float a);
	
	void ApplyBGValuesToAllFollowingLevels();
	//void DoubleTouchDown(
	//void DoubleTouchMove(const int ax, const int ay, const int bx, const int by);
	
	
	/**
	 * Drawing
	 */
	void Draw();
	
	void DrawString(int x, int y, const char *string, ...);	
	
	void SetModifiers(const int GLUTmodifiers);
	
	void DeleteKey();
	
	void SubtractMesh();
	
	void DoUnitTest();
	
	const hd2DBlock* GetSelectedTotemBlock() const;
	
	//hdGameObject* GetSelectedGameObject() const;
	
	void AddTotemBlock(hd2DBlock* block); //hdGameObjectDef& def, e_hd2DMaterial material, e_hd2DBlockType blockType);
	
	const bool SetLevel(const uint32 levelId);
	
	const hd2DLevel* GetCurrentLevel() const;
	
	const hd2DJoint* GetCurrentJoint() const;
	
	void ToggleLayerMode(const e_interfaceLayerMode layerMode);
	
	hd2DWorld* GetCurrentTotemWorld() const;
	
	void GenerateNewEmptyWorldWithName(const char *name);
	
	void GenerateNewEmptyLevelWithName(const char *name);
	
	void GenerateNewLevel();
	
	void FinishCurrentShape();
	
	void FinishCurrentPolygon();
	
	void ReverseCCWVertices(hdVec3 *vertices, const int vertexCount);
	
	void CopySelectedObjects();
	
	void SelectNone();
	
	void SelectAll();
	
	void PasteObjectsExternal(const hdGameObject **objects, const unsigned int itemCount);
	
	void PasteCopiedObjects();
	
	const hdGameObject ** GetSelectedObjects();
	
	void CopyCurrentLevel();
	
	void PasteCopiedLevel();
	
	void ApplyCurrentTextureToSelected();
	
	void RepairSelectedShape();
	
	void DumpUniqueTextureList();
	
	void Resize(float32 w, float32 h);
	
	hdTypedefList<hdGameObject*, 1024>* m_selectedGameObjects;
	
	hdTypedefList<hdGameObject*, 1024>* m_copiedGameObjects;
	
    void GenerateBlocksFromNoise();

	
	void SetNewBlockShapeType(e_hd2DShapeType shapeType);
	
	void SetNewBlockMaterial(hd2DMaterial * material);
	
	void SetNewBlockType(e_hd2DBlockType blockType);
	
	void SetNewJointType(e_hd2DJointType jointType);
	
	// Selected game object
	const uint32 GetSelectedGameObjectsCount() const;
	
	const hdGameObject * GetSelectedGameObjectSingle() const;
	
	const hdTypedefList<hdGameObject *, 1024> * GetSelectedGameObjects() const;
	
	hdGameObject * GetSelectedGameObjectAtIndex(unsigned int index) const;
	
	bool SelectedItemsContainsType(const e_hd2DType type) const; 
	
	Settings settings;
	
	


	
private:
	
	void SetDragStyle(e_dragStyle style);
	
	void SetupProjection();
	
	void DrawModeInfo();
	
	void DrawExperimentalModelObjects();
	
	void ScaleSelectedObjects();
	
	void RotateSelectedObjects();
	
	void MoveSelectedObjects();
	
	void SelectSingleGameObject();
	
	void SelectMultipleGameObjects();
	
	void AddBlockAtClickPointToSelection();
	
	const bool SetCurrentLevel(const hd2DLevel* level);
	
	void AddNewBlock();
	
	void AddNewJoint();
	
	void AddNewEvent();
	
	hd2DBlock* CopyBlockToLevel(hd2DBlock *block);
	
	hd2DJoint* CopyJointToLevel(hd2DJoint *srcJoint);
	
	void InitPhysics();
	
	const bool BlockPassesFilter(const hd2DBlock *block) const;
	
	const bool CanSelectGameObject(const hdGameObject *object) const;
	
	hdVec2 ConvertScreenToWorld(const float32 x, const float32 y);
	
	hdVec2 ConvertSizeToWorld(const float32 x, const float32 y); 
	
	
	int32 m_textLine;
	
	// Bookkeeping
	bool m_canSaveCurrentWorldToExistingFile;
	char m_currentWorldPath[FILE_PATH_LEN];
	hd2DMaterial *m_newTotemMaterial;
	char m_newTotemMaterialName[HD_2D_MATERIAL_NAME_LENGTH];
	
	// View port
	float32 m_viewZoom;
	hdVec2 m_viewCenter;
	float32 m_xExtents, m_yExtents;
	hdVec2 m_currentPan;
	hdVec2 m_prevPan;
	
	// Interaction
	e_dragStyle m_dragStyle;
	e_drawingStyle m_drawingStyle;
	
	// box 2d stuff
	b2AABB m_physicsWorldAABB;
	b2World* m_physicsWorld;
	
	// Target viewports.
	hdVec2 m_targetScreenArea; 
	hdVec2 m_targetScreenPosition;
	
	// Interaction
	bool m_dragging;
	hdVec2 m_startClickPoint;
	hdVec2 m_previousMouseDragPoint;
	hdVec2 m_currentMouseDragPoint;
	char m_currentPaletteTexturePath[FILE_PATH_LEN];
	char m_currentPaletteTextureName[FILE_PATH_LEN];
	
	// gameworld stuff
	hdOrthographicProjection *m_projection;
	
	hdGameWorld *m_gameWorld;
	
	hd2DLevel * m_currentLevel;
	
	hd2DWorld * m_hd2DWorld;
	
	//totemShape* m_currentShape;
	
	int m_nextTotemIndex;
	
	b2MouseJoint* m_mouseJoint;
	
	hdPolygon* m_currentLayerPolygon;
	
	hd2DBlock* m_currentBlockShape;
	
	hd2DJoint *m_currentJoint;
	
	hd2DEvent *m_currentEvent;
	
	// list of unique textures of all worlds loaded.
	std::map<string, string> uniqueTextures;
	
	hdColor4f m_paletteTint;

	
	// TESTING AND PROTOTYPING AFTER THIS POINT
    
    hdTexture * m_noiseTexture;
};


inline const hd2DBlock* hdLevelEditorController::GetSelectedTotemBlock() const
{
	if (settings.DEPRECATEDselectedGameObject == NULL) return NULL;
	if (settings.DEPRECATEDselectedGameObject->GetUserType() != (int)e_hd2DTypeBlock) return NULL;
	return (hd2DBlock *)settings.DEPRECATEDselectedGameObject;
}


inline const hd2DLevel* hdLevelEditorController::GetCurrentLevel() const
{
	return m_currentLevel;
}


inline const hd2DJoint* hdLevelEditorController::GetCurrentJoint() const
{
	return m_currentJoint;
}


inline hd2DWorld* hdLevelEditorController::GetCurrentTotemWorld() const
{
	return m_hd2DWorld;
}


#endif