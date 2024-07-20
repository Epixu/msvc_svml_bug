// Just sets up the Catch2 runner, see TestPow.cpp instead
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* argv[]) {
   Catch::Session session;
   return session.run(argc, argv);
}
