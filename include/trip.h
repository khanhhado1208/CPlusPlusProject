#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <cstddef>
#include <iomanip>
using namespace std;

const int MAX_WIDTH_COLUMN = 15;

class Place
{
public:
    Place() = default;
    Place(string name, string time, float odo)
        : _name(name), _time(time), _odometer(odo)
    {
    }

    const string &name() const { return _name; }
    const string &time() const { return _time; }
    float odometer() const { return _odometer; }

    float operator-(const Place &p); // find distance between p and the current one
    friend istream &operator>>(istream &in, Place &p);
    friend ostream &operator<<(ostream &in, const Place &p);

private:
    string _name;
    string _time;
    float _odometer;
};

class Trip
{
public:
    using Ptr = shared_ptr<Trip>; // using smart pointer for automatic memory realsing to avoid memory leaks

    Trip() = default;
    Trip(Place start, Place stop, float fuel)
        : _start(start), _stop(stop), _amount_of_fuel(fuel) {}

    ~Trip() = default;

    float distance()
    {
        return _stop - _start;
    }

    // const Place& start() { return _start; }
    const Place &start() const { return _start; }
    const Place &stop() const { return _stop; }
    const float &fuel() const { return _amount_of_fuel; }
    friend istream &operator>>(istream &in, Trip &trip);
    friend ostream &operator<<(ostream &out, const Trip &trip);

private:
    Place _start;
    Place _stop;
    float _amount_of_fuel;
};

inline float Place::operator-(const Place &p)
{
    return abs(this->_odometer - p._odometer);
}

inline istream &operator>>(istream &in, Place &p)
{
    cout << "enter name (string): ";
    in >> p._name;
    cout << "enter time (string, hh:mm): ";
    in >> p._time;
    while (1)
    {
        cout << "enter odometer (float or int):  \n";
        in >> p._odometer;
        if (!in)
        {
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number!!! Please try again\n\n";
            continue;
        }
        else
            break;
    }

    return in;
}

inline ostream &operator<<(ostream &out, const Place &p)
{
    // out << "Name: " << p._name << endl;
    // out << "Time: " << p._time << endl;
    // out << "Odometer: " << p._odometer << endl;
    out << internal << setw(MAX_WIDTH_COLUMN) << p._name << setw(MAX_WIDTH_COLUMN) << p._time << setw(MAX_WIDTH_COLUMN) << p._odometer;

    return out;
}

inline istream &operator>>(istream &in, Trip &trip)
{
    cout << "Start place: " << endl;
    in >> trip._start;

    cout << "Stop place: " << endl;
    in >> trip._stop;
    while (1)
    {
        cout << "Amount of fuel (float or int): \n";
        in >> trip._amount_of_fuel;
        if (!in)
        {
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number!!! Please try again\n\n";
            continue;
        }
        else
            break;
    }
    return in;
}

inline ostream &operator<<(ostream &out, const Trip &trip)
{
    // out << "Starts at: "  << endl << trip._start << endl;
    // out << "Stops at: " << endl << trip._stop << endl;
    // out << "The amount of fuel during the trip: " << trip._amount_of_fuel << endl;

    out << internal << trip._start << trip._stop << setw(MAX_WIDTH_COLUMN) << trip._amount_of_fuel << endl;

    return out;
}
