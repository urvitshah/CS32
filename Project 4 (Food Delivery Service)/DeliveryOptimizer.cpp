#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap * m_streetMap;
};

//constructor for DeliveryOptimizer that doesn't do anything
DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    m_streetMap = sm;
}

//destructor for DeliveryOptimizer that doesn't do anything since StreetMap pointer is deleted elsewhere
DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

//reorders delivery requests based on route with least distance
void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    //not enough time, so just calculates the distance
    if (deliveries.size()==0)
        return;
    oldCrowDistance += distanceEarthMiles(depot,deliveries[0].location);
    for (int a = 0; a < deliveries.size()-1; a++)
    {
        oldCrowDistance += distanceEarthMiles(deliveries[a].location,deliveries[a+1].location);
    }
    newCrowDistance = oldCrowDistance; 
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
