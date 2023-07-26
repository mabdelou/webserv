#include "socket/socketInit.hpp"
#include "shared/shared.hpp"

#include "parssing/parssing/Parssing.hpp"

int  main( int ac, char **av ) {

    try {

        parssing_part( ac, av, gServers );
        socketInit socketObg;
    } catch ( const std::exception &e ) {

        std::cout << "\e[31m[Socket]\e[0m "
                  << e.what() << &std::flush << std::endl;
        return EXIT_FAILURE;
    } catch ( std::string &e ) {

        std::cerr << "\e[31m[Parssing]\e[0m "
                  << e << &std::flush << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}