

#pragma once
#include "Pch.h"
#include "Sprite.h"

namespace IS
{
	class Grid 
	{
	public:

		Grid();

		void ClearGrid();

		static void AddIntoCell(Entity const& entity);
		void UpdateCell(Entity const& entity, float const& dt);

		void EmplaceEntity(std::set<Entity>& result, std::set<Entity> const& source);

		static void DrawGrid(Sprite const& sprite);
		static void ResizeGrid(int cols, int rows);

		static const int MIN_GRID_COLS = 2;
		static const int MAX_GRID_COLS = 20;
		static const int MIN_GRID_ROWS = 2;
		static const int MAX_GRID_ROWS = 20;

		static int mRows;
		static int mCols;
		static Vector2D mCellSize;
		static std::vector<std::vector<std::set<Entity>>> mCells;
		static std::set<Entity> mOutsideGridList;
	private:
	};
}