#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

#include "property.hpp"

using namespace std;

string trim(string str)
{
    const char * typeOfWhitespaces = " \t\n\r\f\v";
    str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
    str.erase(0,str.find_first_not_of(typeOfWhitespaces));
    return str;
}

struct class_with_properties
{
    private: int _age;
    public: property<int> age
    {
        get { return _age; },
        set { _age = value > 0 ? value : 0; }
    };

    public: property<int> slow_age
    {
        get { return _age / 4; }
    };

    private: int _dec;
    public: property<int> dec
    {
        get { return _dec - 1; },
        set { _dec = value - 1; }
    };

    public: property<float> pi
    {
        get { return 3.14; }
    };

    public: property<int> black_hole
    {
        set {}
    };

    private: string _name;
    public: property<string> name
    {
        get { return _name; },
        set { _name = trim(value); }
    };

    private: vector<int> _numbers;
    public: property<vector<int>> numbers
    {
        get { return _numbers; },
        set { _numbers = value; }
    };

    private: int* _i = nullptr;
    // mutable reference based on underlying pointer
    public: property<const int &> ref
    {
        get
        {
            cout << "from address " << _i << " which is pointing to " << *_i << endl;
            return *_i;
        },
        [this](auto value)
        {
            cout << "reference " << &value << " is referencing value " << value << endl;
            _i = &value;
            cout << " to address " << _i << " which is pointing now to " << *_i << endl;
        }
    };
};

int main() {
    class_with_properties c;

    cout << "\ntest scalar\n";
    c.age = 42;
    cout << 42 << " -> " << c.age << endl;

    cout << "\ntest getter changing the value\n";
    cout << 42 << " -> " << c.slow_age << endl;

    cout << "\ntest setter changing the value\n";
    c.age = -5;
    cout << -5 << " -> " << c.age << endl;

    cout << "\ntest both getter setter changing the value\n";
    c.dec = 10;
    c.dec = c.dec = c.dec; // do we want get/set side effects on each assignment? check c#
    cout << 10 << " -> " << c.dec << endl;

    cout << "\ntest read-only property\n";
    try { c.pi = 3; } catch(std::bad_function_call) { cout << "bad_function_call\n"; }
    cout << "pi -> " << c.pi << endl;

    cout << "\ntest write-only property\n";
    c.black_hole = 1;
    c.black_hole = 2;
    try { cout << c.black_hole; } catch(std::bad_function_call) { cout << "bad_function_call\n"; }
    
    cout << "\ntest string trim on set\n";
    string name = " Arthur Dent ";
    c.name = name;
    cout << "\"" << name << "\" -> \"" << c.name << "\"" << endl;

    cout << "\ntest container property\n";
    c.numbers = vector<int>{ 1, 2, 3, 4 };
    cout << "{ 1, 2, 3, 4 }  -> { ";
    copy(c.numbers.begin(), c.numbers.end(), ostream_iterator<int>(cout, ", "));
    cout << "}" << endl;

    std::transform(c.numbers.begin(), c.numbers.end(), c.numbers.begin(), [](auto i){ return i*i; });
    cout << "{ 1, 2, 3, 4 }  -> { ";
    copy(c.numbers.begin(), c.numbers.end(), ostream_iterator<int>(cout, ", "));
    cout << "}" << endl;

    cout << "\ntest reference property i.e. mutable reference\n";
    int i = 5;
    cout << "address " << &i << " contains value " << i << endl;
    int & ir = i;
    cout << "reference " << &ir << " is referencing value " << ir << endl;
    c.ref = ir;
    cout << "5 -> " << c.ref << endl;
    i = 6;
    cout << "6 -> " << c.ref << endl;

    int i2 = 7;
    ir = i2;
    c.ref = ir;
    cout << "7 -> " << c.ref << endl;
}