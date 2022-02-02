#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <iomanip>
#include <cstring>

using namespace std;

class Splitter
{
public:
    static vector<string> exec(const string &str, const string &pattern);
    static void exec(const string &str, const string &pattern, vector<int32_t> &ret);
};

vector<string> Splitter::exec(const string &str, const string &pattern)
{
    vector<string> resultVec;
    char *tmpStr = strtok((char *)str.c_str(), pattern.c_str());

    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }

    return resultVec;
}

void Splitter::exec(const string &str, const string &pattern, vector<int32_t> &ret)
{
    vector<string> stringArray = exec(str, pattern);
    ret.clear();
    for (const auto &elem : stringArray)
    {
        ret.push_back(stoi(elem, 0, 0));
    }
}

class IBaseMenu
{

public:
    using Ptr = shared_ptr<IBaseMenu>;

    IBaseMenu(IBaseMenu *parent, string name)
        : _parent(parent), _name(name)
    {
    }
    virtual ~IBaseMenu() = default;

    void add(IBaseMenu::Ptr subMenu)
    {
        _menus.push_back(subMenu);
    }
    virtual void dump();
    virtual void process(const string &str);
    virtual void onSelected() {}
    virtual string getName() { return _name; }
    void navigateBack();

    static IBaseMenu *pCurrentMenu;
    static string strError;

protected:
    string _name;
    vector<IBaseMenu::Ptr> _menus;
    IBaseMenu *_parent = nullptr;
    bool isMenu = true;
};

IBaseMenu *IBaseMenu::pCurrentMenu = nullptr;
string IBaseMenu::strError;

void IBaseMenu::dump()
{
    cout << getName() << ": select and press enter" << endl;
    if (pCurrentMenu == this)
    {
        for (size_t i = 0; i < _menus.size(); i++)
            cout << setw(3) << i << ".  " << _menus.at(i)->getName() << endl;
        cout << endl;
        if (_parent)
            cout << "press enter to navigate back" << endl;
    }
    else
    {
        for (size_t i = 0; i < _menus.size(); i++)
            _menus[i]->dump();
    }
}

void IBaseMenu::navigateBack()
{
    if (_parent)
    {
        pCurrentMenu = _parent;
    }
}

void IBaseMenu::process(const string &str)
{
    if (pCurrentMenu == this)
    {
        int id = atoi(str.c_str());
        if (id >= 0 && id < (int)_menus.size())
        {
            if (_menus[id]->isMenu)
                pCurrentMenu = _menus[id].get();
            else
                _menus[id]->onSelected();
        }
        else
        {
            strError = "input error.";
        }
    }
}

struct BaseMenu : public IBaseMenu
{
public:
    using IBaseMenu::IBaseMenu;
    virtual ~BaseMenu() = default;
};

class BaseOption : public IBaseMenu
{
public:
    using IBaseMenu::IBaseMenu;

    BaseOption(IBaseMenu *parent, string name) : IBaseMenu(parent, name)
    {
        isMenu = false;
    }
    virtual ~BaseOption() = default;

    virtual void onSelected() {}

    bool isEnable() { return _enable; }
    void setEnable(bool enable) { _enable = enable; }

protected:
    bool _enable = true;
};
