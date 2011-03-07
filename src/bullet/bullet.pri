INCLUDEPATH += bullet/src

# BulletSoftBody

SOURCES += bullet/src/BulletSoftBody/btSoftBody.cpp \
  bullet/src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
  bullet/src/BulletSoftBody/btSoftBodyHelpers.cpp \
  bullet/src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
  bullet/src/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
  bullet/src/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
  bullet/src/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
  bullet/src/BulletSoftBody/btDefaultSoftBodySolver.cpp

HEADERS += bullet/src/BulletSoftBody/btSoftBody.h \
  bullet/src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h \
  bullet/src/BulletSoftBody/btSoftBodyHelpers.h \
  bullet/src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h \
  bullet/src/BulletSoftBody/btSoftRigidCollisionAlgorithm.h \
  bullet/src/BulletSoftBody/btSoftRigidDynamicsWorld.h \
  bullet/src/BulletSoftBody/btSoftSoftCollisionAlgorithm.h \
  bullet/src/BulletSoftBody/btSparseSDF.h \
  bullet/src/BulletSoftBody/btSoftBodySolvers.h \
  bullet/src/BulletSoftBody/btDefaultSoftBodySolver.h \
  bullet/src/BulletSoftBody/btSoftBodySolverVertexBuffer.h


# BulletCollision

SOURCES += bullet/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btDbvt.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
  bullet/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btGhostObject.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/CollisionDispatch/btUnionFind.cpp \
  bullet/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
  bullet/src/BulletCollision/CollisionShapes/btBoxShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btBox2dShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btCollisionShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btCompoundShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConcaveShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConeShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvexShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btCylinderShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btEmptyShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
  bullet/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btShapeHull.cpp \
  bullet/src/BulletCollision/CollisionShapes/btSphereShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
  bullet/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
  bullet/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
  bullet/src/BulletCollision/Gimpact/btContactProcessing.cpp \
  bullet/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
  bullet/src/BulletCollision/Gimpact/btGImpactBvh.cpp \
  bullet/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
  bullet/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
  bullet/src/BulletCollision/Gimpact/btGImpactShape.cpp \
  bullet/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
  bullet/src/BulletCollision/Gimpact/gim_box_set.cpp \
  bullet/src/BulletCollision/Gimpact/gim_contact.cpp \
  bullet/src/BulletCollision/Gimpact/gim_memory.cpp \
  bullet/src/BulletCollision/Gimpact/gim_tri_collision.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
  bullet/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp

HEADERS += bullet/src/btBulletCollisionCommon.h \
  bullet/src/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
  bullet/src/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
  bullet/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
  bullet/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
  bullet/src/BulletCollision/BroadphaseCollision/btDbvt.h \
  bullet/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
  bullet/src/BulletCollision/BroadphaseCollision/btDispatcher.h \
  bullet/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h \
  bullet/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
  bullet/src/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
  bullet/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
  bullet/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
  bullet/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionObject.h \
  bullet/src/BulletCollision/CollisionDispatch/btCollisionWorld.h \
  bullet/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
  bullet/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btGhostObject.h \
  bullet/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
  bullet/src/BulletCollision/CollisionDispatch/btManifoldResult.h \
  bullet/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
  bullet/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
  bullet/src/BulletCollision/CollisionDispatch/btUnionFind.h \
  bullet/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
  bullet/src/BulletCollision/CollisionShapes/btBoxShape.h \
  bullet/src/BulletCollision/CollisionShapes/btBox2dShape.h \
  bullet/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
  bullet/src/BulletCollision/CollisionShapes/btCapsuleShape.h \
  bullet/src/BulletCollision/CollisionShapes/btCollisionMargin.h \
  bullet/src/BulletCollision/CollisionShapes/btCollisionShape.h \
  bullet/src/BulletCollision/CollisionShapes/btCompoundShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConcaveShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConeShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvexHullShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvexInternalShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvexShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvex2dShape.h \
  bullet/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
  bullet/src/BulletCollision/CollisionShapes/btCylinderShape.h \
  bullet/src/BulletCollision/CollisionShapes/btEmptyShape.h \
  bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
  bullet/src/BulletCollision/CollisionShapes/btMaterial.h \
  bullet/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
  bullet/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
  bullet/src/BulletCollision/CollisionShapes/btMultiSphereShape.h \
  bullet/src/BulletCollision/CollisionShapes/btOptimizedBvh.h \
  bullet/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
  bullet/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
  bullet/src/BulletCollision/CollisionShapes/btShapeHull.h \
  bullet/src/BulletCollision/CollisionShapes/btSphereShape.h \
  bullet/src/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
  bullet/src/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
  bullet/src/BulletCollision/CollisionShapes/btTetrahedronShape.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleBuffer.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleCallback.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleMesh.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
  bullet/src/BulletCollision/CollisionShapes/btTriangleShape.h \
  bullet/src/BulletCollision/CollisionShapes/btUniformScalingShape.h \
  bullet/src/BulletCollision/Gimpact/btBoxCollision.h \
  bullet/src/BulletCollision/Gimpact/btClipPolygon.h \
  bullet/src/BulletCollision/Gimpact/btContactProcessing.h \
  bullet/src/BulletCollision/Gimpact/btGenericPoolAllocator.h \
  bullet/src/BulletCollision/Gimpact/btGeometryOperations.h \
  bullet/src/BulletCollision/Gimpact/btGImpactBvh.h \
  bullet/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
  bullet/src/BulletCollision/Gimpact/btGImpactMassUtil.h \
  bullet/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
  bullet/src/BulletCollision/Gimpact/btGImpactShape.h \
  bullet/src/BulletCollision/Gimpact/btQuantization.h \
  bullet/src/BulletCollision/Gimpact/btTriangleShapeEx.h \
  bullet/src/BulletCollision/Gimpact/gim_array.h \
  bullet/src/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
  bullet/src/BulletCollision/Gimpact/gim_bitset.h \
  bullet/src/BulletCollision/Gimpact/gim_box_collision.h \
  bullet/src/BulletCollision/Gimpact/gim_box_set.h \
  bullet/src/BulletCollision/Gimpact/gim_clip_polygon.h \
  bullet/src/BulletCollision/Gimpact/gim_contact.h \
  bullet/src/BulletCollision/Gimpact/gim_geom_types.h \
  bullet/src/BulletCollision/Gimpact/gim_geometry.h \
  bullet/src/BulletCollision/Gimpact/gim_hash_table.h \
  bullet/src/BulletCollision/Gimpact/gim_linear_math.h \
  bullet/src/BulletCollision/Gimpact/gim_math.h \
  bullet/src/BulletCollision/Gimpact/gim_memory.h \
  bullet/src/BulletCollision/Gimpact/gim_radixsort.h \
  bullet/src/BulletCollision/Gimpact/gim_tri_collision.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
  bullet/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h


# BulletDynamics

SOURCES += bullet/src/BulletDynamics/Character/btKinematicCharacterController.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
  bullet/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
  bullet/src/BulletDynamics/Dynamics/btContinuousDynamicsWorld.cpp \
  bullet/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
  bullet/src/BulletDynamics/Dynamics/btRigidBody.cpp \
  bullet/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
  bullet/src/BulletDynamics/Dynamics/Bullet-C-API.cpp \
  bullet/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
  bullet/src/BulletDynamics/Vehicle/btWheelInfo.cpp

HEADERS += bullet/src/btBulletDynamicsCommon.h \
  bullet/src/btBulletCollisionCommon.h \
  bullet/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
  bullet/src/BulletDynamics/ConstraintSolver/btContactConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
  bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
  bullet/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
  bullet/src/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btSolverBody.h \
  bullet/src/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
  bullet/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
  bullet/src/BulletDynamics/Dynamics/btActionInterface.h \
  bullet/src/BulletDynamics/Dynamics/btContinuousDynamicsWorld.h \
  bullet/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
  bullet/src/BulletDynamics/Dynamics/btDynamicsWorld.h \
  bullet/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
  bullet/src/BulletDynamics/Dynamics/btRigidBody.h \
  bullet/src/BulletDynamics/Vehicle/btRaycastVehicle.h \
  bullet/src/BulletDynamics/Vehicle/btVehicleRaycaster.h \
  bullet/src/BulletDynamics/Vehicle/btWheelInfo.h \
  bullet/src/BulletDynamics/Character/btCharacterControllerInterface.h \
  bullet/src/BulletDynamics/Character/btKinematicCharacterController.h


# LinearMath

SOURCES += bullet/src/LinearMath/btAlignedAllocator.cpp \
  bullet/src/LinearMath/btConvexHull.cpp \
  bullet/src/LinearMath/btGeometryUtil.cpp \
  bullet/src/LinearMath/btQuickprof.cpp \
  bullet/src/LinearMath/btSerializer.cpp

HEADERS += bullet/src/LinearMath/btAabbUtil2.h \
  bullet/src/LinearMath/btAlignedAllocator.h \
  bullet/src/LinearMath/btAlignedObjectArray.h \
  bullet/src/LinearMath/btConvexHull.h \
  bullet/src/LinearMath/btDefaultMotionState.h \
  bullet/src/LinearMath/btGeometryUtil.h \
  bullet/src/LinearMath/btHashMap.h \
  bullet/src/LinearMath/btIDebugDraw.h \
  bullet/src/LinearMath/btList.h \
  bullet/src/LinearMath/btMatrix3x3.h \
  bullet/src/LinearMath/btMinMax.h \
  bullet/src/LinearMath/btMotionState.h \
  bullet/src/LinearMath/btPoolAllocator.h \
  bullet/src/LinearMath/btQuadWord.h \
  bullet/src/LinearMath/btQuaternion.h \
  bullet/src/LinearMath/btQuickprof.h \
  bullet/src/LinearMath/btRandom.h \
  bullet/src/LinearMath/btScalar.h \
  bullet/src/LinearMath/btSerializer.h \
  bullet/src/LinearMath/btStackAlloc.h \
  bullet/src/LinearMath/btTransform.h \
  bullet/src/LinearMath/btTransformUtil.h \
  bullet/src/LinearMath/btVector3.h


# BulletFileLoader

SOURCES += bullet/Extras/Serialize/BulletFileLoader/bChunk.cpp \
  bullet/Extras/Serialize/BulletFileLoader/bDNA.cpp \
  bullet/Extras/Serialize/BulletFileLoader/bFile.cpp \
  bullet/Extras/Serialize/BulletFileLoader/btBulletFile.cpp

HEADERS += bullet/Extras/Serialize/BulletFileLoader/bChunk.h \
  bullet/Extras/Serialize/BulletFileLoader/bCommon.h \
  bullet/Extras/Serialize/BulletFileLoader/bDefines.h \
  bullet/Extras/Serialize/BulletFileLoader/bDNA.h \
  bullet/Extras/Serialize/BulletFileLoader/bFile.h \
  bullet/Extras/Serialize/BulletFileLoader/btBulletFile.h


# BulletWorldImporter

INCLUDEPATH += bullet/Extras/Serialize/BulletWorldImporter/

SOURCES += bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp

HEADERS += bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.h
