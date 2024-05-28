#ifndef _CAMERA_H
#define _CAMERA_H

#include <ultra64.h>

#include "math/quaternion.h"
#include "math/vector3.h"
#include "math/transform.h"
#include "math/plane.h"
#include "graphics/renderstate.h"
#include "../physics/collision_quad.h"
#include "../math/boxs16.h"
#include "../math/rotated_box.h"

#define CLIPPING_PLANE_LEFT         0
#define CLIPPING_PLANE_BOTTOM       1
#define CLIPPING_PLANE_RIGHT        2
#define CLIPPING_PLANE_TOP          3
#define CLIPPING_PLANE_NEAR         4
#define CLIPPING_PLANE_FAR          5
#define MAX_CLIPPING_PLANE_COUNT    6

struct Camera {
    struct Transform transform;
    float nearPlane;
    float farPlane;
    float fov;
};

struct FrustumCullingInformation {
    struct Plane clippingPlanes[MAX_CLIPPING_PLANE_COUNT];
    short usedClippingPlaneCount;

    struct Vector3 cameraPos;
};

struct CameraMatrixInfo {
    Mtx* projectionView;
    u16 perspectiveNormalize;
    struct FrustumCullingInformation cullingInformation;
};

enum FrustumResult {
    FrustumResultOutisde,
    FrustumResultInside,
    FrustumResultBoth,
};

void frustumFromQuad(struct Vector3* cameraPos, struct CollisionQuad* quad, struct FrustumCullingInformation* out);
enum FrustumResult isOutsideFrustum(struct FrustumCullingInformation* frustum, struct BoundingBoxs16* boundingBox);
int isRotatedBoxOutsideFrustum(struct FrustumCullingInformation* frustum, struct RotatedBox* rotatedBox);
int isSphereOutsideFrustum(struct FrustumCullingInformation* frustum, struct Vector3* scaledCenter, float scaledRadius);
int isQuadOutsideFrustum(struct FrustumCullingInformation* frustum, struct CollisionQuad* quad);

void cameraInit(struct Camera* camera, float fov, float near, float far);
void cameraBuildViewMatrix(struct Camera* camera, float matrix[4][4]);
void cameraBuildProjectionMatrix(struct Camera* camera, float matrix[4][4], u16* perspectiveNorm, float aspectRatio);
int cameraSetupMatrices(struct Camera* camera, struct RenderState* renderState, float aspectRatio, Vp* viewport, int extractClippingPlanes, struct CameraMatrixInfo* output);
void cameraModifyProjectionViewForPortalGun(struct Camera* camera, struct RenderState* renderState, float newNearPlane, float aspectRatio);

int cameraApplyMatrices(struct RenderState* renderState, struct CameraMatrixInfo* matrixInfo);

float cameraClipDistance(struct Camera* camera, float distance);

int fogIntValue(float floatValue);

#endif