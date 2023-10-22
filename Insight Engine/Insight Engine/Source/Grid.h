

#pragma once
#include "Pch.h"
#include "Sprite.h"

namespace IS
{

	struct Cell {
		int row;
		int col;
	};

	enum class GridState : short {
		Uninitialized,
		Inside,
		Outside,
		Overlap
	};

	class ImplicitGrid
	{
	public:
		ImplicitGrid();

		void ClearGrid();

		Cell GetCell(Vector2D const& position);

		bool areCellsEqual(Cell const& c1, Cell const& c2);

		bool areCellsEqual(Cell const& c1, Cell const& c2, Cell const& c3, Cell const& c4);

		void AddIntoCell(std::set<Entity> const& Entities);

		void AddToBitArray(Cell const& min, Cell const& max, Entity const& entity);

		void RemoveFromBitArray(Cell const& min, Cell const& max, Entity const& entity);

		void UpdateCell(Entity const& entity, float const& dt);

		bool GridContains(Cell const& cell);

		static void DrawGrid(Sprite const& sprite);

		static const int MIN_GRID_COLS = 1;
		static const int MAX_GRID_COLS = 20;
		static const int MIN_GRID_ROWS = 1;
		static const int MAX_GRID_ROWS = 20;

		static const int DEFAULT_GRID_COLS = 6;
		static const int DEFAULT_GRID_ROWS = 6;

		static int mRows;
		static int mCols;
		Vector2D mGridSize;
		static Vector2D mCellSize;
		std::bitset<MAX_ENTITIES> mRowsBitArray[MAX_GRID_ROWS];
		std::bitset<MAX_ENTITIES> mColsBitArray[MAX_GRID_COLS];

		std::vector<Entity> mInGridList;
		std::vector<Entity> mOverlapGridList;
		std::vector<Entity> mOutsideGridList;

	private:

	};

#if 0
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

#endif
}