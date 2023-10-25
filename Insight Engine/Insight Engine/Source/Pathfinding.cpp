/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"

namespace IS {
    std::string Pathfinding::GetName() {
        return "Pathfinding";
    }

    void Pathfinding::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void Pathfinding::Update([[maybe_unused]] float deltaTime) {
        // Create and add waypoints
        //Waypoint waypointA;
        //Waypoint waypointB;
        //Waypoint waypointC;

        //// Add waypoints to the pathfinding system
        //AddWaypoint(waypointA);
        //AddWaypoint(waypointB);
        //AddWaypoint(waypointC);

        //// Connect waypoints to define the navigation graph
        //ConnectWaypoints(waypointA, waypointB);
        //ConnectWaypoints(waypointB, waypointC);

        //// Define a start and goal waypoint
        //Waypoint startWaypoint = waypointA;
        //Waypoint goalWaypoint = waypointC;

        //// Perform pathfinding for an entity
        //Entity entity; // Create an entity
        //PerformPathfinding(entity, startWaypoint, goalWaypoint);

        //// Get the path for the entity
        //std::vector<Waypoint*> path = GetPathForEntity(entity);

        //// Check if a valid path was found
        //if (!path.empty()) {
        //    // Path found, now you can use the path for entity navigation
        //    // The 'path' vector contains the ordered waypoints from start to goal.
        //    for (Waypoint* waypoint : path) {
        //        // Navigate the entity to 'waypoint'
        //        // You may have your own entity navigation logic here.
        //    }
        //}
        //else {
        //    // No path found, handle this case
        //}
    }

    void Pathfinding::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }

    double Pathfinding::distance_between_waypoints(const Waypoint& a, const Waypoint& b) {
        return ISVector2DDistance(a.mPosition, b.mPosition);
    }

    bool Pathfinding::ClosedListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &closed_list, Waypoint* waypoint) {
        while (!closed_list.empty()) {
            if (closed_list.top() == waypoint) {
                return true;
            }
            closed_list.pop();
        }
        return false;
    }

    // check if a waypoint is in the open set
    bool Pathfinding::OpenListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &open_list, Waypoint* waypoint) {
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

                double tentative_g_cost = g_cost[current] + distance_between_waypoints(*current, *neighbor);

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
    
    void Pathfinding::AddWaypoint(const Waypoint& waypoint){
        mWaypoints.push_back(waypoint);
    }

    void Pathfinding::ConnectWaypoints(Waypoint& waypoint1, Waypoint& waypoint2){
        if(!waypoint1.mIsObstacle && !waypoint2.mIsObstacle){
            waypoint1.mNeighbors.push_back(&waypoint2);
            waypoint2.mNeighbors.push_back(&waypoint1);
        }
    }

    void Pathfinding::PerformPathfinding(Entity& entity, const Waypoint& start, const Waypoint& goal){
        auto& engine = InsightEngine::Instance();

        if (engine.HasComponent<Pathfinder>(entity)) {
            // Perform pathfinding and store the result in the component
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