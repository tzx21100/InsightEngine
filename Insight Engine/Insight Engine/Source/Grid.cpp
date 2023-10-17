

#include "Pch.h"
#include "CoreEngine.h"

namespace IS
{
	Vector2D Grid::mCellSize = { WIDTH / mCols, HEIGHT / mRows };

	std::set<Entity> Grid::mCells[mRows][mCols];

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

	void Grid::AddIntoCell(Entity const& entity) {
		if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) { // if entity has rigidbody component
			//getting rigidbody component for each entity
			auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
			if (body.mBodyTransform.world_position.x >= -( WIDTH / 2) && body.mBodyTransform.world_position.x <= (WIDTH / 2) &&
				body.mBodyTransform.world_position.y >= -(HEIGHT / 2) && body.mBodyTransform.world_position.x <= (HEIGHT / 2)) {
				// set the body in grid as true
				body.mIsInGrid = true;
				
				// determine which grid cell the body is in (to be updated with camera move)
				if (body.mBodyTransform.scaling.x <= mCellSize.x && body.mBodyTransform.scaling.y <= mCellSize.y) {
					// the body size smaller than the cell size
					int col = static_cast<int>((body.mBodyTransform.world_position.x + (WIDTH / 2)) / mCellSize.x);
					int row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (HEIGHT / 2))) / mCellSize.y);

					if (col < 0 || col >= mCols || row < 0 || row >= mRows) {
						throw std::runtime_error("Entities outside of the camera view");
					}

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
						tl_col = static_cast<int>((body.mTransformedVertices[0].x + (WIDTH / 2)) / mCellSize.x);
						tl_row = static_cast<int>(std::abs((body.mTransformedVertices[0].y - (HEIGHT / 2))) / mCellSize.y);

						br_col = static_cast<int>((body.mTransformedVertices[2].x + (WIDTH / 2)) / mCellSize.x);
						br_row = static_cast<int>(std::abs((body.mTransformedVertices[2].y - (HEIGHT / 2))) / mCellSize.y);

						if (tl_col < 0 || tl_col >= mCols || tl_row < 0 || tl_row >= mRows) {
							throw std::runtime_error("Entities outside of the camera view");
						}

						if (br_col < 0 || br_col >= mCols || br_row < 0 || br_row >= mRows) {
							throw std::runtime_error("Entities outside of the camera view");
						}

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
			else {
				body.mIsInGrid = false;
			}
		}
	}

	// issue here
	void Grid::UpdateCell(Entity const& entity, float const& dt) {
		if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) { // if entityA has rigidbody component
			//getting rigidbody component for each entity
			auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
			auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
			int old_col = static_cast<int>((body.mBodyTransform.world_position.x + (WIDTH / 2)) / mCellSize.x);
			int old_row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (HEIGHT / 2))) / mCellSize.y);

			if (old_col < 0 || old_col >= mCols || old_col < 0 || old_row >= mRows) {
				throw std::runtime_error("Entities outside of the camera view");
			}

			body.mBodyTransform.world_position.x += body.mVelocity.x * dt;
			body.mBodyTransform.world_position.y += body.mVelocity.y * dt;

			int new_col = static_cast<int>((body.mBodyTransform.world_position.x + (WIDTH / 2)) / mCellSize.x);
			int new_row = static_cast<int>(std::abs((body.mBodyTransform.world_position.y - (HEIGHT / 2))) / mCellSize.y);

			if (new_col < 0 || new_col >= mCols || new_row < 0 || new_row >= mRows) {
				throw std::runtime_error("Entities outside of the camera view");
			}

			trans.world_position.x = body.mBodyTransform.world_position.x;
			trans.world_position.y = body.mBodyTransform.world_position.y;

			if (old_col == new_col && old_row == new_row) {
				return; // still in the same cell
			}

			// remove it from the old cell
			mCells[old_row][old_col].erase(entity);

			mCells[new_row][new_col].emplace(entity);
		}
	}

	void Grid::EmplaceEntity(std::set<Entity>& result, std::set<Entity> const& source) {
		for (auto const& entity : source) {
			result.emplace(entity);
		}
	}

}