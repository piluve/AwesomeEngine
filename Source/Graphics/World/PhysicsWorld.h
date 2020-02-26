#pragma once

#include "Component.h"

#include "glm/glm.hpp"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxScene;
	class PxPvd;
	class PxDefaultCpuDispatcher;
	class PxMaterial;
	class PxRigidActor;
}

namespace World
{
	class RigidBodyComponent;
	class ColliderComponent;
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void Initialize();
		void Update(float deltaTime);
		void Release();

		void AddRigidBody(RigidBodyComponent* rigidBodyComponent);

	private:
		// One per instance.
		physx::PxFoundation* mFoundation;
		physx::PxPhysics* mPhysics;
		physx::PxScene* mScene;
		// PhysX Visual Debugger
		physx::PxPvd* mPvd;
		physx::PxDefaultCpuDispatcher* mCpuDispatcher;
		physx::PxMaterial* mMaterial;

	};

	class RigidBodyComponent : public Component
	{
		friend PhysicsWorld;
		friend ColliderComponent;
	public:
		RigidBodyComponent();

		// We just finished updating the physics world, now it's a good point
		// to sync the transform with the physics
		void UpdatePhysics();
		void Update(float deltaTime) {};
		// Sync back the transformations, generally, this shouldn't do anything 
		// as you shouldn't update a dynamic/kinematic object using positions (for static it is fine).
		void UpdateLate();

		struct Type
		{
			enum T
			{
				Static,
				Kinematic,
				Dynamic,
				COUNT
			};
		};

		Type::T GetBodyType()const;
		void SetBodyType(const Type::T& t);

		void RemoveCollider(ColliderComponent* collider);

	private:
		Type::T mBodyType;
		float mMass;
		physx::PxRigidActor* mRigidBody;
	};

	class ColliderComponent : public Component
	{
		friend RigidBodyComponent;
	public:
		ColliderComponent();
		void Update(float deltaTime) {};

	protected:
		RigidBodyComponent* mRigidBodyOwner;
	};

	class SphereColliderComponent : public ColliderComponent
	{
	public:
		SphereColliderComponent();
		void Update(float deltaTime) {};

	protected:

	private:
	};

	class BoxColliderComponent : public ColliderComponent
	{
	public:
		BoxColliderComponent();
		void Update(float deltaTime) {};

		void SetLocalExtents(const glm::vec3& extents);
		glm::vec3 GetLocalExtents()const;

	protected:

	private:
		glm::vec3 mLocalExtents;
	};

}