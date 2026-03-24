#include <iostream>

void check_access(unsigned short items) {
    // Se items for 65535 e somarmos 1, ele vira 0 (Overflow)
    if (items + 1 < 10) {
        std::cout << "Acesso concedido! Admin Mode." << std::endl;
    }
}

int main() {
    check_access(65535);
    return 0;
}
