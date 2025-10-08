// ===========================================================================
// Simple Threading Demo (std::thread basics)
// ===========================================================================

#include <iostream>
#include <thread>
#include <vector>

void test_simple_threading_01();
void test_simple_threading_02();
void test_simple_threading_03();

//extern namespace SimpleThreading01;
//
extern void myFunction(int value);

// unresolved external symbol "void __cdecl function(int,int,int)"
// (? function@@YAXHHH@Z) referenced in function main


extern void function(int a, int b, int c);  // 3 int-Parametern: function@@YAXHHH@Z

extern std::thread copyT1;
extern std::thread copyT2;

extern std::vector<std::thread> myThreads;

int main()
{
    test_simple_threading_01();
    //test_simple_threading_02();
    //test_simple_threading_03();

     char ch;
     std::cin >> ch;

     //copyT1.join();  // oder detach ...
     //copyT2.join();

     // 1. Alternative
     //while (!myThreads.empty())
     //{
     //    std::thread t = std::move(myThreads.back());
     //    myThreads.pop_back();

     //    t.detach();
     //}

     // 2. Alternative: Ohne Move
     //while (!myThreads.empty())
     //{
     //    std::thread& t  = myThreads.back();

     //    t.detach();

     //    myThreads.pop_back();
     //}

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
