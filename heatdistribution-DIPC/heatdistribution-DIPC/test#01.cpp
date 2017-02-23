//
//  test#01.cpp
//  heatdistribution-DIPC
//
//  Created by William Morris on 22/02/2017.
//
//

#include "test#01.hpp"
#include "gtest.framework/Headers/gtest.h"
#include "stack.h"

TEST (StackTest, PushAndPeek) {
    Stack intStack;
    int a = 12;
    int b = 15;
    EXPECT_EQ (12, intStack.push(a));
    EXPECT_EQ (15, intStack.push(b));
    EXPECT_EQ (15, intStack.peek());
    //make sure adding in LIFO Order
    EXPECT_EQ (15, intStack.peek());
    //Should still be there
}

TEST (StackTest, PushAndPop) {
    Stack intStack;
    int a = 12;
    int b = 15;
    EXPECT_EQ (12, intStack.push(a));
    EXPECT_EQ (15, intStack.push(b));
    EXPECT_EQ (15, intStack.pop());
    //make sure adding in LIFO Order
    EXPECT_EQ (12, intStack.pop());
    //Should have removed 15, then removed 12
    EXPECT_EQ (-1, intStack.pop());
    //Should return -1 because there is nothing on the stack
}



