#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp>

int main(int argc, char** argv)
{

    //HERE: my setup


    int result = Catch::Session().run(argc, argv);

    //HERE: my cleanup

    return result;

}
