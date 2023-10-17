

#pragma once
#include "Pch.h"

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

		static const int mRows = 5;
		static const int mCols = 5;
		static Vector2D mCellSize;
		static std::set<Entity> mCells[mRows][mCols];

	private:
	};
}