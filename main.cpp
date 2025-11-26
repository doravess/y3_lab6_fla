#include <iostream>


int main() {
    int x = 10;
    int count = 0;

    if (x > 5) {
        std::cout << "Outside loop." << std::endl;
    }

    for (int i = 0; i < x; ++i) {
        if (i % 2 == 0) {
            count++;
        }
    }

    while (x > 0) {
        x--;
        if (x < 5) {
            break;
        }
    }

    return count;
}