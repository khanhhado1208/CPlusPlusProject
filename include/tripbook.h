#pragma once

#include <list>
#include <iostream>
#include <memory>

#include "trip.h"

using namespace std;
class TripBook
{
public:
    using Ptr = shared_ptr<TripBook>;

    TripBook() = default;
    ~TripBook() = default;

    void reset(); // delete all trips

    bool save(string file);

    bool read(string file);

    bool addFromStd(); // add from screen

    bool add(Trip::Ptr trip); // add trip

    bool removeLast();

    bool remove(string name);

    float avgFuel();

    static TripBook::Ptr filterByDistance(TripBook::Ptr, int min, int max);

    friend ostream &operator<<(ostream &out, const TripBook &book);
    friend istream &operator>>(istream &out, TripBook &book);

private:
    list<Trip::Ptr> _trips;
};

inline ostream &operator<<(ostream &out, const TripBook &book)
{
    out<<internal;
    out.width(MAX_WIDTH_COLUMN);
    out << "StartName";
    out.width(MAX_WIDTH_COLUMN);
    out << "StartTime";
    out.width(MAX_WIDTH_COLUMN);
    out << "StartOdo";
    out.width(MAX_WIDTH_COLUMN);
    out << "StopName";
    out.width(MAX_WIDTH_COLUMN);
    out << "StopTime";
    out.width(MAX_WIDTH_COLUMN);
    out << "StopOdo";
    out.width(MAX_WIDTH_COLUMN);
    out << "AmountOfFuel";
    out<<endl;

    for (const auto &trip : book._trips)
    {
        out << *trip << endl; // print object
    }

    return out;
}

inline istream &operator>>(istream &in, TripBook &book)
{
    do
    {
        cout << "Enter trip name (empty to exit): " << endl;
        string inputString;
        getline(cin, inputString);
        if (inputString.empty())
            break;

        Trip::Ptr ptr = make_shared<Trip>();
        in >> *ptr;
        book.add(ptr);

    } while (1);

    return in;
}
