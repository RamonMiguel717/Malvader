#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LIM 50
//como eu to com muita preguiça, nao vou fazer agora, mas a ideia é
//fazer duas funções, uma pra conta corrente e outra pra conta poupança
//como ambas usam basicamente as mesmas informações eu decidi alocar todas as info de cadastro em uma só struct e quando for chamar as funções só especificar elas na struct CADASTRO
//lembrando que eu devo utilizar o fgets na maior parte dos casos, pois o scanf não lê espaço, então em situações de cadastro de nome daria problema.

void CP(){
}

void CC(){
}
typedef struct {
    int DIA;
    int MES;
    int ANO;
} DATA;
typedef struct {
    char LOGIN[50];
    char SENHA[50];
    char AGENCIA[LIM];
    int NUMERO_CONTA[LIM];
    char NOME[LIM];
    int CPF[LIM];
    DATA DATA_NASCIMENTO;
    int TELEFONE[LIM];
    char ENDERECO[LIM];
    int CEP[8];
    int LOCAL[LIM];
    int NUMERO_CASA[3];
    int BAIRRO[LIM];
    int CIDADE[LIM];
    int ESTADO[LIM];
    int LIMITE_CONTA[LIM];
	DATA DATA_VENCIMENTO;
    
} CADASTRO;


// Função para apagar uma conta com base no login
void apagarConta(CADASTRO** usuarios, int* n, const char* login) {
    int indice = -1;
    for (int i = 0; i < *n; i++) {
        if (strcmp((*usuarios)[i].LOGIN, login) == 0) {
            indice = i;
            break;
        }
    }
    if (indice != -1) {
        for (int i = indice; i < *n - 1; i++) {
            (*usuarios)[i] = (*usuarios)[i + 1];
        }
        *usuarios = (CADASTRO*)realloc(*usuarios, (*n - 1) * sizeof(CADASTRO));
        (*n)--;
        printf("Conta com login %s foi APAGADA.\n", login);
    } else {
        printf("Conta com login %s nao foi encontrada.\n", login);

    }
}

// Função para cadastrar um novo usuário
void cadastrar(CADASTRO** usuarios, int* n) {
    *usuarios = (CADASTRO*)realloc(*usuarios, (*n + 1) * sizeof(CADASTRO));
    if (*usuarios == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
printf ("   Digite a sua opcao\n(1)-Conta Poupanca-CP\n (2)-Conta Corrente-CC\n(3)-Voltar");
}

// Função para exibir a tela do funcionário e realizar operações
void telaFuncionario(int* opcao, CADASTRO** usuarios, int* n) {
    char Xlogin[50];
    printf("Escolha sua opcao:\n(1) Abertura de conta\n(2) Encerramento de conta\n(3) Consultar Dados\n(4) Alterar Dados\n(5) Cadastro de Funcionarios\n(6) Gerar relatorios\n(7) Sair\n");
    scanf("%d", opcao);
    switch (*opcao) {
        case 1:
            printf("_____Abertura de conta______\n");
            cadastrar(usuarios, n);
            break;
        case 2:
            printf("____Encerramento de Conta_____\n");
            printf("Digite o Login da conta que deseja apagar: ");
            scanf("%s", Xlogin);
            apagarConta(usuarios, n, Xlogin);
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
}

// Função para verificar a senha do usuário
int verificarSenha(const CADASTRO* usuario, const char* senha) {
    return strcmp(usuario->SENHA, senha) == 0;
}

// Função para realizar o login do usuário
int fazerLogin(const CADASTRO* usuarios, int n) {
    char login[50], senha[50];
    printf("Digite o seu login: ");
    scanf("%s", login);
    printf("Digite a sua senha: ");
    scanf("%s", senha);
    for (int i = 0; i < n; i++) {
        if (strcmp(usuarios[i].LOGIN, login) == 0 && verificarSenha(&usuarios[i], senha)) {
            printf("Login bem-sucedido!\n\n");
            return 1;
        }
    }
    printf("Login ou senha incorretos!\n\n");
    return 0;
}

// Função para gerenciar o cadastro de usuários
void gerenciarCadastro(CADASTRO** usuarios, int* n, const char* tipo) {
    int escolha;
    printf("Ja tem cadastro de %s na plataforma?\n(1) SIM\n(2) NAO\n", tipo);
    scanf("%d", &escolha);
    switch (escolha) {
        case 1:
            fazerLogin(*usuarios, *n);
            break;
        case 2:
            cadastrar(usuarios, n);
            break;
        default:
            printf("Opcao invalida!\n\n");
            break;
    }
}

// Função principal
int main() {
    CADASTRO* funcionarios = NULL;
    CADASTRO* clientes = NULL;
    int nFuncionarios = 0, nClientes = 0;
    int opcao; 
    while (1) {
        printf("Escolha uma opcao:\n(1) Funcionario\n(2) Cliente\n(3) Sair\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                telaFuncionario(&opcao, &funcionarios, &nFuncionarios);
                break;
            case 2:
            //isso aqui é a parte da gaby, então por enquanto vai ficar a tela de gerenciar cadastro mesmo
    
                gerenciarCadastro(&clientes, &nClientes, "cliente");
                break;
            case 3:
                printf("Saindo do programa...\n");
                free(funcionarios);
                free(clientes);
                return 0;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
    return 0;
}

