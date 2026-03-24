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
*![Format String](https://owasp.org/www-community/attacks/Format_string_attack_diagram.png)
* ![Memory Layout](https://upload.wikimedia.org/wikipedia/commons/thumb/5/50/Program_memory_layout.pdf/page1-1200px-Program_memory_layout.pdf.jpg)
*![Use After Free](https://cwe.mitre.org/data/images/cwe_416_uaf.png)
*![Format String](https://owasp.org/www-community/attacks/Format_string_attack_diagram.png)
*![Heap Overflow](https://blog.qualys.com/wp-content/uploads/2021/01/heap-overflow.png)
*![Heap Overflow](https://blog.qualys.com/wp-content/uploads/2021/01/heap-overflow.png)
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

#include <stdio.h>

int main(int argc, char** argv) {
    if (argc > 1) {
        // PERIGO: O input do usuário é usado como string de formato
        printf(argv[1]); 
    }
    return 0;
}

03_integer_overflow.cpp

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

----------------------------------------------------------------------------------
1. Stack-Based Buffer Overflow
O que é: Ocorre quando um programa escreve mais dados em um buffer (array) na pilha (stack) do que ele pode suportar.
A Causa Raiz: O uso de funções inseguras da biblioteca C, como strcpy, gets ou scanf, que não verificam o tamanho do destino.
O Impacto: O atacante sobrescreve o Endereço de Retorno (EIP/RIP) da função. Quando a função termina, em vez de voltar para o fluxo normal, o processador pula para um endereço controlado pelo atacante (onde pode haver um shellcode).
No seu código: O buffer[16] recebe qualquer tamanho de string via argv[1], permitindo o transbordamento.


2. Format String Vulnerability
O que é: Uma falha que ocorre quando o input do usuário é passado como o primeiro argumento de funções da família printf(), sprintf() ou fprintf().
A Causa Raiz: O programador confia que o usuário enviará apenas texto, mas o atacante envia "especificadores de formato" como %x (para ler a pilha) ou %n (para escrever na memória).
O Impacto: Vazamento de dados sensíveis da memória (como chaves de criptografia ou endereços de funções) e, em casos avançados, execução de código.
No seu código: printf(argv[1]) permite que eu digite %p %p %p %p para ver os endereços reais da sua memória.


3. Integer Overflow
O que é: Ocorre quando uma operação aritmética resulta em um valor que excede o limite máximo (ou mínimo) do tipo da variável.
A Causa Raiz: Tipos de dados têm tamanhos fixos. Por exemplo, um unsigned short só vai até 65.535. Se você somar 1 a ele, ele "reseta" para 0.
O Impacto: Bypass de verificações de segurança. Se um sistema checa se você tem dinheiro suficiente somando taxas, um overflow pode fazer o valor final parecer muito pequeno ou zero, enganando a lógica.
No seu código: items + 1 < 10 torna-se verdadeiro se items for 65535, pois o resultado matemático é 0.


4. Use-After-Free (UAF)
O que é: Um tipo de corrupção de memória onde uma aplicação continua a usar um ponteiro após a memória ter sido liberada com free ou delete.
A Causa Raiz: Falta de gerenciamento de ponteiros "pendentes" (dangling pointers). A memória é liberada, mas o endereço continua gravado na variável.
O Impacto: Se um atacante conseguir alocar um novo objeto no mesmo endereço de memória que foi liberado, ele pode controlar os valores que a função original vai ler, levando ao sequestro do fluxo de execução.
No seu código: O objeto user é deletado, mas a verificação if (user->is_admin) continua acessando aquele espaço de memória que agora pode pertencer a outro processo.


5. Heap Overflow
O que é: Similar ao Stack Overflow, mas ocorre no Heap, a área de memória usada para alocações dinâmicas (via new ou malloc).
A Causa Raiz: Manipulação incorreta de memória dinâmica, geralmente ao copiar dados para buffers alocados em tempo de execução.
O Impacto: Sobrescrita de metadados do Heap (que o sistema usa para gerenciar a memória) ou de objetos vizinhos. É mais complexo de explorar que o Stack Overflow, mas é a base de ataques contra navegadores e kernels.
No seu código: O strcpy(buffer, argv[1]) invade o espaço do ponteiro metadata, que foi alocado logo em seguida no Heap.


6. Null Pointer Dereference
O que é: Ocorre quando o programa tenta acessar um endereço de memória através de um ponteiro que é NULL (ou zero).
A Causa Raiz: Falta de validação de retornos de funções ou falta de inicialização. Se uma alocação de memória falha e o programa não checa, ele recebe um ponteiro nulo.
O Impacto: Geralmente causa o fechamento imediato da aplicação (Crash), resultando em Negação de Serviço (DoS). Em contextos de Kernel, pode ser usado para escalação de privilégios.
No seu código: A função process_data tenta ler o valor de *ptr quando ele vale nullptr, o que faz o Sistema Operacional matar o processo para proteger a memória.


🛡️ Resumo de Proteções Modernas
Para o seu conhecimento (e para explicar aos alunos), os compiladores modernos usam:

ASLR: Randomiza os endereços de memória.

Stack Canaries: Pequenos valores inseridos antes do endereço de retorno para detectar transbordamentos.

DEP/NX: Marca a pilha como "Não Executável", impedindo que shellcodes rodem nela.
