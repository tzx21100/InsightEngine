

#include "Pch.h"
#include "CoreEngine.h"

namespace IS
{
	int Grid::mCols = 5;
	int Grid::mRows = 5;
	Vector2D Grid::mCellSize{}; // default 0 because no window added to engine yet

	std::vector<std::vector<std::set<Entity>>> Grid::mCells;
	std::set<Entity> Grid::mOutsideGridList;

	Grid::Grid() {
		ResizeGrid(mRows, mCols);
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

	void Grid::AddIntoCell(Entity const& entity) {
		InsightEngine& engine = InsightEngine::Instance();

		if (engine.HasComponent<RigidBody>(entity)) { // if entity has rigidbody component

			// Set cell size based on current window size
			auto [width, height] = engine.GetWindowSize();
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
					std::cout << body.mBodyTransform.world_position.x << std::endl;
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
		auto [width, height] = engine.GetWindowSize();
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
						std::cout << row << std::endl;
						std::cout << col << std::endl;
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
		auto [width, height] = InsightEngine::Instance().GetWindowSize();
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

	void Grid::ResizeGrid(int cols, int rows) {
		mCols = cols;
		mRows = rows;
		mCells.resize(rows, std::vector<std::set<Entity>>(cols));
	}
}