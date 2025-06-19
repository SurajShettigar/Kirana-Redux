#include <application.hpp>

int main(const int argc, char **argv)
{
    kirana::Application app{argc, argv};
    return app.run();
}