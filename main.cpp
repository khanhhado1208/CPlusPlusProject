#include <atomic>

#include "include/menu.h"
#include "include/tripbook.h"

using namespace std;

const string input_file = "trips_in.txt";
const string output_file = "trips_out.txt";

atomic_bool isrunning{false};

//===============================================================================
class BaseTripOption : public BaseOption
{
public:
    BaseTripOption(IBaseMenu *parent, string name, TripBook::Ptr ptr)
        : BaseOption(parent, name), _pBook(ptr)
    {
    }

    virtual ~BaseTripOption() = default;

protected:
    shared_ptr<TripBook> _pBook;
};

class BaseTripMenu : public BaseMenu
{
public:
    BaseTripMenu(IBaseMenu *parent, string name, TripBook::Ptr ptr)
        : BaseMenu(parent, name), _pBook(ptr)
    {
    }
    virtual ~BaseTripMenu() = default;

protected:
    shared_ptr<TripBook> _pBook;
};

//==============================================================================
class ExitOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    virtual void onSelected() override
    {
        isrunning = false;
    }
};

class InitializeOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    virtual void onSelected() override
    {
        _pBook->reset();
        strError = "tripbook is reset";
    }
};

class SaveFileOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    virtual void onSelected() override
    {
        if (_pBook->save(output_file))
        {
            strError = "saved to file";
        }
        else
        {
            strError = "fail to save to file";
        }
    }
};

class ReadFileOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    void dump()
    {
        cout << "Not message" << endl;
    }
    virtual void onSelected() override
    {
        if (_pBook->read(input_file))
        {
            strError = "read to file";
        }
        else
        {
            strError = "fail to read file";
        }
    }
};

class AddNewTripMenu : public BaseTripMenu
{
public:
    using BaseTripMenu::BaseTripMenu;

    void dump()
    {
        cout << "Enter any key to contine: ";
    }

    void process(const string &str) override
    {

        if (_pBook->addFromStd())
            strError = "Added a new trip";
        else
            strError = "Fail to add a new trip";
    }
};

class RemoveLastTripOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    virtual void onSelected() override
    {
        _pBook->removeLast();
    }
};

class RemoveTripMenu : public BaseTripMenu
{
public:
    using BaseTripMenu::BaseTripMenu;

    void dump()
    {
        cout << "Please enter startname" << endl
             << ": ";
    }

    void process(const string &str)
    {
        if (_pBook->remove(str))
        {
            strError = "Removed a trip with startname: " + str;
        }
        else
        {
            strError = "Fail to remove trip with startname: " + str;
        }
    }
};

class ReportTripMenu : public BaseTripMenu
{
public:
    using BaseTripMenu::BaseTripMenu;

    void dump()
    {
        cout << "Please enter distance range include 2 values [min(int), max(int)], example: (10, 100)" << endl
             << ": ";
    }

    void process(const string &str)
    {
        vector<int32_t> values;
        Splitter::exec(str, ",", values);
        if (values.size() < 2)
        {
            strError = "Require input 2 values of min and max";
            return;
        }

        if (values.at(0) > values.at(1))
        {
            strError = "Input wrong values of min and max";
            return;
        }
        auto ptr = TripBook::filterByDistance(_pBook, values.at(0), values.at(1));
        if (ptr)
        {
            cout << *ptr << endl;

            cout << "Press any key to continue!";
            string endstring;
            getline(cin, endstring); // workaround to wait and show output
        }
        else
        {
            strError = "Not found any trip in distance range";
        }
    }
};

class ReportAllTripsOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    void onSelected()
    {
        system("cls");
        cout << *_pBook;

        cout << "Press any key to continue!";
        string endstring;
        getline(cin, endstring); // workaround to wait and show output
    }
};

class MeansureFuelTripOption : public BaseTripOption
{
public:
    using BaseTripOption::BaseTripOption;

    void onSelected()
    {
        auto fuel = _pBook->avgFuel();
        strError = "Average fuel consumption based on the driven distance and bought fuel = " + to_string(fuel);
    }
};

class InfoMenu : public BaseTripMenu
{
public:
    using BaseTripMenu::BaseTripMenu;

    void dump()
    {
        cout << "Diary Record by HA DO " << endl;
    }
};

//==========================================================================
class SecondMenu : public BaseTripMenu
{
public:
    SecondMenu(IBaseMenu *parent, string name, TripBook::Ptr ptr) : BaseTripMenu(parent, name, ptr)
    {
        add(make_shared<InfoMenu>(this, "Show project information", ptr));
    }
};

class MainMenu : public BaseTripMenu
{
public:
    MainMenu(IBaseMenu *parent, string name, TripBook::Ptr ptr) : BaseTripMenu(parent, name, ptr)
    {
        add(make_shared<ExitOption>(this, "Exit", ptr));
        add(make_shared<InitializeOption>(this, "Initialize (clear all trip information)", ptr));
        add(make_shared<SaveFileOption>(this, "Save to file (" + output_file + ")", ptr));
        add(make_shared<ReadFileOption>(this, "Read from file (" + input_file + ")", ptr));
        add(make_shared<AddNewTripMenu>(this, "Add new trip", ptr));
        add(make_shared<RemoveLastTripOption>(this, "Remove last trip", ptr));
        add(make_shared<RemoveTripMenu>(this, "Remove trip by start name", ptr));
        add(make_shared<ReportTripMenu>(this, "Report a list of trips", ptr));
        add(make_shared<ReportAllTripsOption>(this, "Report all trips", ptr));
        add(make_shared<MeansureFuelTripOption>(this, "Calculate average fuel consumption", ptr));
        add(make_shared<SecondMenu>(this, "->NEXT", ptr));
    }
};

//=========================================================================================

int main()
{
    TripBook::Ptr pTripBook = make_shared<TripBook>(); // create a empty tripbook object
    IBaseMenu::Ptr pMenu = make_shared<MainMenu>(nullptr, "MainMenu", pTripBook);
    IBaseMenu::pCurrentMenu = pMenu.get(); // it's bad, but too complicated to use shared_from_this in inheritance

    isrunning = true;
    while (isrunning)
    {
        if( system("clear\n") < 0) {
            cout<<"can't clear screen"<<endl;
            break;
        }

        system("cls\n"); // workaround for window system terminal
        cout << endl;
        // trace last error message
        if (!IBaseMenu::strError.empty())
        {
            cout << "ERROR MSG: " << IBaseMenu::strError << endl;
            cout.width(IBaseMenu::strError.size() + 20);
            cout.fill('-');
            cout << '-' << endl;
            cout.fill(' ');
        }
        IBaseMenu::strError.clear();

        IBaseMenu::pCurrentMenu->dump();

        string inputString;
        getline(cin, inputString);
        if (inputString.empty())
        {
            IBaseMenu::pCurrentMenu->navigateBack();
            continue;
        }
        IBaseMenu::pCurrentMenu->process(inputString);
    };
}
