#include "test_framework.h"

int main() {
    return archlab::test::Registry::instance().run_all();
}
