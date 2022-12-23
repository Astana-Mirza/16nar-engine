#include <iostream>

#ifdef __linux__
#    define EXAMPLE_SCRIPT
#elif _WIN32
#    define EXAMPLE_SCRIPT __declspec(dllexport)
#endif

extern "C" EXAMPLE_SCRIPT void example_setup()
{
	 std::cout << "Setting up example scene!" << std::endl;
}
