#include <iostream>

#include "Queue.h"

int main() {
    Queue<int> q;
    q.push(1);
    auto x = q.pop();
    std::cout << x << std::endl;
    return 0;
}