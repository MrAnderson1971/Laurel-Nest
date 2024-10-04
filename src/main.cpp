#define GL3W_IMPLEMENTATION
#include <gl3w.h>
#include <iostream>
#include "ecs.hpp";

int main() {
	std::cout << "Hello world" << std::endl;
	registry.list_all_components();
	return 0;
}
