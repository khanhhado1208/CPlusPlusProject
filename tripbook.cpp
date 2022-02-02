#include "include/tripbook.h"

#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

void TripBook::reset()
{
    this->_trips.clear();
}

bool TripBook::save(string file)
{
    ofstream out(file);
    if (out.is_open())
    {
        out << *this << endl;
        return true;
    }
    else
    {
        return false;
    }
}

bool TripBook::read(string file)
{
    ifstream input(file);
    if(input.is_open())
    {
        cout<<"File opened successfully!!"<<endl;
        // loop to getline
        string line; 
        while(getline(input, line)){
            // split line to elements
            vector<string> v;
            char *tmpStr = strtok((char *)line.c_str(), " ");
            while (tmpStr != NULL) {
                v.push_back(string(tmpStr));
                tmpStr = strtok(NULL," ");
            }
            if(v.size() < 7){ // a line should have 7 columns data
                continue;
            }
            Place start(v[0], v[1], stof(v[2])); // create a place from input data
            Place stop(v[3], v[4], stof(v[5]));
            Trip::Ptr trip = make_shared<Trip>(start, stop, stof(v[6]));
        
            _trips.push_back(trip);
        }

        return true;
    } else{
        return false;
    }
}

bool TripBook::addFromStd()
{
    Trip::Ptr trip = make_shared<Trip>();
    cin >> *trip;

    _trips.push_back(trip);

    return true;
}

bool TripBook::add(Trip::Ptr trip)
{
    _trips.push_back(trip);
    return true;
}

bool TripBook::removeLast()
{
    if (_trips.empty())
        return false;

    _trips.pop_back();
   
    return true;
}

bool TripBook::remove(string name)
{
    _trips.remove_if([&name](const Trip::Ptr& ptrip){
        if(ptrip->start().name().find(name) != string::npos
            || ptrip->stop().name().find(name) != string::npos) {
                // found name in trip
                return true;
        }
        else {
            return false;
        }
    });

    return true;
}

float TripBook::avgFuel()
{

    int totalDistance = 0;
    float totalFuel = 0;
    for (auto trip : _trips)
    {
        totalDistance += trip->distance();
        totalFuel += trip->fuel();
    }

    return (totalFuel / totalDistance);
}

TripBook::Ptr TripBook::filterByDistance(TripBook::Ptr book, int min, int max)
{

    TripBook::Ptr sub = make_shared<TripBook>();

    for (auto trip : book->_trips)
    {
        if (trip->distance() >= min && trip->distance() <= max)
        {
            sub->add(trip);
        }
    }

    return sub;
}
