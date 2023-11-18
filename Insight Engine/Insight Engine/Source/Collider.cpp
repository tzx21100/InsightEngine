
#include "Pch.h"
#include "Collider.h"

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
		mCircleCollider = CircleCollider();
		DisableAllColliders(); // Reset all bits to 0
		EnableBoxCollider();
		mIsColliding = false;
		mResponseEnable = true;
	}

	void Collider::CreateCollider() {
		if (IsBoxColliderEnable()) {
			CreateBoxCollider();
		}

		if (IsCircleColliderEnable()) {
			//CreateCircleCollider();
		}

		//if (mSelectedCollider.test(ColliderShape::LINE)) {
		//	//CreateLineCollider();
		//}
	}

	void Collider::UpdateCollider(Transform const& trans) {
		if (IsBoxColliderEnable()) {
			UpdateBoxCollider(trans);
		}

		if (IsCircleColliderEnable()) {
			UpdateCircleCollider(trans);
		}

		//if (mSelectedCollider.test(ColliderShape::LINE)) {
		//	//UpdateLineCollider();
		//}
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
		mCircleCollider.radius = std::max(std::abs(trans.scaling.x), std::abs(trans.scaling.y)) * mCircleCollider.radiusScale;
	}

	void Collider::EnableAllColliders() {
		mSelectedCollider.set();
	}

	void Collider::DisableAllColliders() {
		mSelectedCollider.reset();
	}

	// HELPER FUNCTION FOR BOX COLLIDER

	void Collider::EnableBoxCollider() {
		mSelectedCollider.set(ColliderShape::BOX);
	}

	void Collider::DisableBoxCollider() {
		mSelectedCollider.reset(ColliderShape::BOX);
	}

	bool Collider::IsBoxColliderEnable() {
		return mSelectedCollider.test(ColliderShape::BOX);
	}

	// HELPER FUNCTION FOR CIRCLE COLLIDER

	void Collider::EnableCircleCollider() {
		mSelectedCollider.set(ColliderShape::CIRCLE);
	}

	void Collider::DisableCircleCollider() {
		mSelectedCollider.reset(ColliderShape::CIRCLE);
	}

	bool Collider::IsCircleColliderEnable() {
		return mSelectedCollider.test(ColliderShape::CIRCLE);
	}


	
	Json::Value Collider::Serialize()
	{
		Json::Value data;
		bool box_enabled = IsBoxColliderEnable() ? true : false;
		bool circle_enabled = IsCircleColliderEnable() ? true : false;

		// Save Box Collider
		data["BoxColliderEnabled"] = box_enabled;
		if (box_enabled)
		{
			Json::Value box_collider;

			Json::Value collider_center;
			collider_center["X"] = mBoxCollider.center.x;
			collider_center["Y"] = mBoxCollider.center.y;
			box_collider["BoxColliderCenter"] = collider_center;

			Json::Value collider_offset;
			collider_offset["X"] = mBoxCollider.offset.x;
			collider_offset["Y"] = mBoxCollider.offset.y;
			box_collider["BoxColliderOffset"] = collider_offset;

			Json::Value collider_scale;
			collider_scale["X"] = mBoxCollider.sizeScale.x;
			collider_scale["Y"] = mBoxCollider.sizeScale.y;
			box_collider["BoxColliderSizeScale"] = collider_scale;

			Json::Value vertices_array(Json::arrayValue);
			for (const auto& vertex : mBoxCollider.vertices)
			{
				Json::Value v;
				v["x"] = vertex.x;
				v["y"] = vertex.y;
				vertices_array.append(v);
			}
			box_collider["BoxColliderVertices"] = vertices_array;

			data["BoxCollider"] = box_collider;
		}

		// Save Circle Collider
		data["CircleColliderEnabled"] = circle_enabled;
		if (circle_enabled)
		{
			Json::Value circle_collider;

			Json::Value collider_center;
			collider_center["X"] = mBoxCollider.center.x;
			collider_center["Y"] = mBoxCollider.center.y;
			circle_collider["CircleColliderCenter"] = collider_center;

			Json::Value collider_offset;
			collider_offset["X"] = mBoxCollider.offset.x;
			collider_offset["Y"] = mBoxCollider.offset.y;
			circle_collider["CircleColliderOffset"] = collider_offset;

			circle_collider["CircleColliderRadius"] = mCircleCollider.radius;
			circle_collider["CircleColliderRadiusScale"] = mCircleCollider.radiusScale;

			data["CircleCollider"] = circle_collider;
		}

		// Save other colliders...

		return data;
	}

	void Collider::Deserialize(Json::Value data)
	{
		// Disable all colliders
		DisableAllColliders();

		bool box_enabled = data["BoxColliderEnabled"].asBool();
		bool circle_enabled = data["CircleColliderEnabled"].asBool();

		// Load Box Collider
		if (box_enabled)
		{
			EnableBoxCollider();
			mBoxCollider.center.x = data["BoxCollider"]["BoxColliderCenter"]["X"].asFloat();
			mBoxCollider.center.y = data["BoxCollider"]["BoxColliderCenter"]["Y"].asFloat();
			mBoxCollider.offset.x = data["BoxCollider"]["BoxColliderOffset"]["X"].asFloat();
			mBoxCollider.offset.y = data["BoxCollider"]["BoxColliderOffset"]["Y"].asFloat();
			mBoxCollider.sizeScale.x = data["BoxCollider"]["BoxColliderSizeScale"]["X"].asFloat();
			mBoxCollider.sizeScale.y = data["BoxCollider"]["BoxColliderSizeScale"]["Y"].asFloat();

			Json::Value vertices_array = data["BoxCollider"]["BoxColliderVertices"];
			for (const auto& v : vertices_array)
			{
				Vector2D vertex;
				vertex.x = v["X"].asFloat();
				vertex.y = v["Y"].asFloat();
				mBoxCollider.vertices.push_back(vertex);
			}
		}

		// Load Circle Collider
		if (circle_enabled)
		{
			EnableCircleCollider();
			mCircleCollider.center.x = data["CircleCollider"]["CircleColliderCenter"]["X"].asFloat();
			mCircleCollider.center.y = data["CircleCollider"]["CircleColliderCenter"]["Y"].asFloat();
			mCircleCollider.offset.x = data["CircleCollider"]["CircleColliderOffset"]["X"].asFloat();
			mCircleCollider.offset.y = data["CircleCollider"]["CircleColliderOffset"]["Y"].asFloat();
			mCircleCollider.radius = data["CircleCollider"]["CircleColliderRadius"].asFloat();
			mCircleCollider.radiusScale = data["CircleCollider"]["CircleColliderRadiusScale"].asFloat();
		}

		// Load other colliders...
	}

}
