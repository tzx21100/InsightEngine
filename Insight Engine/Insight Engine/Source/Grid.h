

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

		static const int mRows = 5;
		static const int mCols = 5;
		Vector2D mGridSize;
		Vector2D mCellSize;
		std::bitset<MAX_ENTITIES> mRowsBitArray[mRows];
		std::bitset<MAX_ENTITIES> mColsBitArray[mCols];

		std::set<Entity> mInGridList;
		std::set<Entity> mOverlapGridList;
		std::set<Entity> mOutsideGridList;

	private:

	};


	class Grid 
	{
	public:

		Grid();

		void ClearGrid();

		static void AddIntoCell(Entity const& entity);
		void UpdateCell(Entity const& entity, float const& dt);

		void EmplaceEntity(std::set<Entity>& result, std::set<Entity> const& source);

		static void DrawGrid(Sprite const& sprite);

		static const int mRows = 5;
		static const int mCols = 5;
		static Vector2D mCellSize;
		static std::set<Entity> mCells[mRows][mCols];
		static std::set<Entity> mOutsideGridList;
	private:
	};
}