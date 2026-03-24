#include <stdio.h>

int main(int argc, char** argv) {
    if (argc > 1) {
        // PERIGO: O input do usuário é usado como string de formato
        printf(argv[1]); 
    }
    return 0;
}
