#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap *m_streetMap;
    
};

//initializes dynamically allocated StreetMap variable
DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_streetMap = sm;
}

//destructs dynamically allocated StreetMap variable
DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

//brings together all the classes to generate delivery requests
DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    //reorder the delivery requests to optimize them
    DeliveryOptimizer d(m_streetMap);
    double oldCrowDistance = 0;
    double newCrowDistance = 0;
    vector<DeliveryRequest> newDeliveries(deliveries);
    d.optimizeDeliveryOrder(depot, newDeliveries, oldCrowDistance, newCrowDistance);
    
    //create point to point routes from depot, to last delivery spot, and back to depot
    list<StreetSegment> route;
    double distance;
    commands.clear();
 
    //creates delivery commands for every delivery
    for (int deliveryCount = 0; deliveryCount < deliveries.size(); deliveryCount++)
    {
        DeliveryCommand dc;
        PointToPointRouter p2pr(m_streetMap);
        list<StreetSegment> route;
        double distance;
        //calculates route from one point to another
        if (deliveryCount == 0)
            p2pr.generatePointToPointRoute(depot, newDeliveries[0].location, route, distance);
        else
            p2pr.generatePointToPointRoute(newDeliveries[deliveryCount-1].location, newDeliveries[deliveryCount].location, route, distance);
        totalDistanceTravelled += distance;
        list<StreetSegment>::iterator prevSegment = route.begin();
        int count = 0;
        for (list<StreetSegment>::iterator it = route.begin(); it != route.end();)
        {
            string name = it->name;
            double angle = angleOfLine(*it);
            double distanceProceed = 0;
            //creates a proceed command as first command and continues to add to distance until the street name is different from the starting street name
            while (it != route.end() && it->name == name)
            {
                distanceProceed += distanceEarthMiles(it->start,it->end);
                it++;
                count++;
                if (count > 1)
                {
                    prevSegment++;
                }
            }
            //calculates direction of street based on angle of street
            if (angle >= 0 && angle < 22.5)
                dc.initAsProceedCommand("east",name,distanceProceed);
            else if (angle >= 22.5 && angle < 67.5)
                dc.initAsProceedCommand("northeast",name,distanceProceed);
            else if (angle >= 67.5 && angle < 112.5)
                dc.initAsProceedCommand("north",name,distanceProceed);
            else if (angle >= 112.5 && angle < 157.5)
                dc.initAsProceedCommand("northwest",name,distanceProceed);
            else if (angle >= 157.5 && angle < 202.5)
                dc.initAsProceedCommand("west",name,distanceProceed);
            else if (angle >= 202.5 && angle < 247.5)
                dc.initAsProceedCommand("southwest",name,distanceProceed);
            else if (angle >= 247.5 && angle < 292.5)
                dc.initAsProceedCommand("south",name,distanceProceed);
            else if (angle >= 292.5 && angle < 337.5)
                dc.initAsProceedCommand("southeast",name,distanceProceed);
            else if (angle >= 337.5)
                dc.initAsProceedCommand("east",name,distanceProceed);
            commands.push_back(dc);
            
           //checks if street names are different and if they are, creates a turn command
            if (it != route.end() && it->name != prevSegment->name)
            {
                angle = angleBetween2Lines(*prevSegment,*it);
                if (angle < 1 || angle > 359)
                    continue;
                else if (angle >= 1 && angle < 180)
                {
                    dc.initAsTurnCommand("left",it->name);
                    commands.push_back(dc);
                    it++;
                    count++;
                    if (count > 1)
                    {
                        prevSegment++;
                    }
                }
                else if(angle >= 180 && angle <= 359)
                {
                    dc.initAsTurnCommand("right",it->name);
                    commands.push_back(dc);
                    it++;
                    count++;
                    if (count > 1)
                    {
                        prevSegment++;
                    }

                }
            }
        }
        // one the iterator reaches the end of the route, a delivery command is printed
        dc.initAsDeliverCommand(newDeliveries[deliveryCount].item);
        commands.push_back(dc);
    }
    
    //creates delivery commands for route back to depot
    
    //calculates route from last delivery to depot
    PointToPointRouter p2pr(m_streetMap);
    p2pr.generatePointToPointRoute(newDeliveries[newDeliveries.size()-1].location, depot, route, distance);
    totalDistanceTravelled += distance;
        
    list<StreetSegment>::iterator prevSegment = route.begin();
    for (list<StreetSegment>::iterator it = route.begin(); it != route.end();)
    {
        if (it!=route.begin())
            prevSegment++;
        if (it == route.end())
            break;
        string name = it->name;
        double angle = angleOfLine(*it);
        double distanceProceed = 0;
        //creates a proceed command as first command and continues to add to distance until the street name is different from the starting street name
        while (it != route.end() && it->name == name)
        {
            distanceProceed += distanceEarthMiles(it->start,it->end);
            it++;
            if (it!=route.begin())
                prevSegment++;
        }
        string direction;
        //calculates direction of street based on angle of street
        if (angle >= 0 && angle < 22.5)
            direction = "east";
        else if (angle >= 22.5 && angle < 67.5)
            direction = "northeast";
        else if (angle >= 67.5 && angle < 112.5)
            direction = "north";
        else if (angle >= 112.5 && angle < 157.5)
            direction = "northwest";
        else if (angle >= 157.5 && angle < 202.5)
            direction = "west";
        else if (angle >= 202.5 && angle < 247.5)
            direction = "southwest";
        else if (angle >= 247.5 && angle < 292.5)
            direction = "south";
        else if (angle >= 292.5 && angle < 337.5)
            direction = "southeast";
        else if (angle >= 337.5)
            direction = "east";
        DeliveryCommand dc;
        dc.initAsProceedCommand(direction,name,distanceProceed);
        commands.push_back(dc);
        
        //checks if street names are different and if they are, creates a turn command
        if (it!=route.end() && it->name != prevSegment->name)
        {
            angle = angleBetween2Lines(*prevSegment,*it);
            if (angle < 1 || angle > 359)
                continue;
            else if (angle >= 1 && angle < 180)
            {
                dc.initAsTurnCommand("left",it->name);
                commands.push_back(dc);
                it++;
                if (it!=route.begin())
                    prevSegment++;
            }
            else if(angle >= 180 && angle <= 359)
            {
                dc.initAsTurnCommand("right",it->name);
                commands.push_back(dc);
                it++;
                if (it!=route.begin())
                    prevSegment++;
            }
        }
    }
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
