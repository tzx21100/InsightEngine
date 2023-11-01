/* Start Header **************************************************************/
/*!
 * \file Pathfinding.cpp
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * Definition of the Pathfinding class for managing pathfinding for AI (will be incoorporated in M3)
 *
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/
 /*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"

namespace IS {
    static int i = 0;

    std::string Pathfinding::GetName() {
        return "Pathfinding";
    }

    void Pathfinding::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
        Waypoint waypointA;
        Waypoint waypointB;
        Waypoint waypointC;
        waypointA.mPosition = Vector2D(0.f, 0.f);
        //waypointA.mNeighbors.emplace_back(&waypointB);
        waypointA.mIsObstacle = false;
        waypointB.mPosition = Vector2D(100.f, 10.f);
        //waypointB.mNeighbors.emplace_back(&waypointA);
        //waypointB.mNeighbors.emplace_back(&waypointC);
        waypointB.mIsObstacle = false;
        waypointC.mPosition = Vector2D(200.f, 0.f);
        //waypointC.mNeighbors.emplace_back(&waypointB);
        waypointC.mIsObstacle = false;

        ConnectWaypoints(waypointA, waypointB);
        ConnectWaypoints(waypointB, waypointC);
        
        AddWaypoint(waypointA);
        AddWaypoint(waypointB);
        AddWaypoint(waypointC);
    }

    void Pathfinding::Update([[maybe_unused]] float deltaTime) {
        for (auto const& entity : mEntities) {
            if (InsightEngine::Instance().HasComponent<Pathfinder>(entity)) {
                auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);

                if (i < mWaypoints.size()) {
                    Vector2D curr_waypoint = mWaypoints[i].mPosition;
                    Vector2D goal = curr_waypoint - trans.getWorldPosition();

                    if (goal.x || goal.y) {
                        // Entity has reached the current waypoint, find the next waypoint using A*
                        i++;
                    }

                    if (i < mWaypoints.size()) {
                        const Waypoint& currentWaypoint = mWaypoints[i];
                        const Waypoint& goalWaypoint = mWaypoints.back();

                        // Call A* pathfinding to get the path from the current waypoint to the goal
                        std::vector<Waypoint*> path = AStarPathfinding(currentWaypoint, goalWaypoint);

                        if (!path.empty()) {
                            // Move towards the next waypoint in the path
                            Vector2D nextWaypointPosition = path[0]->mPosition;
                            Vector2D nextGoal = nextWaypointPosition - trans.getWorldPosition();

                            trans.world_position += nextGoal * deltaTime;
                        }
                    }
                }

                if (i == mWaypoints.size()) {
                    // Entity has reached the last waypoint. You can handle this case here.
                    i = 0;
                }
            }
        }
    }


    void Pathfinding::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    double Pathfinding::DistanceBetweenWaypoints(const Waypoint& a, const Waypoint& b) {
        return ISVector2DDistance(a.mPosition, b.mPosition);
    }

    bool Pathfinding::ClosedListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>>& closed_list, Waypoint* waypoint) {
        while (!closed_list.empty()) {
            if (closed_list.top() == waypoint) {
                return true;
            }
            closed_list.pop();
        }
        return false;
    }

    bool Pathfinding::OpenListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>>& open_list, Waypoint* waypoint) {
        while (!open_list.empty()) {
            if (open_list.top() == waypoint) {
                return true;
            }
            open_list.pop();
        }
        return false;
    }

    std::vector<Waypoint*> Pathfinding::AStarPathfinding(const Waypoint& start, const Waypoint& goal) {

        // Create open and closed sets as priority queues
        std::priority_queue<Waypoint*, std::vector<Waypoint*>> open_list;
        std::priority_queue<Waypoint*, std::vector<Waypoint*>> closed_list;

        // Map to store g-cost values for each waypoint
        std::unordered_map<const Waypoint*, double> g_cost;

        // Map to store parent waypoints
        std::unordered_map<Waypoint*, Waypoint*> parent;

        // Initialize the open set with the start waypoint
        open_list.push(const_cast<Waypoint*>(&start));
        g_cost[&start] = 0.0;

        while (!open_list.empty()) {
            Waypoint* current = open_list.top();
            open_list.pop();

            // Add the current waypoint to the closed set
            closed_list.push(current);

            // If the current waypoint is the goal, reconstruct and return the path
            if (current == &goal) {
                std::vector<Waypoint*> path;
                while (current != &start) {
                    path.insert(path.begin(), current);
                    current = parent[current];
                }
                path.insert(path.begin(), const_cast<Waypoint*>(&start));
                return path;
            }

            // Loop through neighboring waypoints
            for (Waypoint* neighbor : current->mNeighbors) {
                if (neighbor->mIsObstacle) {
                    continue; // Skip if obstacle
                }
                if (ClosedListContains(closed_list, neighbor)) {
                    continue; // Skip waypoints in the closed set
                }

                double tentative_g_cost = g_cost[current] + DistanceBetweenWaypoints(*current, *neighbor);

                if (!OpenListContains(open_list, neighbor) || tentative_g_cost < g_cost[neighbor]) {
                    // Update the parent and g-cost for the neighbor
                    parent[neighbor] = current;
                    g_cost[neighbor] = tentative_g_cost;

                    if (!OpenListContains(open_list, neighbor)) {
                        open_list.push(neighbor);
                    }
                }
            }
        }

        return std::vector<Waypoint*>();
    }

    void Pathfinding::AddWaypoint(const Waypoint& waypoint) {
        mWaypoints.emplace_back(waypoint);
    }

    void Pathfinding::ConnectWaypoints(Waypoint& waypoint1, Waypoint& waypoint2) {
        if (!waypoint1.mIsObstacle && !waypoint2.mIsObstacle) {
            waypoint1.mNeighbors.emplace_back(&waypoint2);
            waypoint2.mNeighbors.emplace_back(&waypoint1);
        }
    }

    void Pathfinding::PerformPathfinding(Entity& entity, const Waypoint& start, const Waypoint& goal) {
        auto& engine = InsightEngine::Instance();

        if (engine.HasComponent<Pathfinder>(entity)) {
            // Perform pathfinding and store the result
            std::vector<Waypoint*> path = AStarPathfinding(start, goal);
            // Store the path in the system's mPathStorage
            mPathStorage[entity] = path;
        }
    }

    std::vector<Waypoint*> Pathfinding::GetPathForEntity(Entity entity)
    {
        if (mPathStorage.find(entity) != mPathStorage.end()) {
            return mPathStorage[entity];
        }
        return std::vector<Waypoint*>();
    }


}