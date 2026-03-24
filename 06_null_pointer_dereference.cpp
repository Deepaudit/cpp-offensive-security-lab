#include <iostream>

void process_data(int* ptr) {
    // PERIGO: Nao verifica se o ponteiro e nulo antes de acessar
    std::cout << "Valor processado: " << *ptr << std::endl; 
}

int main() {
    int* data = nullptr;

    std::cout << "Tentando processar ponteiro nulo..." << std::endl;
    
    // Isso causara um Segmentation Fault (Crash)
    process_data(data); 

    return 0;
}
