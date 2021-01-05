//-< TESTMAP.CXX >---------------------------------------------------*--------*
// POST++                     Version 1.0        (c) 1998  GARRET    *     ?  *
// (Persistent Object Storage)                                       *   /\|  *
//                                                                   *  /  \  *
//                          Created:     31-May-99    K.A. Knizhnik  * / [] \ *
//                          Last update: 31-May-99    K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Example of using STL map class with POST++
// This example explicitly specify allocator for STL types
//-------------------------------------------------------------------*--------*

#include "post_stl.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;
USE_POST_NAMESPACE

typedef basic_string<char, char_traits<char>, post_alloc<char> > post_string;
typedef map<post_string, post_string, less<post_string>, post_alloc<pair<post_string, post_string> > > barmap;

int main()
{
    storage sto("bar.odb");
    if (sto.open(storage::fixed)) { 
        barmap* root  = (barmap*)sto.get_root_object();
        //
        // Microsoft implementation of STL map class uses _Tree template
        // which contains static _Nil component. This component is initialized 
        // by _Init() function when constructor of this class is called first 
        // time. To preserve the same address for this _Nil objects, 
        // programmer should create instances of all used map classes 
        // within begin_static_data, end_static_data section. In this case
        // post_alloc will return the same addresses for _Nil objects.
        //
        sto.begin_static_data();
        barmap* dummy = new (sto) barmap; 
        sto.end_static_data();
        if (root != NULL) { 
            for (barmap::iterator i = root->begin(); i != root->end(); i++) {
                cout << "Key=" << (*i).first 
                     << ", value=" << (*i).second << '\n';
            }
        } else { 
            root = new (sto) barmap;
            sto.set_root_object((object*)root);
        }
        cout << "Add some pairs. Type '.' to exit. Type '*' to clear the content.\n";
        while (true) { 
            post_string key;
            post_string value;
            cout << "Key: ";
            cin >> key;
            if (key == ".") { 
                break;
            }

            //
            //
            //
            if (key == "*") {

              root->clear ();
              continue;
            }
            //
            //
            //

            cout << "Value: ";
            cin >> value;
            cout << "Key = " << key << ", Value = " << value << '\n';
            root->insert(barmap::value_type(key, value));
        }
        sto.flush();
        sto.close();
    }
    return 0;
}

