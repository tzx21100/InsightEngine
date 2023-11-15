
#include "Pch.h"

namespace IS
{
	BoxCollider::BoxCollider() {
		center = Vector2D(0.f, 0.f);
		offset = Vector2D(0.f, 0.f);
		sizeScale = Vector2D(1.f, 1.f);
		vertices.emplace_back(Vector2D(0.f, 0.f));
		vertices.emplace_back(Vector2D(0.f, 0.f));
		vertices.emplace_back(Vector2D(0.f, 0.f));
		vertices.emplace_back(Vector2D(0.f, 0.f));
		transformedVertices = vertices;
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
		radius = 0.f;
		offset = Vector2D(0.f, 0.f);
		radiusScale = 0.5f;
	}

	CircleCollider::CircleCollider(Vector2D const& center_, float const& radius_, Vector2D const& offset_) {
		center = center_;
		radius = radius_;
		offset = offset_;
	}

	Collider::Collider() {
		mBoxCollider = BoxCollider();
		mSelectedCollider.reset(); // Reset all bits to 0
		mSelectedCollider.set(ColliderShape::BOX);
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
			UpdateCircleCollider(trans);
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

	void Collider::CreateBoxVertices(float width, float height) {
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
	}

	void Collider::UpdateBoxCollider(Transform const& trans) {

		mBoxCollider.center = trans.world_position + mBoxCollider.offset;
		float width = trans.scaling.x * mBoxCollider.sizeScale.x;
		float height = trans.scaling.y * mBoxCollider.sizeScale.y;

		CreateBoxVertices(width, height);

		for (int i = 0; i < mBoxCollider.vertices.size(); i++) {
			float angle = glm::radians(trans.rotation);
			mBoxCollider.transformedVertices[i].x = cosf(angle) * mBoxCollider.vertices[i].x - sinf(angle) * mBoxCollider.vertices[i].y + mBoxCollider.center.x;
			mBoxCollider.transformedVertices[i].y = sinf(angle) * mBoxCollider.vertices[i].x + cosf(angle) * mBoxCollider.vertices[i].y + mBoxCollider.center.y;
		}
	}

	void Collider::UpdateCircleCollider(Transform const& trans) {
		mCircleCollider.center = trans.world_position + mCircleCollider.offset;
		// radius calculation will follows the larger scale of x/y
		mCircleCollider.radius = std::max(trans.scaling.x, trans.scaling.y) * mCircleCollider.radiusScale;
	}

}