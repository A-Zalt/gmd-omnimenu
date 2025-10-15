#include "hooks.cpp"

[[gnu::constructor]]
int main() {
    initialize_hooks();
}
