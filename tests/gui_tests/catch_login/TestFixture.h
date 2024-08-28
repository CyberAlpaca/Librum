#pragma once

#include <suri/automator.h>

class TestFixture {
public:
    Automator automator;
    TestFixture();
    ~TestFixture();
    void initTestCase();

private:
    inline static bool initialized = false;


};
