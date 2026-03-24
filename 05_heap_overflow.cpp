#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    if (argc < 2) return 1;

    // Alocando dois buffers vizinhos no Heap
    char* buffer = new char[8];
    char* metadata = new char[8];

    strcpy(metadata, "NORMAL");

    // PERIGO: strcpy nao checa o limite de 8 bytes do 'buffer'
    // Se argv[1] tiver 16 bytes, ele sobrescrevera o conteúdo de 'metadata'
    strcpy(buffer, argv[1]);

    std::cout << "Buffer: " << buffer << std::endl;
    std::cout << "Metadata (sobrescrito?): " << metadata << std::endl;

    return 0;
}
