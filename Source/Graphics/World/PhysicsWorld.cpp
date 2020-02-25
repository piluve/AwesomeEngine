#include "PhysicsWorld.h"
#include "TransformComponent.h"
#include "Actor.h"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "reactphysics3d.h"

using namespace World;
using namespace reactphysics3d;

PhysicsWorld::PhysicsWorld():
	mDynamicsWorld(nullptr)
{

}

PhysicsWorld::~PhysicsWorld()
{

}

void PhysicsWorld::Initialize()
{
	// Setup the world:
	Vector3 kGravity(0.0f, -0.81f, 0.0f);
	WorldSettings kPhysicsSettings = {};
	mDynamicsWorld = new DynamicsWorld(kGravity, kPhysicsSettings);
}

void PhysicsWorld::Update(float deltaTime)
{
	// Start by stepping the physics world. deltaTime is not constant... We probably want to fix that!
	mDynamicsWorld->update(deltaTime / 1000.0f);
}

void PhysicsWorld::AddRigidBody(RigidBodyComponent* rigidBodyComponent)
{
	TransformComponent* transform = rigidBodyComponent->GetParent()->Transform;

	// Retrieve initial transform for the rigid body:
	Vector3 initialPos = Vector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);
	glm::fquat quat = glm::fquat(transform->GetRotation());
	Quaternion initialRot = Quaternion(quat.x, quat.y, quat.z, quat.w);
	Transform rbTransform;
	rbTransform.setPosition(initialPos);
	rbTransform.setOrientation(initialRot);

	// Create the rigid body:
	rigidBodyComponent->mRigidBody = mDynamicsWorld->createRigidBody(rbTransform);
	rigidBodyComponent->SetBodyType(RigidBodyComponent::Type::Dynamic);
	rigidBodyComponent->mRigidBody->set
}

RigidBodyComponent::RigidBodyComponent() :
	 mRigidBody(nullptr)
	,mBodyType(Type::Dynamic)
	,mMass(1.0f)
{
}

void RigidBodyComponent::UpdatePhysics()
{
	if (mRigidBody)
	{
		Transform transform = mRigidBody->getTransform();
		mParent->Transform->SetPosition(transform.getPosition().x, transform.getPosition().y, transform.getPosition().z);
		mParent->Transform->SetRotation(glm::eulerAngles(glm::fquat(
			transform.getOrientation().x, transform.getOrientation().y, transform.getOrientation().z, transform.getOrientation().w)
		));
	}
}

void RigidBodyComponent::UpdateLate()
{
	if (mRigidBody)
	{
		Vector3 pos = Vector3(mParent->Transform->GetPosition().x, mParent->Transform->GetPosition().y, mParent->Transform->GetPosition().z);
		glm::fquat quat = glm::fquat(mParent->Transform->GetRotation());
		Quaternion rot= Quaternion(quat.x, quat.y, quat.z, quat.w);
		Transform rbTransform;
		rbTransform.setPosition(pos);
		rbTransform.setOrientation(rot);
	}
}

RigidBodyComponent::Type::T RigidBodyComponent::GetBodyType() const
{
	return mBodyType;
}

void RigidBodyComponent::SetBodyType(const RigidBodyComponent::Type::T& t)
{
	if (t != mBodyType)
	{
		mBodyType = t;
		mRigidBody->setType(t == Type::Dynamic ? BodyType::DYNAMIC : (t == Type::Kinematic ? BodyType::KINEMATIC : BodyType::STATIC));
	}
}

void RigidBodyComponent::AddCollider(ColliderComponent* collider, float mass, glm::mat4 transform)
{
	// Note that this transform shouldn't have any scale.
	Transform colliderTransform;
	colliderTransform.setFromOpenGL(&transform[0][0]);

	// The collider saves the proxy reference:
	collider->mProxyShape = mRigidBody->addCollisionShape(collider->GetCollisionShape(), colliderTransform, mass);
}

void RigidBodyComponent::RemoveCollider(ColliderComponent* collider)
{
}

ColliderComponent::ColliderComponent() :
	 mProxyShape(nullptr)
	,mRigidBodyOwner(nullptr)
{
}

SphereColliderComponent::SphereColliderComponent() :
	 mSphereShape(nullptr)
{
	mSphereShape = new SphereShape(1.0f);
}

reactphysics3d::CollisionShape* SphereColliderComponent::GetCollisionShape()
{
	return mSphereShape;
}

BoxColliderComponent::BoxColliderComponent():
	 mBoxShape(nullptr)
	,mLocalExtents(0.5f,0.5f,0.5f)
{
	// 1x1x1 box
	mBoxShape = new BoxShape(Vector3(mLocalExtents.x, mLocalExtents.y, mLocalExtents.z));
}

void BoxColliderComponent::SetLocalExtents(const glm::vec3& extents)
{
	if (extents != mLocalExtents)
	{
		mLocalExtents = extents;
		if (mRigidBodyOwner)
		{
			mRigidBodyOwner->mRigidBody->removeCollisionShape(mProxyShape);
			mProxyShape = nullptr;
		}
		delete mBoxShape;
		mBoxShape = new BoxShape(Vector3(mLocalExtents.x, mLocalExtents.y, mLocalExtents.z));
		
	}
}

glm::vec3 BoxColliderComponent::GetLocalExtents() const
{
	return mLocalExtents;
}

reactphysics3d::CollisionShape* BoxColliderComponent::GetCollisionShape()
{
	return mBoxShape;
}
