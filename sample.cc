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

    auto action = []( sentry &st, const std::string &unused ) {
        std::cout << "watching orcs:" << levels::orcs << std::endl;
        std::cout << "watching arrows:" << levels::arrows << std::endl;

        /**/ if( levels::arrows > 2000 ) st.good("nominal");
        else if( levels::arrows >  200 ) st.warn("we are running out of arrows!");
        else                             st.fail("no weapons!!");

        /**/ if( levels::orcs  >= 3000 ) st.good("nominal");
        else if( levels::orcs  >=  100 ) st.warn("we are running out of orcs!");
        else                             st.fail("battle is lost!!");
    };

    // patrol every 2 seconds
    st.patrol( action, 0, 2 );

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
