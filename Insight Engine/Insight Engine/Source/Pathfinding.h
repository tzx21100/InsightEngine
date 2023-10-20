/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include "Pathfinder.h"

namespace IS {
    struct Waypoint {
        int mId;
        Vector2D mPosition;
        std::vector<Waypoint*> mNeighbors;
    };


    //// Define a custom comparison function for waypoints in the open set
    //class CompareWaypoints {
    //public:
    //    //default ctor
    //    CompareWaypoints() = default;
    //    
    //    CompareWaypoints(std::unordered_map<const Waypoint*, double>& g_cost, const Waypoint& goal)
    //        : mGCost(g_cost), mGoal(goal) {}

    //    bool operator()(const Waypoint* a, const Waypoint* b) {
    //        return mGCost[a] + distance(*a, mGoal) > mGCost[b] + distance(*b, mGoal);
    //    }

    //private:
    //    std::unordered_map<const Waypoint*, double>& mGCost;
    //    const Waypoint& mGoal;
    //};



    class Pathfinding : public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

        double distance_between_waypoints(const Waypoint& a, const Waypoint& b);

        // check if a waypoint is in the closed queue
        bool ClosedListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &closed_list, Waypoint* waypoint);
        
        // check if a waypoint is in the open queue
        bool OpenListContains(std::priority_queue<Waypoint*, std::vector<Waypoint*>> &open_list, Waypoint* waypoint);

        // find a path using A* algorithm
        std::vector<Waypoint*> FindPath(const Waypoint& start, const Waypoint& goal);

        // add a waypoint
        void AddWaypoint(const Waypoint& waypoint);

        // connect two waypoints
        void ConnectWaypoints(Waypoint& waypoint1, Waypoint& waypoint2);

        // perform pathfinding and update the Pathfinder component
        void PerformPathfinding(Entity& entity, const Waypoint& start, const Waypoint& goal);

        // get path for a specific entity
        std::vector<Waypoint*> GetPathForEntity(Entity entity);

    private:
        std::vector<Waypoint> mWaypoints{}; // Store all waypoints
        std::unordered_map<Entity, std::vector<Waypoint*>> mPathStorage{}; // Store paths for entities
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDING_H  