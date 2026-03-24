#include <iostream>
#include <cstring>

void vuln_function(char* str) {
    char buffer[16];
    // PERIGO: strcpy não verifica o tamanho do input
    strcpy(buffer, str); 
    std::cout << "Buffer content: " << buffer << std::endl;
}

int main(int argc, char** argv) {
    if (argc > 1) vuln_function(argv[1]);
    return 0;
}
