#pragma once

// Common includes
#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
// Physics includes
#include <Physics/Collide/hkpCollide.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>
#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintData.h>
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintConstructionKit.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#ifdef _DEBUG
	// Visual Debugger includes
	#include <Common/Visualize/hkVisualDebugger.h>
	#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif
#include <iostream>
using namespace std;
// --------------------------- Error Reporting ------------------------------ //
// Havok error reporting function
static inline void HK_CALL errorReport(const char* msg, void* userContext){
	std::cout << msg << std::endl;
}


//#include <Common/Base/keycode.cxx>
//
//// we're not using any product apart from Havok Physics.
//	#undef HK_FEATURE_PRODUCT_AI
//	#undef HK_FEATURE_PRODUCT_ANIMATION
//	#undef HK_FEATURE_PRODUCT_CLOTH
//	#undef HK_FEATURE_PRODUCT_DESTRUCTION
//	#undef HK_FEATURE_PRODUCT_BEHAVIOR
//
//// Also we're not using any serialization/versioning so we don't need any of these.
//#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
//#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
//#define HK_EXCLUDE_FEATURE_MemoryTracker
//
//#include <Common/Base/Config/hkProductFeatures.cxx>