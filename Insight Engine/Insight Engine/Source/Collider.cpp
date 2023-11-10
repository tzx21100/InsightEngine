
#include "Pch.h"

namespace IS
{
	BoxCollider::BoxCollider() {
		center = Vector2D(0.f, 0.f);
		offset = Vector2D(0.f, 0.f);
		sizeScale = Vector2D(1.f, 1.f);
		vertices.reserve(4);
		transformedVertices.reserve(4);
	}

	BoxCollider::BoxCollider(Vector2D const& center_, Vector2D const& offset_, Vector2D const& sizeScale_, std::vector<Vector2D> const& vertices_, std::vector<Vector2D> const& transformedVertices_) {
		center = center_;
		offset = offset_;
		sizeScale = sizeScale_;
		vertices = vertices_;
		transformedVertices = transformedVertices_;
	}

	CircleCollider::CircleCollider() {
		center = Vector2D(0.f, 0.f);
		radius = 0.5f;
		offset = Vector2D(0.f, 0.f);
	}

	CircleCollider::CircleCollider(Vector2D const& center_, float const& radius_, Vector2D const& offset_) {
		center = center_;
		radius = radius_;
		offset = offset_;
	}

	Collider::Collider() {
		mBoxCollider = BoxCollider();
		mSelectedCollider.reset(); // Reset all bits to 0
	}

	void Collider::CreateCollider() {
		if (mSelectedCollider.test(ColliderShape::BOX)) {
			CreateBoxCollider();
		}

		if (mSelectedCollider.test(ColliderShape::CIRCLE)) {
			//CreateCircleCollider();
		}

		if (mSelectedCollider.test(ColliderShape::LINE)) {
			//CreateLineCollider();
		}
	}

	void Collider::UpdateCollider(Transform const& trans) {
		if (mSelectedCollider.test(ColliderShape::BOX)) {
			UpdateBoxCollider(trans);
		}

		if (mSelectedCollider.test(ColliderShape::CIRCLE)) {
			//UpdateCircleCollider();
		}

		if (mSelectedCollider.test(ColliderShape::LINE)) {
			//UpdateLineCollider();
		}
	}

	void Collider::CreateBoxCollider() {

		//mBoxCollider->center = Vector2D();
		//mBoxCollider->min = Vector2D();
		//mBoxCollider->max = Vector2D();
		//mBoxCollider->offset = Vector2D();
		//mBoxCollider->sizeScale = Vector2D(1.f, 1.f);
		//mBoxCollider->vertices.reserve(4);
		//mBoxCollider->transformedVertices.reserve(4);

	}

	void Collider::UpdateBoxCollider(Transform const& trans) {

		mBoxCollider.center = trans.world_position + mBoxCollider.offset;
		float width = trans.scaling.x * mBoxCollider.sizeScale.x;
		float height = trans.scaling.y * mBoxCollider.sizeScale.y;

		// the vertices are calculated based on origin (not transform yet)
		float left = -width / 2.f;
		float right = left + width;
		float bottom = -height / 2.f;
		float top = bottom + height;

		std::vector<Vector2D> box_vertices;
		box_vertices.emplace_back(Vector2D(left, top)); // 0 top left
		box_vertices.emplace_back(Vector2D(right, top)); // 1 top right
		box_vertices.emplace_back(Vector2D(right, bottom)); // 2 bottom right
		box_vertices.emplace_back(Vector2D(left, bottom)); // 3 bottom left

		mBoxCollider.vertices = box_vertices;

		for (int i = 0; i < mBoxCollider.vertices.size(); i++) {
			mBoxCollider.transformedVertices[i].x = cosf(trans.rotation) * trans.world_position.x - sinf(trans.rotation) * trans.world_position.y + trans.world_position.x;
			mBoxCollider.transformedVertices[i].y = sinf(trans.rotation) * trans.world_position.x + cosf(trans.rotation) * trans.world_position.y + trans.world_position.y;
		}
	}

}