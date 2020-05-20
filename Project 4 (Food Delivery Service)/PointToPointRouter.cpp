#include "provided.h"
#include <list>
#include <queue>
#include "ExpandableHashMap.h"
using namespace std;

unsigned int hasher(const GeoCoord& g);

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap * m_streetMap;
};

//initializes dynamically allocated StreetMap variable
PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_streetMap = sm;
}

//does nothing because StreetMap pointer is deleted elsewhere
PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    vector<StreetSegment> segments;
    //checks that both the start and end coordinates can be found in the mapping data
    if (!m_streetMap->getSegmentsThatStartWith(start, segments) || !m_streetMap->getSegmentsThatStartWith(end, segments))
        return BAD_COORD;
    
    //checks if the start and end coordinates are the same which means that the user has already reached the destination
    if (start == end)
    {
        totalDistanceTravelled = 0;
        route.clear();
        return DELIVERY_SUCCESS;
    }
    
    queue<GeoCoord> geoCoordQueue;
    vector<GeoCoord> visited;
    ExpandableHashMap<GeoCoord,StreetSegment> g;
    totalDistanceTravelled = 0;
    
    //add starting coordinate and associated end coordinates to queue
    m_streetMap->getSegmentsThatStartWith(start,segments);
    geoCoordQueue.push(start);
    visited.push_back(start);
    for (int a = 0; a < segments.size(); a++)
    {
        geoCoordQueue.push(segments[a].end);
        g.associate(segments[a].end,segments[a]);
        visited.push_back(segments[a].end);
    }
    
    //loops until all coordinates have been visited or until the end coordinate has been reached
    while (!geoCoordQueue.empty())
    {
        GeoCoord current = geoCoordQueue.front();
        //cout << current.latitude << " " << current.longitude << endl;
        geoCoordQueue.pop();
        m_streetMap->getSegmentsThatStartWith(current,segments);
        
        //checks if the geocoord has reached the end
        if (current == end)
        {
            //extract segement by segement directions
            GeoCoord currentStart = start;
            StreetSegment currentStreet = *g.find(end);
            if (g.find(end) == nullptr)
                return NO_ROUTE;
            while (currentStreet.start != start)
            {
                totalDistanceTravelled += distanceEarthMiles(currentStreet.start,currentStreet.end);
                route.push_front(currentStreet);
                currentStreet = *g.find(currentStreet.start);
                if (g.find(currentStreet.start) == nullptr)
                    return NO_ROUTE;
            }
            return DELIVERY_SUCCESS;
        }
        
        //adds segments associated with current coordinate to the expandablehashmap
        for (int a = 0; a < segments.size(); a++)
        {
            bool add = true;
            for (int b = 0; b <visited.size(); b++)
            {
                //only adds segement to expandableHashMap if it isn't already there
                if (segments[a].end == visited[b])
                    add = false;
            }
            if (add)
            {
                geoCoordQueue.push(segments[a].end);
                g.associate(segments[a].end,segments[a]);
                visited.push_back(segments[a].end);
                //cout << "segments ";
                //cout << segments[a].end.latitude << " " << segments[a].end.longitude << endl;
            }
        }
    }
    
   //there is no route if the queue becomes empty without ever reaching the end coordinate
    return NO_ROUTE;  
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
