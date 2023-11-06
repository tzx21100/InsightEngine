/*!
 * \file Grid.h
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

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_GRID_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_GRID_H

 /*                                                                   includes
   ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Sprite.h"

namespace IS
{
	/**
	 * \struct Cell
	 * \brief Represents a cell in the grid by its row and column.
	 */
	struct Cell {
		int row; //!< The row of the cell.
		int col; //!< The column of the cell.
	};

	/**
	 * \enum GridState
	 * \brief Enumerates the states of grid cells (Uninitialized, Inside, Outside, Overlap).
	 */
	enum class GridState : short {
		Uninitialized,
		Inside,
		Outside,
		Overlap
	};

	/**
	 * \class ImplicitGrid
	 * \brief Manages an implicit grid system for optimizing spatial partitioning.
	 */
	class ImplicitGrid
	{
	public:
		/**
		 * \brief Default ctor
		 */
		ImplicitGrid();

		/**
		 * \brief Clears the grid, resetting its state.
		 */
		void ClearGrid();

		/**
		 * \brief Get the cell corresponding to a given position in the grid.
		 * \param position The position for which to determine the cell.
		 * \return The cell representing the position.
		 */
		Cell GetCell(Vector2D const& position);

		/**
		 * \brief Check if two cells are equal.
		 * \param c1 The first cell.
		 * \param c2 The second cell.
		 * \return `true` if the cells are equal, `false` otherwise.
		 */
		bool areCellsEqual(Cell const& c1, Cell const& c2);

		/**
		 * \brief Check if multiple cells are equal.
		 * \param c1 The first cell.
		 * \param c2 The second cell.
		 * \param c3 The third cell.
		 * \param c4 The fourth cell.
		 * \return `true` if all cells are equal, `false` otherwise.
		 */
		bool areCellsEqual(Cell const& c1, Cell const& c2, Cell const& c3, Cell const& c4);

		/**
		 * \brief Add entities into the appropriate cell of the grid.
		 * \param Entities The entities to add into the grid.
		 */
		void AddIntoCell(std::set<Entity> const& Entities);

		/**
		 * \brief Add entities into the bit arrays representing grid cells.
		 * \param min The minimum cell.
		 * \param max The maximum cell.
		 * \param entity The entity to add.
		 */
		void AddToBitArray(Cell const& min, Cell const& max, Entity const& entity);

		/**
		 * \brief Remove entities from the bit arrays representing grid cells.
		 * \param min The minimum cell.
		 * \param max The maximum cell.
		 * \param entity The entity to remove.
		 */
		void RemoveFromBitArray(Cell const& min, Cell const& max, Entity const& entity);

		/**
		 * \brief Check if there is an overlap at the edge of the grid.
		 * \param min The minimum cell.
		 * \param max The maximum cell.
		 * \return `true` if there is an overlap, `false` otherwise.
		 */
		bool CheckOverlap(Cell const& min, Cell const& max);

		/**
		 * \brief Update the cell for an entity in the grid.
		 * \param entity The entity to update.
		 * \param dt The time step.
		 */
		void UpdateCell(Entity const& entity, float const& dt); // not in use

		/**
		 * \brief Check if the grid contains a specific cell.
		 * \param cell The cell to check.
		 * \return `true` if the grid contains the cell, `false` otherwise.
		 */
		bool GridContains(Cell const& cell);

		/**
		 * \brief Draw the grid for visualization purposes.
		 */
		static void DrawGrid();

		/**
		 * \brief Emplace entities from the source vector into the result vector.
		 * \param result The vector to store the emplaced entities.
		 * \param source The vector from which to emplace entities.
		 */
		void EmplaceEntity(std::vector<Entity>& result, std::vector<Entity> const& source);

		// for imgui
		static const int MIN_GRID_COLS = 1;
		static const int MAX_GRID_COLS = 20;
		static const int MIN_GRID_ROWS = 1;
		static const int MAX_GRID_ROWS = 20;

		static const int DEFAULT_GRID_COLS = 5;
		static const int DEFAULT_GRID_ROWS = 5;

		static int mRows;											// total number of rows for the grid
		static int mCols;											// total number of columns for the grid
		Vector2D mGridSize;											// width and height of the grid
		static Vector2D mCellSize;									// width and height of each cell
		std::bitset<MAX_ENTITIES> mRowsBitArray[MAX_GRID_ROWS];		// bitset of the row
		std::bitset<MAX_ENTITIES> mColsBitArray[MAX_GRID_COLS];		// bitset of the column
		std::vector<Entity> mInGridList;							// entity vector list for in grid entities
		std::vector<Entity> mOverlapGridList;						// entity vector list for overlap with grid entities
		std::vector<Entity> mOutsideGridList;						// entity vector list for outside grid entities

	private:

	};

	// opertor + overload for adding two vector of entity
	std::vector<Entity> operator+(std::vector<Entity> const& lhs, std::vector<Entity> const& rhs);

}

#endif