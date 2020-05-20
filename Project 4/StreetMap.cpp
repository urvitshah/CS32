#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <iostream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
};

//constructor that does nothing
StreetMapImpl::StreetMapImpl()
{
}

//destructor that does nothing because there are no dynamically allocated variables
StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    //gets the passed in text file
    ifstream infile(mapFile);
    
    //checks that the file was successfully found
    if (!infile)
    {
        cerr << "ERROR could not open " << mapFile << endl;
        return false;
    }
    
    string name;
    //goes through entire textfile, line by line
    while (getline(infile,name))
    {
        string s;
        int num;
        getline(infile,s);
        num = stoi(s);
        //goes through line with coordinates on it and separates each coordinate
        for (int a = 0; a < num; a++)
        {
            getline(infile,s);
            string s1 = s.substr(0,s.find(" "));
            
            s = s.substr(s.find(" ")+1);
            string s2 = s.substr(0,s.find(" "));
            
            s = s.substr(s.find(" ")+1);
            string s3 = s.substr(0,s.find(" "));
            
            s = s.substr(s.find(" ")+1);
            string s4 = s.substr(0,s.find(" "));
        
            //creates geocoords and streetsegments
            GeoCoord b(s1,s2);
            GeoCoord e(s3,s4);
            StreetSegment s(b,e,name);
            StreetSegment r(e,b,name);
            
            //adds associations to expandablehashmap
            const vector<StreetSegment> *v;
            v = m_map.find(b);
            if (v == nullptr)
            {
                vector<StreetSegment> v1;
                v1.push_back(s);
                m_map.associate(b,v1);
            }
            else
            {
                vector<StreetSegment> v1 = *v;
                v1.push_back(s);
                m_map.associate(b,v1);
            }
            
            v = m_map.find(e);
            if (v == nullptr)
            {
                vector<StreetSegment> v1;
                v1.push_back(r);
                m_map.associate(e,v1);
            }
            else
            {
                vector<StreetSegment> v1 = *v;
                v1.push_back(r);
                m_map.associate(e,v1);
            }
        }
    }
    return true;
}

//gets the streetsegments that are associated with the given geocoord
bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment> *v = m_map.find(gc);
    //return false if the geocoord could not be found
    if (v == nullptr)
        return false;
    segs = *v;
    return true;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
