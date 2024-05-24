#include "MyQueue.hpp"
#include "Beverage.hpp"

int main(void)
{
    MyQueue<Beverage> a;

    for (int i = 0; i < 10; i++)
        a.push(Beverage("hello", i));
    for(int i = 0; i < 10; i++) {
        printf("pop 시도 %d\n", i + 1);
        a.pop();
    }
}