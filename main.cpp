#include <iostream>
#include "inisettings.h"

using namespace std;

int main()
{
    { // Write settings

    IniSettings settings("config.ini");
    settings.beginGroup("HELLO");
    settings.setValue("val1", "hello");
    settings.setValue("val2", 123);
    settings.setValue("val3", 1.23);
    settings.endGroup();
    }

    { // Read settings

    IniSettings settings("config.ini");
    settings.beginGroup("HELLO");
    string val1 = settings.value("val1");
    int val2 = settings.value("val2", 0);
    double val3 = settings.value("val3", 0.);
    settings.endGroup();

    cout << "settings: " << endl
         << "val1: " << val1 << endl
         << "val2: " << val2 << endl
         << "val3: " << val3 << endl;
    }

    return 0;
}
