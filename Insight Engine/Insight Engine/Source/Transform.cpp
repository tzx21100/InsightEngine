/*!
 * \file Transform.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This source file defines the Transform class, which represents transformation properties
 * for game objects in an OpenGL-based application.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"

namespace IS {
	Transform::Transform() { // default constructor
		this->world_position = Vec2D();
		this->rotation = 0.f;
		this->scaling = Vec2D();
		ISMtx33Identity(mdl_to_ndc_xform);
	}

	Transform::Transform(Vec2D world_position, float rotation, Vec2D scaling) { // param constructor
		this->world_position = world_position;
		this->rotation = rotation;
		this->scaling = scaling;
		ISMtx33Identity(mdl_to_ndc_xform);
	}

	Vector2D Transform::getWorldPosition() { // for physics system
		return Vector2D(world_position.x, world_position.y);
	}

	void Transform::getTransformedPoint(Vector2D& v) const {
		Vector2D temp = v;
		float angle = glm::radians(rotation);
		v.x = cosf(angle) * temp.x - sinf(angle) * temp.y + world_position.x;
		v.y = sinf(angle) * temp.x + cosf(angle) * temp.y + world_position.y;;
	}

	float Transform::getRotation() { 
		return rotation;
	}

	Vector2D Transform::getScaling() {
		return Vector2D(scaling.x, scaling.y);
	}

	void Transform::setWorldPosition(float x, float y) { // set T
		world_position.x = x;
		world_position.y = y;
	}

	void Transform::setRotation(float angle, float angleSpeed) { // set R
		rotation = angle;
		angle_speed = angleSpeed;
	}

	void Transform::setScaling(float width, float height) { // set S
		scaling.x = width;
		scaling.y = height;
	}

	void Transform::Move(Vector2D const& val) {
		world_position.x += val.x;
		world_position.y += val.y;
	}

	Mtx33 Transform::ReturnXformMatrix() {
		// convert angle to radians
		float angle_rad = glm::radians(rotation);

		// to scale to world coordinates
		auto [width, height] = InsightEngine::Instance().GetWindowSize();

		// math variables used often
		float sin_angle = sinf(angle_rad);
		float cos_angle = cosf(angle_rad);
		float model_scale_x = scaling.x / 2.f;
		float model_scale_y = scaling.y / 2.f;
		float tx = world_position.x;
		float ty = world_position.y;

		// dont calculate camera xform every frame
		//glm::mat3 world_to_cam_xform = { (a * model_scale_x * cos_angle) + (b * model_scale_x * sin_angle),  (d * model_scale_x * cos_angle) + (e * model_scale_x * sin_angle),  0.f,   // column 1
		//								 (a * model_scale_y * -sin_angle) + (b * model_scale_y * cos_angle), (d * model_scale_y * -sin_angle) + (e * model_scale_y * cos_angle), 0.f,   // column 2
		//								 (a * tx + b * ty + c),                                              (d * tx + e * ty + f),                                              1.f }; // column 3

		glm::mat3 world_to_cam_xform = { (model_scale_x * cos_angle),  (model_scale_x * sin_angle),  0.f,   // column 1
										 (model_scale_y * -sin_angle), (model_scale_y * cos_angle),  0.f,   // column 2
										 tx,                           ty,                           1.f }; // column 3


		// save matrix
		return GlmMat3ToISMtx33(ISGraphics::cameras[Camera::mActiveCamera].xform * world_to_cam_xform);
	}

	glm::mat4 Transform::Return3DXformMatrix() {
		// convert angle to radians
		float angle_rad = glm::radians(rotation);

		// to scale to world coordinates
		//auto [width, height] = InsightEngine::Instance().GetWindowSize();

		// math variables used often
		float sin_angle = sinf(angle_rad);
		float cos_angle = cosf(angle_rad);
		float model_scale_x = scaling.x / 2.f;
		float model_scale_y = scaling.y / 2.f;
		float tx = world_position.x;
		float ty = world_position.y;

		// dont calculate camera xform every frame
		//glm::mat3 world_to_cam_xform = { (a * model_scale_x * cos_angle) + (b * model_scale_x * sin_angle),  (d * model_scale_x * cos_angle) + (e * model_scale_x * sin_angle),  0.f,   // column 1
		//								 (a * model_scale_y * -sin_angle) + (b * model_scale_y * cos_angle), (d * model_scale_y * -sin_angle) + (e * model_scale_y * cos_angle), 0.f,   // column 2
		//								 (a * tx + b * ty + c),                                              (d * tx + e * ty + f),                                              1.f }; // column 3

		glm::mat4 world_to_cam_xform = { (model_scale_x * cos_angle),  (model_scale_x * sin_angle), 0.f, 0.f,  // column 1
										 (model_scale_y * -sin_angle), (model_scale_y * cos_angle), 0.f, 0.f,  // column 2
										 0.f,                          0.f,                         1.f, 0.f,   
										 tx,					       ty,						    1.f, 1.f }; // column 3


		// save matrix
		return ISGraphics::cameras3D[Camera3D::mActiveCamera].getCameraToNDCXform() * world_to_cam_xform;
	}

	glm::mat4 Transform::FUCKYK() {
		// convert angle to radians
		float angle_rad = glm::radians(rotation);

		// to scale to world coordinates
		//auto [width, height] = InsightEngine::Instance().GetWindowSize();

		// math variables used often
		float sin_angle = sinf(angle_rad);
		float cos_angle = cosf(angle_rad);
		float model_scale_x = scaling.x;
		float model_scale_y = scaling.y;
		float tx = world_position.x;
		float ty = world_position.y;

		// dont calculate camera xform every frame
		//glm::mat3 world_to_cam_xform = { (a * model_scale_x * cos_angle) + (b * model_scale_x * sin_angle),  (d * model_scale_x * cos_angle) + (e * model_scale_x * sin_angle),  0.f,   // column 1
		//								 (a * model_scale_y * -sin_angle) + (b * model_scale_y * cos_angle), (d * model_scale_y * -sin_angle) + (e * model_scale_y * cos_angle), 0.f,   // column 2
		//								 (a * tx + b * ty + c),                                              (d * tx + e * ty + f),                                              1.f }; // column 3

		glm::mat4 world_to_cam_xform = { (model_scale_x * cos_angle),  (model_scale_x * sin_angle), 0.f, 0.f,  // column 1
										 (model_scale_y * -sin_angle), (model_scale_y * cos_angle), 0.f, 0.f,  // column 2
										 0.f,                          0.f,                         1.f, 0.f,
										 tx,					       ty,						    1.f, 1.f }; // column 3


		// save matrix
		return world_to_cam_xform;
	}

	// not in use
	std::vector<Vector2D> Transform::GetSquareTransformVertices() {
		std::vector<Vector2D> vertices;
		float left = world_position.x - (scaling.x / 2.f);
		float right = world_position.x + (scaling.x / 2.f);
		float bottom = world_position.y - (scaling.y / 2.f);
		float top = world_position.y + (scaling.y / 2.f);

		vertices.emplace_back(Vector2D(left, top)); // 0 top left
		vertices.emplace_back(Vector2D(right, top)); // 1 top right
		vertices.emplace_back(Vector2D(right, bottom)); // 2 bottom right
		vertices.emplace_back(Vector2D(left, bottom)); // 3 bottom left

		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].x = cosf(rotation) * world_position.x - sinf(rotation) * world_position.y + world_position.x;
			vertices[i].y = sinf(rotation) * world_position.x + cosf(rotation) * world_position.y + world_position.y;
		}
		
		return vertices;
	}

	Json::Value Transform::Serialize() {
		Json::Value transformData;

		// Serializing world_position
		transformData["TransformWorldPositionX"] = world_position.x;
		transformData["TransformWorldPositionY"] = world_position.y;

		// Serializing rotation
		transformData["TransformRotation"] = rotation;

		// Serializing scaling
		transformData["TransformScalingX"] = scaling.x;
		transformData["TransformScalingY"] = scaling.y;

		// Note: Not serializing mdl_to_ndc_xform since matrix serialization can be complex and depends on specifics

		return transformData;
	}

	void Transform::Deserialize(Json::Value data) {
		// Deserializing world_position
		world_position.x = data["TransformWorldPositionX"].asFloat();
		world_position.y = data["TransformWorldPositionY"].asFloat();

		// Deserializing rotation
		rotation = data["TransformRotation"].asFloat();

		// Deserializing scaling
		scaling.x = data["TransformScalingX"].asFloat();
		scaling.y = data["TransformScalingY"].asFloat();

		// Note: Not deserializing mdl_to_ndc_xform since matrix deserialization can be complex and depends on specifics
	}

	glm::mat3 ISMtx33ToGlmMat3(Matrix3x3 const& mat) {
		glm::mat3 ret{
			mat.m00, mat.m10, mat.m20, // col 0
			mat.m01, mat.m11, mat.m21, // col 1
			mat.m02, mat.m12, mat.m22, // col 2
		};
		return ret;
	}

	Matrix3x3 GlmMat3ToISMtx33(glm::mat3 const& mat) {
		Matrix3x3 ret;

		ret.m00 = mat[0][0]; ret.m01 = mat[1][0]; ret.m02 = mat[2][0]; // row 0
		ret.m10 = mat[0][1]; ret.m11 = mat[1][1]; ret.m12 = mat[2][1]; // row 1
		ret.m20 = mat[0][2]; ret.m21 = mat[1][2]; ret.m22 = mat[2][2]; // row 2

		return ret;
	}
}
