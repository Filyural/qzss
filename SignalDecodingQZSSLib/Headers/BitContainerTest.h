#pragma once

class BitContainerTest
{
public:
    static bool test_ALL();

private:
    static bool test_add();
    static bool test_subContainer();
    static bool test_toLength();
    static bool test_getNum();
    static bool test_trimLeadingZeros();
    static bool test_equals();
    static bool test_operatorXOR();
    static bool test_operatorEQUAL();
};
