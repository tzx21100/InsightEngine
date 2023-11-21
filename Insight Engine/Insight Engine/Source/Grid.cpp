/*!
 * \file Grid.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This header file contains the declaration of the `ImplicitGrid` class, which is responsible
 * for managing an implicit grid system used for optimizing spatial partitioning in a game engine.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
   ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Grid.h"

namespace IS
{
	// init cols and rows
	int ImplicitGrid::mCols = ImplicitGrid::DEFAULT_GRID_COLS;
	int ImplicitGrid::mRows = ImplicitGrid::DEFAULT_GRID_ROWS;

	Vector2D ImplicitGrid::mCellSize{};

	// Default ctor
	ImplicitGrid::ImplicitGrid() { // clear the grid and reset all bits to 0
		for (int i = 0; i < MAX_GRID_ROWS; ++i) {
			mRowsBitArray[i].reset();  // Reset all bits to 0
		}

		for (int i = 0; i < MAX_GRID_COLS; ++i) {
			mColsBitArray[i].reset();  // Reset all bits to 0
		}
		mInGridList.reserve(MAX_ENTITIES);
		mOverlapGridList.reserve(MAX_ENTITIES);
		mOutsideGridList.reserve(MAX_ENTITIES);
	}

	// Clears the grid, resetting its state.
	void ImplicitGrid::ClearGrid() {
		// clear the grid and reset all bits to 0
		for (int i = 0; i < MAX_GRID_ROWS; ++i) {
			mRowsBitArray[i].reset();  // Reset all bits to 0
		}

		for (int i = 0; i < MAX_GRID_COLS; ++i) {
			mColsBitArray[i].reset();  // Reset all bits to 0
		}
		mInGridList.clear();
		mOverlapGridList.clear();
		mOutsideGridList.clear();
	}

	// Get the cell corresponding to a given position in the grid.
	Cell ImplicitGrid::GetCell(Vector2D const& position) {

		//// Set cell size based on current window size
		//glm::vec2 camera_dimension = ISGraphics::cameras[Camera::mActiveCamera].GetCamDim();
		//glm::vec2 camera_center = ISGraphics::cameras[Camera::mActiveCamera].GetCamPos();

		//// set the grid's width and height as 2 times bigger than camera's width and height
		//float width = camera_dimension.x * 2;
		//float height = camera_dimension.y * 2;

		auto& engine = InsightEngine::Instance();
		auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
		glm::vec2 camera_center = ISGraphics::cameras3D[Camera3D::mActiveCamera].GetPosition2D();
		auto [x, y] = engine.IsFullScreen() ? engine.GetMonitorSize() : engine.GetWindowSize();
		// set the grid's width and height as 2 times bigger than camera's width and height
		float width = (static_cast<float>(x) * camera.GetZoomLevel()) * 1.f;
		float height = (static_cast<float>(y) * camera.GetZoomLevel()) * 1.f;
		//IS_CORE_DEBUG("Width : {:.2f}", width);
		//IS_CORE_DEBUG("Height : {:.2f}", height);
		/*float width = 12000.f;
		float height = 5000.f;*/

		mGridSize = { static_cast<float>(width), static_cast<float>(height) };
		mCellSize = { static_cast<float>(width) / mCols, static_cast<float>(height) / mRows };
		
		float tmp_col = (position.x + (mGridSize.x / 2.f) - camera_center.x) / mCellSize.x;
		float tmp_row = (-position.y + (mGridSize.y / 2.f) + camera_center.y) / mCellSize.y;
		int col = static_cast<int>(tmp_col < 0 ? tmp_col -1 : tmp_col);
		int row = static_cast<int>(tmp_row < 0 ? tmp_row - 1 : tmp_row);

		return {row, col};
	}

	// Check if two cells are equal.
	bool ImplicitGrid::areCellsEqual(Cell const& c1, Cell const& c2) {
		return (c1.row == c2.row == c1.col == c2.col);
	}

	// Check if multiple cells are equal.
	bool ImplicitGrid::areCellsEqual(Cell const& c1, Cell const& c2, Cell const& c3, Cell const& c4) {
		return (c1.row == c2.row == c1.col == c2.col == c3.row == c4.row == c3.col == c4.col);
	}
#if 0
	// Add entities into the appropriate cell of the grid.
	void ImplicitGrid::AddIntoCell(std::set<Entity> const& Entities) {

		InsightEngine& engine = InsightEngine::Instance();
		// loop through all the entities
		for (auto const& entity : Entities) {
			if (engine.HasComponent<RigidBody>(entity)) { // if entity has rigidbody component
				//getting rigidbody component for each entity
				auto& body = engine.GetComponent<RigidBody>(entity);
				if (body.mGridState == GridState::Uninitialized) { // if the body is not initialized with grid
					auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
					body.BodyFollowTransform(trans);
					// getting the min & max for checking which cell contain the entity
					Box box = body.GetAABB();
					Cell min_cell = GetCell(box.min);
					Cell max_cell = GetCell(box.max);
					Cell center = GetCell(body.mBodyTransform.world_position);

					if (GridContains(min_cell) && GridContains(max_cell)) {
						body.mGridState = GridState::Inside;
						AddToBitArray(min_cell, max_cell, entity);
					}else if ((GridContains(min_cell) && !GridContains(max_cell)) ||
							(!GridContains(min_cell) && GridContains(max_cell))) {
						// body box overlap on the grid edge, half inside half outside
						body.mGridState = GridState::Overlap;
						mOverlapGridList.emplace_back(entity);
					}
					else if (GridContains(center) || CheckOverlap(min_cell, max_cell)) {
						// the obj is too big, min and max both outside of the grid but a small part of the obj is inside the grid
						body.mGridState = GridState::Overlap;
						mOverlapGridList.emplace_back(entity);
					}
					else { // body box fully outside of the grid
						body.mGridState = GridState::Outside;
						mOutsideGridList.emplace_back(entity);
					}
				}
				else {
					continue;
				}
			}
		}
	}
#endif

#if 0 
	// Add entities into the appropriate cell of the grid.
	void ImplicitGrid::AddIntoCell(std::set<Entity> const& Entities) {

		InsightEngine& engine = InsightEngine::Instance();
		// loop through all the entities
		for (auto const& entity : Entities) {
			if (engine.HasComponent<RigidBody>(entity)) { // if entity has rigidbody component
				//getting rigidbody component for each entity
				auto& body = engine.GetComponent<RigidBody>(entity);

				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				body.BodyFollowTransform(trans);
				// getting the min & max for checking which cell contain the entity
				Box box = body.GetAABB();
				Cell min_cell = GetCell(box.min);
				Cell max_cell = GetCell(box.max);
				Cell center = GetCell(body.mBodyTransform.world_position);

				if (GridContains(min_cell) && GridContains(max_cell)) {
					// fully in grid
					AddToBitArray(min_cell, max_cell, entity);
				}
				else if ((GridContains(min_cell) && !GridContains(max_cell)) ||
					(!GridContains(min_cell) && GridContains(max_cell))) {
					// body box overlap on the grid edge, half inside half outside
					AddToBitArray(min_cell, max_cell, entity);
					mOutsideGridList.emplace_back(entity);
				}
				else if (GridContains(center) || CheckOverlap(min_cell, max_cell)) {
					// the obj is too big, min and max both outside of the grid but a 
					// small part of the obj is inside the grid
					AddToBitArray(min_cell, max_cell, entity);
					mOutsideGridList.emplace_back(entity);
				}
				else { // body box fully outside of the grid
					mOutsideGridList.emplace_back(entity);
				}
			}
		}
	}
#endif

	// Add entities into the appropriate cell of the grid.
	void ImplicitGrid::AddIntoCell(std::set<Entity> const& Entities)
	{
		InsightEngine& engine = InsightEngine::Instance();
		if (engine.IsMinimized())
			return;
		// loop through all the entities
		for (auto const& entity : Entities)
		{
			if (engine.HasComponent<Collider>(entity))
			{ // if entity has rigidbody component
				auto& collider = engine.GetComponent<Collider>(entity);

				/*auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				body.BodyFollowTransform(trans);*/
				// getting the min & max for checking which cell contain the entity
				Box box = collider.GetAABB();
				Cell min_cell = GetCell(box.min);
				Cell max_cell = GetCell(box.max);
				//Cell center = GetCell(body.mBodyTransform.world_position);

				if (GridContains(min_cell) && GridContains(max_cell))
				{
					// fully in grid
					AddToBitArray(min_cell, max_cell, entity);
				}
				else if ((GridContains(min_cell) && !GridContains(max_cell)) ||
					(!GridContains(min_cell) && GridContains(max_cell)))
				{
					// body box overlap on the grid edge, half inside half outside
					AddToBitArray(min_cell, max_cell, entity);
					mOutsideGridList.emplace_back(entity);
				}
				else if (CheckOverlap(min_cell, max_cell))
				{
					// the obj is too big, min and max both outside of the grid but a 
					// small part of the obj is inside the grid
					AddToBitArray(min_cell, max_cell, entity);
					mOutsideGridList.emplace_back(entity);
				}
				else
				{ // body box fully outside of the grid
					mOutsideGridList.emplace_back(entity);
				}
			}
		}
	}

	// Add entities into the bit arrays representing grid cells.
	void ImplicitGrid::AddToBitArray(Cell const& min, Cell const& max, Entity const& entity) {

		// add from min to max
		for (int row = min.row; row >= max.row; row--) {
			for (int col = min.col; col <= max.col; col++) {
				if (row >= 0 && row < mRows && col >= 0 && col < mCols) {

					std::bitset<MAX_ENTITIES> mask1;
					// set the specific entity index position to 1
					mask1.set(entity, true);
					mRowsBitArray[row] |= mask1;

					std::bitset<MAX_ENTITIES> mask2;
					// set the specific entity index position to 1
					mask2.set(entity, true);
					mColsBitArray[col] |= mask2;
				}
			}
		}
		
			
		
	}

	// Remove entities from the bit arrays representing grid cells.
	void ImplicitGrid::RemoveFromBitArray(Cell const& min, Cell const& max, Entity const& entity) {

		// remove from min to max
		for (int row = min.row; row >= max.row; row--) {
			for (int col = min.col; col <= max.col; col++) {
				if (row >= 0 && row < mRows && col >= 0 && col < mCols) {
					// set the entity index position in array to 0, like XOR
					mRowsBitArray[row].reset(entity);
					// set the entity index position in array to 0, like XOR
					mColsBitArray[col].reset(entity);
				}
			}
		}
		
	}

#if 0
	// Add entities into the bit arrays representing grid cells.
	void ImplicitGrid::AddToBitArray(Cell const& min, Cell const& max, Entity const& entity) {

		// add from min to max
		for (int row = min.row; row >= max.row; row--) {
			std::bitset<MAX_ENTITIES> mask;
			// set the specific entity index position to 1
			mask.set(entity, true);
			mRowsBitArray[row] |= mask;
		}
		for (int col = min.col; col <= max.col; col++) {
			std::bitset<MAX_ENTITIES> mask;
			// set the specific entity index position to 1
			mask.set(entity, true);
			mColsBitArray[col] |= mask;
		}
	}

	// Remove entities from the bit arrays representing grid cells.
	void ImplicitGrid::RemoveFromBitArray(Cell const& min, Cell const& max, Entity const& entity) {
		
		/*if (min.row < 0 || min.row >= mRows || min.col < 0 || min.col >= mCols ||
			max.row < 0 || max.row >= mRows || max.col < 0 || max.col >= mCols) {
			return;
		}*/

		// remove from min to max
		for (int row = min.row; row >= max.row; row--) {
			// set the entity index position in array to 0, like XOR
			mRowsBitArray[row].reset(entity);
			
		}
		for (int col = min.col; col <= max.col; col++) {
			// set the entity index position in array to 0, like XOR
			mColsBitArray[col].reset(entity);
		}
	}
#endif
	// Check if there is an overlap at the edge of the grid.
	bool ImplicitGrid::CheckOverlap([[maybe_unused]] Cell const& min, [[maybe_unused]] Cell const& max) {
		//bool check = false;
		for (int row = min.row; row >= max.row; row--) {
			for (int col = min.col; col <= max.col; col++) {
				if (row >= 0 && row < mRows && col >= 0 && col < mCols) {
					return true;
				}
			}
		}
		return false;
	}

#if 0

	// Update the cell for an entity in the grid.
	void ImplicitGrid::UpdateCell(Entity const& entity, float const& dt) {
		if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
			InsightEngine& engine = InsightEngine::Instance();
			auto& body = engine.GetComponent<RigidBody>(entity);
			auto& trans = engine.GetComponent<Transform>(entity);
			body.BodyFollowTransform(trans);
			// getting the min & max for checking which cell contain the entity
			Box prev_box = body.GetAABB();
			Cell prev_min_cell = GetCell(prev_box.min);
			Cell prev_max_cell = GetCell(prev_box.max);

			// update body and trans position with velocity and dt
			body.mBodyTransform.world_position += body.mVelocity * dt;
			trans.world_position = body.mBodyTransform.world_position;

			float angle = trans.getRotation();
			angle += body.mAngularVelocity * dt * 10.f;
			trans.setRotation(angle, body.mAngularVelocity);

			// getting the new min & max for body after updating position
			Box next_box = body.GetAABB();
			Cell next_min_cell = GetCell(next_box.min);
			Cell next_max_cell = GetCell(next_box.max);
			Cell center = GetCell(body.mBodyTransform.world_position);

			switch (body.mGridState) // body's previous state for grid
			{
			case GridState::Inside: // if the body inside grid

				// if the moving dynamic body min & max move but still within the same cell
				//if (areCellsEqual(prev_min_cell, prev_max_cell, next_min_cell, next_max_cell) &&
				//	body.mBodyType == BodyType::Dynamic) {
				//	return; // no update on cell
				//} // remove due to the bugs when changing grid dimensions

				// remove the cell from previous cell
				RemoveFromBitArray(prev_min_cell, prev_max_cell, entity);
				// if the body box still within the camera view
				if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
					body.mGridState = GridState::Inside;
					AddToBitArray(next_min_cell, next_max_cell, entity);
				}
				else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
					(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
					// body box overlap on the grid edge, half inside half outside
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				// overlap grid (special case)
				else if (GridContains(center) || CheckOverlap(next_min_cell, next_max_cell)) {
					// the obj is too big, min and max both outside of the grid but a small part of the obj is inside the grid
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				else { // body box fully outside of the grid
					body.mGridState = GridState::Outside;
					mOutsideGridList.emplace_back(entity);
				}

				break;
			case GridState::Outside: // if body outside grid

				// remove from outside entity list
				mOutsideGridList.erase(std::remove(mOutsideGridList.begin(), mOutsideGridList.end(), entity), mOutsideGridList.end());

				// inside grid
				if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
					body.mGridState = GridState::Inside;
					AddToBitArray(next_min_cell, next_max_cell, entity);
				}
				// overlap grid
				else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
					(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
					// body box overlap on the grid edge, half inside half outside
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				// overlap grid (special case)
				else if (GridContains(center) || CheckOverlap(next_min_cell, next_max_cell)) {
					// the obj is too big, min and max both outside of the grid but a small part of the obj is inside the grid
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				else { // outside grid
					body.mGridState = GridState::Outside;
					mOutsideGridList.emplace_back(entity);
				}

				break;
			case GridState::Overlap: // if body overlap on grid

				// remove from outside entity list
				mOverlapGridList.erase(std::remove(mOverlapGridList.begin(), mOverlapGridList.end(), entity), mOverlapGridList.end());

				// inside grid
				if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
					body.mGridState = GridState::Inside;
					AddToBitArray(next_min_cell, next_max_cell, entity);
				}
				// overlap grid
				else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
					(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
					// body box overlap on the grid edge, half inside half outside
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				// overlap grid (special case)
				else if (GridContains(center) || CheckOverlap(next_min_cell, next_max_cell)) {
					// the obj is too big, min and max both outside of the grid but a small part of the obj is inside the grid
					body.mGridState = GridState::Overlap;
					mOverlapGridList.emplace_back(entity);
				}
				else { // outside grid
					body.mGridState = GridState::Outside;
					mOutsideGridList.emplace_back(entity);
				}

				break;
			case GridState::Uninitialized:
				break;
			default:
				break;
			}
		}
	}

#endif
	// Check if the grid contains a specific cell.
	bool ImplicitGrid::GridContains(Cell const& cell) {
		return (cell.col >= 0 && cell.col < mCols && cell.row >= 0 && cell.row < mRows);
	}

	// Draw the grid for visualization purposes.
	void ImplicitGrid::DrawGrid() {
		//glm::vec2 camera_dimension = ISGraphics::cameras[Camera::mActiveCamera].GetCamDim();
		// getting the width and height of the camera
		auto& engine = InsightEngine::Instance();
		auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
		auto [x, y] = engine.IsFullScreen() ? engine.GetMonitorSize() : engine.GetWindowSize();
		// set the grid's width and height as 2 times bigger than camera's width and height
		float width = (static_cast<float>(x) * camera.GetZoomLevel()) * 1.f;
		float height = (static_cast<float>(y) * camera.GetZoomLevel()) * 1.f;

		/*float width = 12000.f;
		float height = 5000.f;*/
		ImplicitGrid::mCellSize = { static_cast<float>(width) / ImplicitGrid::mCols, static_cast<float>(height) / ImplicitGrid::mRows };

		glm::vec2 camera_center = ISGraphics::cameras3D[Camera3D::mActiveCamera].GetPosition2D();
		// start from top left
		float start_x = camera_center.x - width / 2.f;
		float start_y = camera_center.y + height / 2.f;

		// draw each row
		for (int i = 1; i < ImplicitGrid::mRows; i++) {
			Vector2D hori = { start_x, -(i * ImplicitGrid::mCellSize.y) + start_y };
			Vector2D horiend = { width,0.f };
			Sprite::drawDebugLine(hori, hori + horiend, { 1.f, 1.f, 1.f }, width, 0.f);
		}

		// draw each column
		for (int i = 1; i < ImplicitGrid::mCols; i++) {
			Vector2D vertical = { start_x + (i * ImplicitGrid::mCellSize.x), start_y };
			Vector2D verticalend = { 0.f, -height };
			Sprite::drawDebugLine(vertical, vertical + verticalend, { 1.f, 1.f, 1.f }, height, 90.f);
		}
	}

	// Emplace entities from the source vector into the result vector.
	void ImplicitGrid::EmplaceEntity(std::vector<Entity>& result, std::vector<Entity> const& source) {
		for (auto const& entity : source) {
			result.emplace_back(entity);
		}
	}

	// opertor + overload for adding two vector of entity
	std::vector<Entity> operator+(std::vector<Entity> const& lhs, std::vector<Entity> const& rhs) {
		std::vector<Entity> ret = lhs;;
		for (auto const& entity : rhs) {
			ret.emplace_back(entity);
		}
		return ret;
	}
}