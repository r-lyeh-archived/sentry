Sentry <a href="https://travis-ci.org/r-lyeh/sentry"><img src="https://api.travis-ci.org/r-lyeh/sentry.svg?branch=master" align="right" /></a>
======

- Sentry is a lightweight data monitor (C++11).
- Sentry is highly customizable.
- Sentry is tiny, and header-only.
- Sentry is self-contained. No external dependencies.
- Sentry is BOOST licensed.

## Sample
```c++
#include <iostream>
#include <thread>
#include <chrono>

#include "sentry.hpp"

// variables to monitor

namespace levels {
    double arrows = 15000, orcs = 3000;
}

// sentry monitoring sample

int main()
{
    // configuration
    sentry st;

    st.chiefs = {"sauron@mordor.com", "saruman@isengard.com"};
    st.guards = {"orc1@mordor.com", "orc2@isengard.com", "orc3@mordor.com"};
    st.others = {"automatic-subsystem@middleearth.com" };

    st.ongood = { []( sentry &st, const std::string &issue ) {
        for( auto &who : st.others )
            std::cout << '\t' << who << " says: " << issue << std::endl;
    }};
    st.onwarn = { []( sentry &st, const std::string &warning ) {
        for( auto &who : st.guards )
            std::cout << '\t' << who << " says: " << warning << std::endl;
    }};
    st.onfail = { []( sentry &st, const std::string &failure ) {
        for( auto &who : st.chiefs )
            std::cout << '\t' << who << " says: " << failure << std::endl;
        for( auto &who : st.guards )
            std::cout << '\t' << who << " says: " << failure << std::endl;
    }};

    // patrol every 2 seconds
    st.patrol( 2, []( sentry &st, const std::string &issue_type ) {
        std::cout << "watching orcs:" << levels::orcs << std::endl;
        std::cout << "watching arrows:" << levels::arrows << std::endl;

        /**/ if( levels::arrows > 2000 ) st.good("nominal");
        else if( levels::arrows >  200 ) st.warn("we are running out of arrows!");
        else                             st.fail("no weapons!!");

        /**/ if( levels::orcs  >= 3000 ) st.good("nominal");
        else if( levels::orcs  >=  100 ) st.warn("we are running out of orcs!");
        else                             st.fail("battle is lost!!");
    } );

    // queue as many patrols as you like
    // st.patrol( ... );

    // configuration is done at this point
    // sentry is working in background right now
    // you can modify all monitored data in real-time

    // simulate a cpu load and let sentry reacts
    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
    levels::arrows /= 10; levels::orcs /= 3;

    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
    levels::arrows /= 10; levels::orcs /= 20;

    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
    return 0;
}
```

## Possible output
```
watching orcs:3000
watching arrows:15000
	automatic-subsystem@middleearth.com says: nominal
	automatic-subsystem@middleearth.com says: nominal
watching orcs:3000
watching arrows:15000
	automatic-subsystem@middleearth.com says: nominal
	automatic-subsystem@middleearth.com says: nominal
watching orcs:1000
watching arrows:1500
	orc1@mordor.com says: we are running out of arrows!
	orc2@isengard.com says: we are running out of arrows!
	orc3@mordor.com says: we are running out of arrows!
	orc1@mordor.com says: we are running out of orcs!
	orc2@isengard.com says: we are running out of orcs!
	orc3@mordor.com says: we are running out of orcs!
watching orcs:50
watching arrows:150
	sauron@mordor.com says: no weapons!!
	saruman@isengard.com says: no weapons!!
	orc1@mordor.com says: no weapons!!
	orc2@isengard.com says: no weapons!!
	orc3@mordor.com says: no weapons!!
	sauron@mordor.com says: battle is lost!!
	saruman@isengard.com says: battle is lost!!
	orc1@mordor.com says: battle is lost!!
	orc2@isengard.com says: battle is lost!!
	orc3@mordor.com says: battle is lost!!
```

## Notes
- clang/g++ users: both `-std=c++11` and `-lpthread` may be required when compiling `sentry.cpp`
