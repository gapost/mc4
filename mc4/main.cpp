#include "mc4.h"
#include <iostream>
#include <sstream>
#include <strings.h>

using namespace std;

int main (void)
{
    mc4 m;
    int addr = 6;

    if (m.init(addr)<0) {
        cout << "Error initializing mc4. Error: " << m.error_msg() << endl;
        return -1;
    }

    int done=0;
    string cmd, resp;
    char axis = 'Z';
    while (true) {
        cout << "Enter command: ";

        string tok;
        cin >> tok;
        if (tok == "go") {
            int pos;
            cin >> pos;
            if (pos > -2000 && pos < 2000) {
                cout << "Setting pos to " << pos << endl;
                if (m.set_position(axis, pos) != 0)
                    cout << m.error_msg();
            } else {
                cout << "Invalid pos" << endl;
            }
        } else if (tok == "reset") {
            m.reset_position(axis);
        } else if (tok == "read") {
            int pos;
            if (m.get_position(axis, pos) == 0) {
                cout << "Position = " << pos << endl;
            } else {
                cout << m.error_msg() << endl;
            }
        } else if (tok == "q")
            break;
    }

    return m.close();
}
