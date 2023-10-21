

#include "Pch.h"
#include "CoreEngine.h"

namespace IS
{
	ImplicitGrid::ImplicitGrid() {

		for (int i = 0; i < mRows; ++i) {
			mRowsBitArray[i].reset();  // Reset all bits to 0
		}

		for (int i = 0; i < mCols; ++i) {
			mColsBitArray[i].reset();  // Reset all bits to 0
		}

		mInGridList;
		mOverlapGridList;
		mOutsideGridList;
	}

	void ImplicitGrid::ClearGrid() {
		// clear the grid and reset all bits to 0
		for (int i = 0; i < mRows; ++i) {
			mRowsBitArray[i].reset();  // Reset all bits to 0
		}

		for (int i = 0; i < mCols; ++i) {
			mColsBitArray[i].reset();  // Reset all bits to 0
		}
		mInGridList.clear();
		mOverlapGridList.clear();
		mOutsideGridList.clear();
	}

	Cell ImplicitGrid::GetCell(Vector2D const& position) {

		InsightEngine& engine = InsightEngine::Instance();
		// Set cell size based on current window size
		auto [width, height] = engine.GetSystem<WindowSystem>("Window")->GetWindowSize();
		mGridSize = { static_cast<float>(width), static_cast<float>(height) };
		mCellSize = { static_cast<float>(width) / mCols, static_cast<float>(height) / mRows };

		int col = static_cast<int>((position.x + (mGridSize.x / 2.f)) / mCellSize.x);
		int row = static_cast<int>(std::abs((position.y - (mGridSize.y / 2.f))) / mCellSize.y);

		return {row, col};
	}

	bool ImplicitGrid::areCellsEqual(Cell const& c1, Cell const& c2) {
		return (c1.row == c2.row == c1.col == c2.col);
	}

	bool ImplicitGrid::areCellsEqual(Cell const& c1, Cell const& c2, Cell const& c3, Cell const& c4) {
		return (c1.row == c2.row == c1.col == c2.col == c3.row == c4.row == c3.col == c4.col);
	}


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

					// to be update
					/*if (min_cell.col < 0 || min_cell.col >= mCols || min_cell.row < 0 || min_cell.row >= mRows ||
						max_cell.col < 0 || max_cell.col >= mCols || max_cell.row < 0 || max_cell.row >= mRows) {
						throw std::runtime_error("Entities outside of the camera view");
					}*/

					if (GridContains(min_cell) && GridContains(max_cell)) {
						body.mGridState = GridState::Inside;
						AddToBitArray(min_cell, max_cell, entity);
					}else if ((GridContains(min_cell) && !GridContains(max_cell)) ||
							(!GridContains(min_cell) && GridContains(max_cell))) {
						// body box overlap on the grid edge, half inside half outside
						body.mGridState = GridState::Overlap;
						mOverlapGridList.emplace(entity);
					}
					else { // body box fully outside of the grid
						body.mGridState = GridState::Outside;
						mOutsideGridList.emplace(entity);
					}

					//AddToBitArray(min_cell, max_cell, entity);

					//// check whether min and max are at the same cell
					//if (areCellsEqual(min_cell, max_cell)) {
					//	// assign the entity to the grid cell based on its position
					//	std::bitset<MAX_ENTITIES> mask = 1;
					//	// left shift the eneity into the bitset
					//	mRowsBitArray[min_cell.row] |= (mask << entity);
					//	mColsBitArray[min_cell.col] |= (mask << entity);
					//}
					//else { // the min and max are not at the same cell
					//	// add from min to max
					//	for (int row = min_cell.row; row >= max_cell.row; row--) {
					//		std::bitset<MAX_ENTITIES> mask = 1;
					//		mRowsBitArray[row] |= (mask << entity);
					//	}
					//	for (int col = min_cell.col; col <= max_cell.col; col++) {
					//		std::bitset<MAX_ENTITIES> mask = 1;
					//		mColsBitArray[col] |= (mask << entity);
					//	}
					//}

					// indicate that the entity is inside grid
					//body.mGridState = GridState::Inside;
				}
			}
		}
	}

	void ImplicitGrid::AddToBitArray(Cell const& min, Cell const& max, Entity const& entity) {
		//// check whether min and max are at the same cell
		//if (areCellsEqual(min, max)) {
		//	// assign the entity to the grid cell based on its position
		//	std::bitset<MAX_ENTITIES> mask = 1;
		//	// left shift the eneity into the bitset
		//	mRowsBitArray[min.row] |= (mask << entity);
		//	mColsBitArray[max.col] |= (mask << entity);
		//}
		//else { // the min and max are not at the same cell
		//	// add from min to max
		//	for (int row = min.row; row >= max.row; row--) {
		//		std::bitset<MAX_ENTITIES> mask = 1;
		//		mRowsBitArray[row] |= (mask << entity);
		//	}
		//	for (int col = min.col; col <= max.col; col++) {
		//		std::bitset<MAX_ENTITIES> mask = 1;
		//		mColsBitArray[col] |= (mask << entity);
		//	}
		//}

		// add from min to max
		for (int row = min.row; row >= max.row; row--) {
			std::bitset<MAX_ENTITIES> mask = 1;
			mRowsBitArray[row] |= (mask << entity);
		}
		for (int col = min.col; col <= max.col; col++) {
			std::bitset<MAX_ENTITIES> mask = 1;
			mColsBitArray[col] |= (mask << entity);
		}
	}

	void ImplicitGrid::RemoveFromBitArray(Cell const& min, Cell const& max, Entity const& entity) {
		//// check whether min and max are at the same cell
		//if (areCellsEqual(min, max)) {
		//	// assign the entity to the grid cell based on its position
		//	std::bitset<MAX_ENTITIES> mask = 1;
		//	// left shift the eneity into the bitset
		//	mRowsBitArray[min.row] |= (mask << entity);
		//	mColsBitArray[max.col] |= (mask << entity);
		//}
		//else { // the min and max are not at the same cell
		//	// add from min to max
		//	for (int row = min.row; row >= max.row; row--) {
		//		std::bitset<MAX_ENTITIES> mask = 1;
		//		mRowsBitArray[row] |= (mask << entity);
		//	}
		//	for (int col = min.col; col <= max.col; col++) {
		//		std::bitset<MAX_ENTITIES> mask = 1;
		//		mColsBitArray[col] |= (mask << entity);
		//	}
		//}

		// add from min to max
		for (int row = min.row; row >= max.row; row--) {
			std::bitset<MAX_ENTITIES> mask = 1;
			mRowsBitArray[row] ^= (mask << entity);
		}
		for (int col = min.col; col <= max.col; col++) {
			std::bitset<MAX_ENTITIES> mask = 1;
			mColsBitArray[col] ^= (mask << entity);
		}
	}


	void ImplicitGrid::UpdateCell(Entity const& entity, float const& dt) {
		InsightEngine& engine = InsightEngine::Instance();
		auto& body = engine.GetComponent<RigidBody>(entity);
		auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
		body.BodyFollowTransform(trans);
		// getting the min & max for checking which cell contain the entity
		Box prev_box = body.GetAABB();
		Cell prev_min_cell = GetCell(prev_box.min);
		Cell prev_max_cell = GetCell(prev_box.max);

		trans.world_position += body.mVelocity * dt;

		body.BodyFollowTransform(trans);
		Box next_box = body.GetAABB();
		Cell next_min_cell = GetCell(next_box.min);
		Cell next_max_cell = GetCell(next_box.max);

		switch (body.mGridState) // body's previous state for grid
		{
		case GridState::Inside: // if the body inside grid

			// if the body min & max move but still within the same cell
			if (areCellsEqual(prev_min_cell, next_min_cell, next_min_cell, next_max_cell)) {
				return; // no update on cell
			}

			// remove the cell from previous cell
			RemoveFromBitArray(prev_min_cell, prev_max_cell, entity);
			// if the body box still within the camera view
			if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
				AddToBitArray(next_min_cell, next_max_cell, entity);
			}
			else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
				(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
				// body box overlap on the grid edge, half inside half outside
				body.mGridState = GridState::Overlap;
				mOverlapGridList.emplace(entity);
			}
			else { // body box fully outside of the grid
				body.mGridState = GridState::Outside;
				mOutsideGridList.emplace(entity);
			}

			break;
		case GridState::Outside: // if body outside grid
			
			// remove from outside entity list
			mOutsideGridList.erase(entity);

			// inside grid
			if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
				AddToBitArray(next_min_cell, next_max_cell, entity);
			}
			// overlap grid
			else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
				(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
				// body box overlap on the grid edge, half inside half outside
				body.mGridState = GridState::Overlap;
				mOverlapGridList.emplace(entity);
			}
			else { // outside grid
				body.mGridState = GridState::Outside;
				mOutsideGridList.emplace(entity);
			}

			break;
		case GridState::Overlap: // if body overlap on grid

			// remove from outside entity list
			mOverlapGridList.erase(entity);

			// inside grid
			if (GridContains(next_min_cell) && GridContains(next_max_cell)) {
				AddToBitArray(next_min_cell, next_max_cell, entity);
			}
			// overlap grid
			else if ((GridContains(next_min_cell) && !GridContains(next_max_cell)) ||
				(!GridContains(next_min_cell) && GridContains(next_max_cell))) {
				// body box overlap on the grid edge, half inside half outside
				body.mGridState = GridState::Overlap;
				mOverlapGridList.emplace(entity);
			}
			else { // outside grid
				body.mGridState = GridState::Outside;
				mOutsideGridList.emplace(entity);
			}

			break;
		default:
			break;
		}
	}

	bool ImplicitGrid::GridContains(Cell const& cell) {
		return (cell.col >= 0 && cell.col < mCols && cell.row >= 0 && cell.row < mRows &&
			cell.col >= 0 && cell.col < mCols && cell.row >= 0 && cell.row < mRows);
	}




	Vector2D Grid::mCellSize{}; // default 0 because no window added to engine yet

	std::set<Entity> Grid::mCells[mRows][mCols];
	std::set<Entity> Grid::mOutsideGridList;

	Grid::Grid() {
		// clear the grid
		for (int i = 0; i < mRows; i++) {
			for (int j = 0; j < mCols; j++) {
				mCells[i][j].clear();
			}
		}
	}

	void Grid::ClearGrid() {
		// clear the grid
		for (int i = 0; i < mRows; i++) {
			for (int j = 0; j < mCols; j++) {
				mCells[i][j].clear();
			}
		}
	}
#if 0
	void Grid::AddIntoCell(Entity const& entity) {
		InsightEngine& engine = InsightEngine::Instance();

		if (engine.HasComponent<RigidBody>(entity)) { // if entity has rigidbody component

			// Set cell size based on current window size
			auto [width, height] = engine.GetSystem<WindowSystem>("Window")->GetWindowSize();
			mCellSize = { static_cast<float>(width) / mCols, static_cast<float>(height) / mRows };

			//getting rigidbody component for each entity
			auto& body = engine.GetComponent<RigidBody>(entity);
			// if the body is within the camera view
			if (body.mBodyTransform.world_position.x >= -(width / 2.f) && body.mBodyTransform.world_position.x <= (width / 2.f) &&
				body.mBodyTransform.world_position.y >= -(height / 2.f) && body.mBodyTransform.world_position.y <= (height / 2.f)) {
				// set the body in grid as true
				body.mIsInGrid = true;
				
				// determine which grid cell the body is in (to be updated with camera move)
				if (body.mBodyTransform.scaling.x <= mCellSize.x && body.mBodyTransform.scaling.y <= mCellSize.y) {
					// the body size smaller than the cell size
					int col = static_cast<int>((body.mBodyTransform.world_position.x + (width / 2.f)) / mCellSize.x);
					int row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (height / 2.f))) / mCellSize.y);

					mCells[row][col].emplace(entity);
				}
				else {
					// the body the size bigger than the cell size
					// init
					int tl_col;
					int tl_row;
					int br_col;
					int br_row;

					switch (body.mBodyShape)
					{
					case BodyShape::Box: // for box
						// get the row & col for top left and bottom right vertices
						tl_col = static_cast<int>((body.mTransformedVertices[0].x + (width / 2.f)) / mCellSize.x);
						tl_row = static_cast<int>(std::abs((body.mTransformedVertices[0].y - (height / 2.f))) / mCellSize.y);

						br_col = static_cast<int>((body.mTransformedVertices[2].x + (width / 2.f)) / mCellSize.x);
						br_row = static_cast<int>(std::abs((body.mTransformedVertices[2].y - (height / 2.f))) / mCellSize.y);

						// add from top left to bottom right
						for (int row = tl_row; row <= br_row; row++) {
							for (int col = tl_col; col <= br_col; col++) {
								mCells[row][col].emplace(entity);
							}
						}

						break;
					case BodyShape::Circle: // for circle
						break;
					default:
						break;
					}
				}
			}
			else { // body outside camera view
				body.mIsInGrid = false;
				mOutsideGridList.emplace(entity);
			}
		}
	}

	// issue here
	void Grid::UpdateCell(Entity const& entity, float const& dt) {
		InsightEngine& engine = InsightEngine::Instance();

		// Update cell size based on current window size
		auto [width, height] = engine.GetSystem<WindowSystem>("Window")->GetWindowSize();
		mCellSize = { static_cast<float>(width) / mCols, static_cast<float>(height) / mRows };

		if (engine.HasComponent<RigidBody>(entity)) { // if entityA has rigidbody component
			//getting rigidbody component for each entity
			auto& body = engine.GetComponent<RigidBody>(entity);
			auto& trans = engine.GetComponent<Transform>(entity);
			// if the body is within camera view
			if (body.mBodyTransform.world_position.x >= -(width / 2) && body.mBodyTransform.world_position.x <= (width / 2.f) &&
				body.mBodyTransform.world_position.y >= -(height / 2) && body.mBodyTransform.world_position.y <= (height / 2.f)) {

				body.mIsInGrid = true;

				if (body.mBodyTransform.scaling.x <= mCellSize.x && body.mBodyTransform.scaling.y <= mCellSize.y) {
					// the body size smaller than the cell size
					int old_col = static_cast<int>((body.mBodyTransform.world_position.x + (width / 2.f)) / mCellSize.x);
					int old_row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (height / 2.f))) / mCellSize.y);

					body.mBodyTransform.world_position.x += body.mVelocity.x * dt;
					body.mBodyTransform.world_position.y += body.mVelocity.y * dt;

					int new_col = static_cast<int>((body.mBodyTransform.world_position.x + (width / 2.f)) / mCellSize.x);
					int new_row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (height / 2.f))) / mCellSize.y);

					trans.world_position.x = body.mBodyTransform.world_position.x;
					trans.world_position.y = body.mBodyTransform.world_position.y;

					if (old_col == new_col && old_row == new_row) {
						return; // still in the same cell
					}

					// remove it from the old cell
					mCells[old_row][old_col].erase(entity);

					mCells[new_row][new_col].emplace(entity);
				}
				else {
					// the body the size bigger than the cell size

					// init
					int old_tl_col;
					int old_tl_row;
					int old_br_col;
					int old_br_row;

					int new_tl_col;
					int new_tl_row;
					int new_br_col;
					int new_br_row;

					switch (body.mBodyShape)
					{
					case BodyShape::Box: // for box
						// get the row & col for top left and bottom right vertices
						old_tl_col = static_cast<int>((body.mTransformedVertices[0].x + (width / 2.f)) / mCellSize.x);
						old_tl_row = static_cast<int>(std::abs((body.mTransformedVertices[0].y - (height / 2.f))) / mCellSize.y);

						old_br_col = static_cast<int>((body.mTransformedVertices[2].x + (width / 2)) / mCellSize.x);
						old_br_row = static_cast<int>(std::abs((body.mTransformedVertices[2].y - (height / 2.f))) / mCellSize.y);

						trans.world_position.x += body.mVelocity.x * dt;
						trans.world_position.y += body.mVelocity.y * dt;
						body.mTransformUpdateRequired = true;
						body.mTransformedVertices = body.GetTransformedVertices();

						new_tl_col = static_cast<int>((body.mTransformedVertices[0].x + (width / 2)) / mCellSize.x);
						new_tl_row = static_cast<int>(std::abs((body.mTransformedVertices[0].y - (height / 2.f))) / mCellSize.y);

						new_br_col = static_cast<int>((body.mTransformedVertices[2].x + (width / 2)) / mCellSize.x);
						new_br_row = static_cast<int>(std::abs((body.mTransformedVertices[2].y - (height / 2.f))) / mCellSize.y);

						if (old_tl_col == new_tl_col && old_tl_row == new_tl_row &&
							old_br_col == new_br_col && old_br_row == new_br_row) {
							return; // still in the same cell
						}

						// earse from top left to bottom right
						for (int row = old_tl_row; row <= old_br_row; row++) {
							for (int col = old_tl_col; col <= old_br_col; col++) {
								// remove it from the old cell
								mCells[row][col].erase(entity);
							}
						}

						for (int row = new_tl_row; row <= new_br_row; row++) {
							for (int col = new_tl_col; col <= new_br_col; col++) {
								// remove it from the old cell
								mCells[row][col].emplace(entity);
							}
						}

						break;
					case BodyShape::Circle: // for circle
						break;
					default:
						break;
					}
				}
			}
			else { // body outside camera view
				
				if (body.mIsInGrid) { // if body was inside grid last frame, remove it from the last cell
					body.mIsInGrid = false;
					body.mBodyTransform.world_position.x += body.mVelocity.x * dt;
					body.mBodyTransform.world_position.y += body.mVelocity.y * dt;
					trans.world_position.x = body.mBodyTransform.world_position.x;
					trans.world_position.y = body.mBodyTransform.world_position.y;
					if (body.mBodyTransform.scaling.x <= mCellSize.x && body.mBodyTransform.scaling.y <= mCellSize.y) {
						// the body size smaller than the cell size
						int col = static_cast<int>((body.mBodyTransform.world_position.x + (width / 2.f)) / mCellSize.x);
						int row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (height / 2.f))) / mCellSize.y);

						// remove it from the old cell
						//std::cout << row << std::endl;
						//std::cout << col << std::endl;
						row = (row >= mRows) ? mRows - 1 : row;
						row = (row < 0) ? 0 : row;
						col = (col >= mCols) ? mCols - 1 : col;
						col = (col < 0) ? 0 : col;
						
						mCells[row][col].erase(entity);

					}
					else {
						// the body the size bigger than the cell size
						body.mTransformUpdateRequired = true;
						body.mTransformedVertices = body.GetTransformedVertices();
						// init
						int tl_col;
						int tl_row;
						int br_col;
						int br_row;

						switch (body.mBodyShape)
						{
						case BodyShape::Box: // for box
							// get the row & col for top left and bottom right vertices
							tl_col = static_cast<int>((body.mTransformedVertices[0].x + (width / 2.f)) / mCellSize.x);
							tl_row = static_cast<int>(std::abs((body.mTransformedVertices[0].y - (height / 2.f))) / mCellSize.y);

							br_col = static_cast<int>((body.mTransformedVertices[2].x + (width / 2.f)) / mCellSize.x);
							br_row = static_cast<int>(std::abs((body.mTransformedVertices[2].y - (height / 2.f))) / mCellSize.y);

							// earse from top left to bottom right
							for (int row = tl_row; row <= br_row; row++) {
								for (int col = tl_col; col <= br_col; col++) {
									// remove it from the old cell
									row = (row >= mRows) ? mRows - 1 : row;
									row = (row < 0) ? 0 : row;
									col = (col >= mCols) ? mCols - 1 : col;
									col = (col < 0) ? 0 : col;
									mCells[row][col].erase(entity);
								}
							}
							break;
						case BodyShape::Circle: // for circle
							break;
						default:
							break;
						}
					}
				}
				else {
					trans.world_position.x += body.mVelocity.x * dt;
					trans.world_position.y += body.mVelocity.y * dt;
				}
			}
		}
	}
#endif
	// certain issues in the function above:
	// 1. everytime the obj go in / out of the grid need check its mIsInGrid data, need update 
	// if the obj is inside grid with mIsInGrid false or outside grid with mIsInGrid true
	// 2. planning to add a mOutsideGridList to keep tract on the entities outside of the grid
	// 3. code need clean up

	void Grid::EmplaceEntity(std::set<Entity>& result, std::set<Entity> const& source) {
		for (auto const& entity : source) {
			result.emplace(entity);
		}
	}

	void Grid::DrawGrid(Sprite const& sprite) {
		auto [width, height] = InsightEngine::Instance().GetSystem<WindowSystem>("Window")->GetWindowSize();
		Grid::mCellSize = { static_cast<float>(width) / Grid::mCols, static_cast<float>(height) / Grid::mRows };

		for (int i = 0; i < Grid::mRows; i++) {
			for (int j = 0; j < Grid::mCols; j++) {
				Vector2D vertical = { (j * Grid::mCellSize.x) - (width / 2), -(i * Grid::mCellSize.y) + (height / 2) };
				Vector2D verticalend = { 0.f,-(Grid::mCellSize.y) };
				sprite.drawLine(vertical, vertical + verticalend);
				Vector2D hori = { (j * Grid::mCellSize.x) - (width / 2), -(i * Grid::mCellSize.y) + (height / 2) };
				Vector2D horiend = { Grid::mCellSize.x, 0.f };
				sprite.drawLine(hori, hori + horiend);
			}
		}
	}
}