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
#include "Pathfinding.h"
#include "Engine/Core/CoreEngine.h"
#include "Graphics/System/Transform.h"

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
        waypointA.mNeighbors.emplace_back(&waypointB);
        waypointA.mIsObstacle = false;
        waypointB.mPosition = Vector2D(100.f, 100.f);
        waypointB.mNeighbors.emplace_back(&waypointA);
        waypointB.mNeighbors.emplace_back(&waypointC);
        waypointB.mIsObstacle = false;
        waypointC.mPosition = Vector2D(200.f, 0.f);
        waypointC.mNeighbors.emplace_back(&waypointB);
        waypointC.mIsObstacle = false;

       /* ConnectWaypoints(waypointA, waypointB);
        ConnectWaypoints(waypointB, waypointC);*/
        
        AddWaypoint(waypointA);
        AddWaypoint(waypointB);
        AddWaypoint(waypointC);
    }

    void Pathfinding::Update(float deltaTime) {
        for (auto const& entity : mEntities) {
            if (InsightEngine::Instance().HasComponent<Pathfinder>(entity)) {
                auto& pathfinder = InsightEngine::Instance().GetComponent<Pathfinder>(entity);
                auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);

                // Find the waypoints closest to the start and end positions
                Waypoint* startWaypoint = FindClosestWaypoint(pathfinder.mInitPos);
                Waypoint* endWaypoint = FindClosestWaypoint(pathfinder.mEndPos);

                if (!startWaypoint || !endWaypoint) {
                    // Handle error: No waypoints found
                    IS_CORE_DEBUG("NO WAYPOINTS FOUND");
                    continue;
                }

                // Call A* pathfinding to get the path from the start waypoint to the goal
                std::vector<Waypoint*> path = AStarPathfinding(*startWaypoint, *endWaypoint);

                if (!path.empty()) {
                    // Move towards the next waypoint in the path
                    Vector2D nextWaypointPosition = path.front()->mPosition;
                    Vector2D goal = nextWaypointPosition - trans.getWorldPosition();

                    if ((goal.x > -10.f && goal.x < 10.f) && (goal.y > -10.f && goal.y < 10.f)) {
                        // Entity has reached the waypoint. You can handle this case here.
                        path.erase(path.begin());
                    }

                    if (!path.empty()) {
                        Vector2D nextGoal = path.front()->mPosition - trans.getWorldPosition();
                        trans.world_position += nextGoal * deltaTime *2;
                    }
                }
            }
        }
    }

    Waypoint* Pathfinding::FindClosestWaypoint(const Vector2D& position) {
        Waypoint* closest = nullptr;
        double minDistance = std::numeric_limits<double>::max();

        for (auto& waypoint : mWaypoints) {
            double distance = DistanceBetweenPoints(position, waypoint.mPosition);
            if (distance < minDistance) {
                minDistance = distance;
                closest = &waypoint;
            }
        }

        return closest;
    }

    double Pathfinding::DistanceBetweenPoints(const Vector2D& a, const Vector2D& b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
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

    // Modify the OpenListContains and ClosedListContains to use the new data structures
    bool Pathfinding::ClosedListContains(std::unordered_set<Waypoint*>& closed_list, Waypoint* waypoint) {
        return closed_list.find(waypoint) != closed_list.end();
    }

    bool Pathfinding::OpenListContains(std::priority_queue<std::pair<Waypoint*, double>, std::vector<std::pair<Waypoint*, double>>, CompareCost>& open_list, Waypoint* waypoint) {
        auto temp = open_list;
        while (!temp.empty()) {
            if (temp.top().first == waypoint) {
                return true;
            }
            temp.pop();
        }
        return false;
    }

    std::vector<Waypoint*> Pathfinding::AStarPathfinding(const Waypoint& start, const Waypoint& goal) {
        std::priority_queue<std::pair<Waypoint*, double>, std::vector<std::pair<Waypoint*, double>>, CompareCost> open_list;
        std::unordered_set<Waypoint*> closed_list;
        std::unordered_map<const Waypoint*, double> g_cost;
        std::unordered_map<Waypoint*, Waypoint*> parent;
        std::unordered_set<Waypoint*> open_set;  // Additional set to check for existence in open list

        // Initialize g_cost with a high value for all waypoints
        for (const auto& waypoint : mWaypoints) {
            g_cost[&waypoint] = std::numeric_limits<double>::max();
        }

        // Initialize the open set with the start waypoint and its cost
        open_list.push({ const_cast<Waypoint*>(&start), 0.0 });
        open_set.insert(const_cast<Waypoint*>(&start));  // Add to open set
        g_cost[&start] = 0.0;

        while (!open_list.empty()) {
            Waypoint* current = open_list.top().first;
            open_list.pop();
            open_set.erase(current);  // Remove from open set
            closed_list.insert(current);  // Add to closed set

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
                if (neighbor->mIsObstacle || closed_list.find(neighbor) != closed_list.end()) {
                    continue;  // Skip if obstacle or in the closed set
                }

                double tentative_g_cost = g_cost[current] + DistanceBetweenWaypoints(*current, *neighbor);
                double h_cost = DistanceBetweenWaypoints(*neighbor, goal);
                double f_cost = tentative_g_cost + h_cost;

                if (open_set.find(neighbor) == open_set.end() || tentative_g_cost < g_cost[neighbor]) {
                    parent[neighbor] = current;
                    g_cost[neighbor] = tentative_g_cost;

                    if (open_set.find(neighbor) == open_set.end()) {
                        open_list.push({ neighbor, f_cost });
                        open_set.insert(neighbor);  // Add to open set
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