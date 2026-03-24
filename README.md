# 🛡️ C++ Offensive Security Lab: Memory & Logic Corruptions

Este repositório é um guia prático para Pentesters e Pesquisadores de Segurança que desejam entender como falhas em nível de memória funcionam no C++. 

> **Aviso:** Estes códigos são estritamente para fins educacionais. Compilar e executar sem proteção (como desativar ASLR/Stack Canaries) é recomendado apenas em ambientes controlados.

---

## 🏗️ Estrutura das Vulnerabilidades

Abaixo, os códigos-fonte minimalistas que exemplificam falhas críticas de segurança.

### 1. Buffer Overflow (Stack-based)
Ocorre quando dados excedem o tamanho do buffer alocado, sobrescrevendo o endereço de retorno na pilha.
* **Arquivo:** `01_stack_overflow.cpp`
* **Risco:** Execução Remota de Código (RCE).



### 2. Format String Vulnerability
Acontece quando o input do usuário é passado diretamente para funções de formatação como `printf`.
* **Arquivo:** `02_format_string.cpp`
* **Risco:** Vazamento de memória (Info Disclosure) ou escrita em endereços arbitrários.

### 3. Integer Overflow
Quando uma operação aritmética excede o valor máximo de um tipo inteiro, causando comportamentos inesperados em verificações de segurança.
* **Arquivo:** `03_integer_overflow.cpp`

### 4. Use-After-Free (UAF)
Tentar acessar um ponteiro após a memória ter sido liberada, o que pode levar ao sequestro do fluxo do programa.
* **Arquivo:** `04_use_after_free.cpp`

---

## 🛠️ Códigos de Exemplo (PoCs)

#### `01_stack_overflow.cpp`
```cpp
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
02_format_string.cpp
C++
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc > 1) {
        // PERIGO: O input do usuário é usado como string de formato
        printf(argv[1]); 
    }
    return 0;
}
03_integer_overflow.cpp
C++
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
🔍 Guia de Compilação para Testes
Para estudar essas falhas sem as proteções modernas do compilador (GCC), use:

Bash
g++ -fno-stack-protector -z execstack -no-pie 01_stack_overflow.cpp -o buffer_lab
Desenvolvido por Deep Audit - Pesquisa em Segurança Ofensiva.
