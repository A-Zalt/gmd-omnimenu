#include "hooks.cpp"

[[gnu::constructor]]
int main() {
    srand(time(0));
    initialize_hooks();
}
