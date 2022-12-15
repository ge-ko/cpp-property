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
        set { _dec = value - 1; },
        get { return _dec - 1; }
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

    private: int* _iptr = nullptr;
    // mutable reference based on underlying pointer
    public: property<int&> ref
    {
        get
        {
            return *_iptr;
        },
        set
        {
            _iptr = value;
        }
    };

    private: string* _strptr = nullptr;
    // mutable reference based on underlying pointer
    public: property<string&> ref2
    {
        get
        {
            return *_strptr;
        },
        set
        {
            _strptr = value;
        }
    };
};

int main() {
    class_with_properties c;

    cout << "\ntest scalar\n";
    cout << "write: 42\n"; c.age = 42;
    cout << "read: " << c.age << endl;

    cout << "\ntest getter changing the value\n";
    cout << "write: 82\n"; c.age = 82;
    cout << "read: " << c.slow_age << endl;

    cout << "\ntest setter changing the value\n";
    cout << "write: -5\n"; c.age = -5;
    cout << "read: " << c.age << endl;

    cout << "\ntest both getter setter changing the value\n";
    cout << "write: 10\n"; c.dec = 10;
    c.dec = c.dec = c.dec; // do we want get/set side effects on each assignment? check c#
    cout << "read: " << c.dec << endl;

    cout << "\ntest read-only property\n";
    cout << "write: 3";
    try { c.pi = 3; } catch(std::bad_function_call) { cout << " -> bad_function_call\n"; }
    cout << "read: " << c.pi << endl;

    cout << "\ntest write-only property\n";
    cout << "write: 1\n"; c.black_hole = 1;
    cout << "read:";
    try { cout << c.black_hole; } catch(std::bad_function_call) { cout << " -> bad_function_call\n"; }
    
    cout << "\ntest string trim on write\n";
    string name = " Arthur Dent ";
    cout << "write: \"" << name << "\"\n"; c.name = name;
    cout << "read: \"" << c.name << "\"" << endl;

    cout << "\ntest container\n";
    cout << "write: { 1, 2, 3, 4 }\n"; c.numbers = vector<int>{ 1, 2, 3, 4 };
    cout << "read: {"; copy(c.numbers.begin(), c.numbers.end(), ostream_iterator<int>(cout, ", ")); cout << "}\n";

    cout << "\ntest transforming the container\n";
    std::transform(c.numbers.begin(), c.numbers.end(), c.numbers.begin(), [](auto i){ return i*i; });
    cout << "read: {"; copy(c.numbers.begin(), c.numbers.end(), ostream_iterator<int>(cout, ", ")); cout << "}\n";

    cout << "\ntest scalar mutable reference\n";
    int i = 5;
    cout << "write: " << &i << " -> " << i << "\n"; c.ref = i;
    cout << "read: " << c.ref << endl;

    i = 6;
    cout << "change: *" << &i << " = " << i << endl; 
    cout << "read: " << c.ref << endl;

    int i2 = 7;
    cout << "write: " << &i2 << " -> " << i2 << "\n"; c.ref = i2;
    cout << "read: " << c.ref << endl;

    cout << "\ntest class mutable reference\n";
    string s = "5";
    cout << "write: " << &s << " -> \"" << s << "\"\n"; c.ref2 = s;
    cout << "read: \"" << c.ref2 << "\"\n";

    s = "6"; // previosu string is destroyed
    cout << "change: *" << &s << " = \"" << s << "\"\n";
    cout << "read: \"" << c.ref2 << "\"\n"; // ref remains pointing to the destroyed string

    string s2 = "7";
    cout << "write: " << &s2 << " -> " << s2 << "\n"; c.ref2 = s2;
    cout << "read: \"" << c.ref2 << "\"\n";
}