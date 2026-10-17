#ifndef _APPS_BUILDER_H_
#define _APPS_BUILDER_H_

#include "common/AppPrototype.h"
#include "entitysystems/VertexBufferRenderer.h"
#include "entitysystems/CullingSystem.h"
#include "entitysystems/InstancedRenderer.h"
#include "entitysystems/MeshRenderer.h"
#include "entitysystems/InputSystem.h"

#include "components/Counter.h"

#include "DashboardMenu.h"
#include "Keyboard.h"
#include "SphereMenu.h"

enum class SelectAction {
    Paint,
    Erase,
    Warp
};

enum class PaintOption {
	FreePaint,
    SnapToGrid,
    PaintOnTarget,
};

enum class OnPaintDragOption {
    Resize, /* Paint once and then resize */
    Paint,  /* Paint on each collision */
};

enum class GridSnapOption {
    Cube,   /* Snap to grid painted on a cube */
    Sphere, /* Snap to grid painted on a sphere */
    Plane,  /* Snap to a grid plane */
    CurvedQuad /* A curved quad */
};

class BrushOptions {
};

enum class PaletteItem {
    Cube,
    Sphere,
    Cylinder,
    Mesh,
    Hexagon,
    Quad,
    Light
};

static SelectAction currentSelectAction = SelectAction::Paint;
static PaintOption currentPaintOption = PaintOption::FreePaint; // PaintOnTarget;// ::SnapToGrid;
static PaletteItem currentPaletteItem = PaletteItem::Cube;

class Builder : public AppPrototype {
private:

    CullingSystem m_cullingSystem;

    DashboardMenu m_dashboardMenu;

    MeshRenderer m_dashboardRenderer;

    InputSystem m_collisionInputSystem;

    VertexBufferRenderer m_collisionVolumeRenderer;

    InstancedRenderer m_instancedRenderer;

    MeshRenderer m_worldUIRenderer;

    MeshRenderer m_lineRenderer;

    InputSystem m_dashboardInputSystem;


#pragma mark - Paint stuff.

    hd::Vector<hd::Entity *> m_undoStack = hd::Vector<hd::Entity *>(128);

    hd::Vector<hd::Entity *> m_currentPaintSession = hd::Vector<hd::Entity *>(128);

    Quaternion m_paintItemRotation;

    Color4f m_currentHue;

    hd::ui::SphereLayout m_sphereLayout = hd::ui::SphereLayout(8, 8, 1.0f, hd_2_pi);

    hd::Entity *m_target = nullptr;

    hd::Entity *m_ghostMesh = nullptr;

    hd::Entity *m_currentPaintTarget = nullptr;

	///
    /// collision plane for prototyping ideas...
	///
    hd::Entity *m_collisionVolume = nullptr;

    hd::Entity *m_gapGrid = nullptr;

    hd::Entity *m_gridSnapGuideMesh = nullptr;

    bool m_paintSessionStarted = false;

    float m_ghostMeshRadius = 5.0f;

    float m_paintItemScale = 1.0f;

    hd::Texture *m_currentTexture = nullptr;

	///
	/// These are the targeting pointers controlled by either a thumbpad or VR touch controllers.
	///
	hd::Entity *m_leftTargetingPointer = nullptr;

	hd::Entity *m_rightTargetingPointer = nullptr;

	Collision3 m_pointerCollision;

	Collision3 m_leftPointerCollision;

	Collision3 m_rightPointerCollision;


public:
    Builder(hd::application::Client *client);
    
    ~Builder();

    void InitUI();

    void OnGridSelect(hd::Entity *element, const Collision3 &collision);

    void OnGridDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnGridRelease(hd::Entity *element, const Collision3 &collision);

    void OnGridReleaseInside(hd::Entity *element, const Collision3 &collision);



    void OnPaintItemSelect(hd::Entity *element, const Collision3 &collision);

    void OnPaintItemDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnPaintItemRelease(hd::Entity *element, const Collision3 &collision);

    void OnPaintItemReleaseInside(hd::Entity *element, const Collision3 &collision);

    void OnPaintItemLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnPaintItemLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnPaintItemLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnGeometryLookAt(const hd::Entity *element, const hd::input::Headset *headset, const Collision3 &collision);


#pragma mark - Pointer selection stuff

    void OnPointerMove(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta);

    void OnPointerSelect(hd::Entity *entity, const Collision3 &collision);

    void OnPointerDrag(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta);

    void OnPointerRelease(hd::Entity *entity, const Collision3 &collision);


#pragma mark - Menu selection

    void OnSelectPaint(hd::Entity *element, const Collision3 &collision);

    void OnSelectErase(hd::Entity *element, const Collision3 &collision);

    void OnSelectCube(hd::Entity *element, const Collision3 &collision);

    void OnSelectSphere(hd::Entity *element, const Collision3 &collision);

    void OnSelectCylinder(hd::Entity *element, const Collision3 &collision);

    void OnSelectHexagon(hd::Entity *element, const Collision3 &collision);

    void OnSelectQuad(hd::Entity *element, const Collision3 &collision);

    void OnSelectMesh(hd::Entity *element, const Collision3 &collision);

    void OnSelectWarp(hd::Entity *element, const Collision3 &collision);

    void OnSelectHue(hd::Entity *element, const Collision3 &collision);

    void OnSave(hd::Entity *element, const Collision3 &collision);

    void OnNewWorld(hd::Entity *element, const Collision3 &collision);

    void OnUndo(hd::Entity *element, const Collision3 &collision);

    void SetGridSnapOption(GridSnapOption option);

    void SetPaintDragOption(OnPaintDragOption option);

    void SetCurrentHue(const Color4f &hue);

    void SetCurrentTexture(hd::Texture *texture);


    void GridLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void GridLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void GridLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void WarpTo(hd::Entity *camera);


	void Update(hd::Entity *camera);

	void HandleInput(hd::Entity *camera);

    void DrawDepth(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void Draw(hd::Entity *camera, hd::input::EyePosition eyePosition);


protected:

    void OnErase(hd::Entity *element);

    void OnPaint();

    hd::Entity * Create(const Vec3 &location);


private:

    void RefreshGhostItem();

    void CreateCollisionPlane();

    void PaintOnCollisionPlane(hd::Entity *element, const Collision3 &selectCollision, const Collision3 &releaseCollision);

    void BeginPaintSession();

    void EndPaintSession();

    Vec3 GetPointerWorldPosition() const;
    
};

#endif
