#pragma once
#include "Component.h"
#include <SimpleMath.h>

class Collision : public Component
{
private:
	//class Sphere;
	//class Capsule;
protected:
	class AABB;
	DirectX::SimpleMath::Vector3 m_Offset;
	DirectX::SimpleMath::Vector3 m_Sink;	// ÇﬂÇËçûÇÒÇæÉxÉNÉgÉã


	bool m_bHit;
public:

	virtual bool IsHit(AABB* aabb) { return false; }
	virtual bool IsHit(void) { return false; }
	void SetOffset(const DirectX::SimpleMath::Vector3 offset) { m_Offset = offset; }
};

