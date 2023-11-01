/* Start Header **************************************************************/
/*!
 * \file Pathfinding.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * Declaration of the Pathfinding class for managing pathfinding for AI (will be incoorporated in M3)
 *
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/
 /*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "Pathfinder.h"

namespace IS {
    
    /*!
     * \brief Waypoint class representing a navigation point in the pathfinding system.
     */
    class Waypoint {
    public:
        /*!
         * \brief Default constructor for Waypoint.
         */
        Waypoint() {
            mPosition = { 0,0 };
            mNeighbors.clear();
            mIsObstacle = false;
        };

        /*!
         * \brief Single argument constructor
         * \param obstacle Set to `true` if the waypoint is an obstacle
         */
        Waypoint(bool obstacle) {
            mIsObstacle = obstacle;
        };

        Waypoint(Vector2D pos, std::vector<Waypoint*> neighbors ,bool obstacle) {
            mPosition = pos;
            mNeighbors = neighbors;
            mIsObstacle = obstacle;
        };

        //int mId{}; //id of waypoint
        Vector2D mPosition; //pos of waypoint
        std::vector<Waypoint*> mNeighbors; //neighbouring waypoint
        bool mIsObstacle; //is this an obstacle
    };

    /*!
     * \brief Pathfinding class responsible for managing pathfinding operations.
     */
    class Pathfinding : public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        /*!
         * \brief Calculate the distance between two waypoints.
         * \param a The first waypoint.
         * \param b The second waypoint.
         * \return The distance between the two waypoints.
         */
        double DistanceBetweenWaypoints(const Waypoint& a, const Waypoint& b);

        /*!
        * \brief Check if a waypoint is in the closed queue.
        * \param closed_list The closed queue of waypoints.
        * \param waypoint The waypoint to check.
        * \return `true` if the waypoint is in the closed queue, `false` otherwise.
        */
        bool ClosedListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &closed_list, Waypoint* waypoint);
        
        /*!
         * \brief Check if a waypoint is in the open queue.
         * \param open_list The open queue of waypoints.
         * \param waypoint The waypoint to check.
         * \return `true` if the waypoint is in the open queue, `false` otherwise.
         */
        bool OpenListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &open_list, Waypoint* waypoint);

        /*!
         * \brief Find a path using the A* algorithm.
         * \param start The starting waypoint.
         * \param goal The goal waypoint.
         * \return A vector of waypoints representing the path from the start to the goal.
         */
        std::vector<Waypoint*> AStarPathfinding(const Waypoint& start, const Waypoint& goal);

        /*!
         * \brief Add a waypoint to the pathfinding system.
         * \param waypoint The waypoint to add.
         */
        void AddWaypoint(const Waypoint& waypoint);

        /*!
         * \brief Connect two waypoints in the navigation graph. [not used/wip]
         * \param waypoint1 The first waypoint to connect.
         * \param waypoint2 The second waypoint to connect.
         */
        void ConnectWaypoints(Waypoint& waypoint1, Waypoint& waypoint2);

        /*!
         * \brief Perform pathfinding for an entity [not used/wip]
         * \param entity The entity for which to perform pathfinding.
         * \param start The starting waypoint.
         * \param goal The goal waypoint.
         */
        void PerformPathfinding(Entity& entity, const Waypoint& start, const Waypoint& goal);

        /*!
         * \brief Get the path for a specific entity. [not used/wip]
         * \param entity The entity for which to retrieve the path.
         * \return A vector of waypoints representing the path for the entity.
         */
        std::vector<Waypoint*> GetPathForEntity(Entity entity);

    private:
        std::vector<Waypoint> mWaypoints{}; // Store all waypoints
        std::unordered_map<Entity, std::vector<Waypoint*>> mPathStorage{}; // Store paths for entities
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H  