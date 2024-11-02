#include <iostream>

#include "base/Queue.h"

int main() {
    TQueue<int> q;
    q.push(1);
    auto x = q.pop();
    std::cout << x << std::endl;
    return 0;
}