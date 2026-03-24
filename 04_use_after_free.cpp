#include <iostream>
#include <string>

struct User {
    char name[20];
    bool is_admin;
};

int main() {
    User* user = new User;
    user->is_admin = false;
    
    std::cout << "Liberando memoria do usuario..." << std::endl;
    delete user; // Memoria liberada, mas o ponteiro 'user' ainda existe (Dangling Pointer)

    // PERIGO: O programa continua usando o ponteiro 'user'
    // Se um atacante conseguir alocar algo no lugar dessa memoria antes desta linha:
    if (user->is_admin) {
        std::cout << "Acesso concedido como ADMIN via Use-After-Free!" << std::endl;
    } else {
        std::cout << "Acesso negado." << std::endl;
    }

    return 0;
}
