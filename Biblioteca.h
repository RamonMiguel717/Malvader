#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

#define LIM 50
//STRUCT P/ CADASTRO
typedef struct {
char LOGIN[LIM];
char SENHA[LIM];
char AGENCIA[LIM];
int NUMERO_CONTA;
int SALDO;
char NOME[LIM];
char CPF[12];
struct {
    int DIA;
    int MES;
    int ANO;
} DATA_NASCIMENTO;
char TELEFONE[10];
char ENDERECO[LIM];
char CEP[9];
char LOCAL[LIM];
char NUMERO_CASA[4];
char BAIRRO[LIM];
char CIDADE[LIM];
char ESTADO[LIM];
int LIMITE_CONTA;
struct {
    int DIA;
    int MES;
    int ANO;
} DATA_VENCIMENTO;
char TIPO[2];
char CODIGO_FUNCIONARIO[LIM];
char CARGO[LIM];
} CADASTRO;

//STRUCT P/ ADM
struct ADMINISTRADOR{
char senha_ADM[5];
char login_ADM[6];
};
//FUNÇÕES DO CODIGO

//ARQUIVO
ADMINISTRADOR admin = {"0000", "RAMON"};

void escreverNoArquivo_Cliente(const char* nomeArquivo, CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, int* j);
void verificarSenhaAdmin(const ADMINISTRADOR *admin, int *senhaCorreta);
void cadastro_funcionarios(CADASTRO **funcionarios, int *n, const char* nomeArquivo, CADASTRO* clientes, int* j);
void CP(CADASTRO* clientes, int* n, const char* nomeArquivo, CADASTRO** funcionarios, int* j);
void CC(CADASTRO* clientes, int* n, const char* nomeArquivo, CADASTRO** funcionarios, int* j);
void telaFuncionario(ADMINISTRADOR* admin, CADASTRO** funcionarios,const char* nomeArquivo, CADASTRO* clientes, int* n, int* j);
void consultarFuncionario(CADASTRO *funcionarios, int nFuncionarios, const char *nomeFuncionario);
void consultarConta(const char *senha_ADM, CADASTRO *clientes, int n, const char *nomeCliente);
void apagarConta(const char *senha_ADM, CADASTRO **clientes, int *n, const char *nomeCliente);
void cadastrar(CADASTRO** funcionarios, CADASTRO* clientes, const char* nomeArquivo, int* n, int* j);
int consultLimite(CADASTRO* clientes, int n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j);
void encerrarConta(ADMINISTRADOR *admin, CADASTRO *clientes, int *n);
void alterarDados(CADASTRO* clientes, int n, CADASTRO** funcionarios, const char* nomeArquivo);
void gerarRelatorios(CADASTRO* clientes,CADASTRO* funcionarios, int n, const char* nomeArquivo, ADMINISTRADOR* admin);
int encontrarConta(CADASTRO *clientes, int nClientes, const char *nomeMASK, int numeroMASK);
void verificarSenhaAdmin(const ADMINISTRADOR *admin, int *senhaCorreta);
void saldo(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j);
void deposito(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j);
void saque(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j);
int* extrato(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j);
int menuVolta(ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, CADASTRO* clientes, int* n, int* j);

//FUNÇAO PARA VOLTAR PARA O MENU ANTERIOR (ELE FUNCIONA APOS O FIM DE CADA FUNÇÃO)
int menuVolta(ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, CADASTRO* clientes, int* n, int* j) {
    int opcao1;

    while (1) {
        printf("\nO que deseja agora: \n");
        printf("1. Voltar para o menu anterior\n");
        printf("2. Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao1);

        switch (opcao1) {
            case 1:
                system("cls");
                return 0;
                break;

            case 2:
                telaFuncionario(admin, funcionarios, nomeArquivo, clientes, n, j);
                break;

            default:
                printf("Opcao invalida. Digite o numero de alguma opcao presente no menu:\n");
                break;
        }
    }
}

//FUNÇÃO P/ O ENCERRAMENTO DA CONTA DO CLIENTE
void encerrarConta(ADMINISTRADOR* admin, CADASTRO** funcionarios, CADASTRO* clientes, const char* nomeArquivo, int* n) {
    system("cls");
    int senhaCorreta;
	verificarSenhaAdmin(admin, &senhaCorreta);

    if (!senhaCorreta) {
        printf("Acesso negado. Encerrando a operacao.\n");
        return;
    }

    char nomeCliente[LIM];
    printf("Digite o nome do cliente para encerrar a conta: ");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    nomeCliente[strcspn(nomeCliente, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < *n; i++) {
        if (strcmp(clientes[i].NOME, nomeCliente) == 0) {
            for (int j = i; j < *n - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            (*n)--;
            printf("Conta do cliente %s encerrada com sucesso.\n", nomeCliente);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Cliente nao encontrado.\n");
    }

    int j = 0;
    menuVolta(admin, funcionarios, nomeArquivo, clientes, n, &j);
}


//ARQUIVO
//FUNÇÃO P/ ALTERAR OS DADOS DO CLIENTE
void alterarDados(CADASTRO* clientes, int n, CADASTRO** funcionarios, const char* nomeArquivo) {
    int j;
    system("cls");
    char nomeCliente[LIM];
    printf("Digite o nome do cliente para realizar a alteracao de dados: ");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    nomeCliente[strcspn(nomeCliente, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(clientes[i].NOME, nomeCliente) == 0) {
            printf("Cliente encontrado. Insira os novos dados.\n");
            printf("Insira o seu nome: ");
            fgets(clientes[i].NOME, sizeof(clientes[i].NOME), stdin);
            clientes[i].NOME[strcspn(clientes[i].NOME, "\n")] = 0;

            printf("Insira o seu CPF: ");
            fgets(clientes[i].CPF, sizeof(clientes[i].CPF), stdin);
            clientes[i].CPF[strcspn(clientes[i].CPF, "\n")] = 0;

            printf("Insira a data de nascimento (DD/MM/AAAA): ");
            scanf("%d/%d/%d", &clientes[i].DATA_NASCIMENTO.DIA, &clientes[i].DATA_NASCIMENTO.MES, &clientes[i].DATA_NASCIMENTO.ANO);
            while (getchar() != '\n');

            printf("Insira um telefone de contato: ");
            fgets(clientes[i].TELEFONE, sizeof(clientes[i].TELEFONE), stdin);
            clientes[i].TELEFONE[strcspn(clientes[i].TELEFONE, "\n")] = 0;

            printf("Insira o endereco: ");
            fgets(clientes[i].ENDERECO, sizeof(clientes[i].ENDERECO), stdin);
            clientes[i].ENDERECO[strcspn(clientes[i].ENDERECO, "\n")] = 0;

            encontrado = 1;
            printf("Dados do cliente %s alterados com sucesso.\n", nomeCliente);
            break;
        }
    }
    if (!encontrado) {
        printf("Cliente nao encontrado.\n");
    } else {
        // Gravar todas as informações no arquivo, incluindo as modificações
        FILE *arquivo = fopen(nomeArquivo, "w"); // Abrir no modo de escrita para sobrescrever o arquivo
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return;
        }

        for (int i = 0; i < n; i++) {
            fprintf(arquivo, 
                    "Nome: %s\nCPF: %s\nData de Nascimento: %02d/%02d/%04d\nTelefone: %s\nEndereco: %s\nCEP: %s\nLocal: %s\nNumero da casa: %s\nBairro: %s\nCidade: %s\nEstado: %s\nAgencia: %s\nNumero da Conta: %d\nSaldo: %d\nLimite da Conta: %d\nData de Vencimento: %02d/%02d/%04d\nTipo de Conta: %s\n",
                    clientes[i].NOME,
                    clientes[i].CPF,
                    clientes[i].DATA_NASCIMENTO.DIA, clientes[i].DATA_NASCIMENTO.MES, clientes[i].DATA_NASCIMENTO.ANO,
                    clientes[i].TELEFONE,
                    clientes[i].ENDERECO,
                    clientes[i].CEP,
                    clientes[i].LOCAL,
                    clientes[i].NUMERO_CASA,
                    clientes[i].BAIRRO,
                    clientes[i].CIDADE,
                    clientes[i].ESTADO,
                    clientes[i].AGENCIA,
                    clientes[i].NUMERO_CONTA,
                    clientes[i].SALDO,
                    clientes[i].LIMITE_CONTA,
                    clientes[i].DATA_VENCIMENTO.DIA, clientes[i].DATA_VENCIMENTO.MES, clientes[i].DATA_VENCIMENTO.ANO,
                    clientes[i].TIPO);
        }

        fclose(arquivo);
    }
menuVolta(&admin, funcionarios, nomeArquivo, clientes, &n, &j);

}

//ARQUIVO
//FUNÇÃO P/ GERAR O RELATÓRIO DO CLIENTE
void gerarRelatorios(CADASTRO* clientes,CADASTRO* funcionarios, int n, const char* nomeArquivo, ADMINISTRADOR* admin) {
    system("cls");
 
    printf("Relatorio de Clientes:\n\n");
    for (int i = 0; i < n; i++) {
        printf("Nome: %s\n", clientes[i].NOME);
        printf("CPF: %s\n", clientes[i].CPF);
        printf("Data de Nascimento: %02d/%02d/%04d\n", clientes[i].DATA_NASCIMENTO.DIA, clientes[i].DATA_NASCIMENTO.MES, clientes[i].DATA_NASCIMENTO.ANO);
        printf("Telefone: %s\n", clientes[i].TELEFONE);
        printf("Endereco: %s\n", clientes[i].ENDERECO);
        printf("CEP: %s\n", clientes[i].CEP);
        printf("Local: %s\n", clientes[i].LOCAL);
        printf("Numero da casa: %s\n", clientes[i].NUMERO_CASA);
        printf("Bairro: %s\n", clientes[i].BAIRRO);
        printf("Cidade: %s\n", clientes[i].CIDADE);
        printf("Estado: %s\n", clientes[i].ESTADO);
        printf("Agencia: %s\n", clientes[i].AGENCIA);
        printf("Numero da Conta: %d\n", clientes[i].NUMERO_CONTA);
        printf("Saldo: %d\n", clientes[i].SALDO);
        printf("Limite da Conta: %d\n", clientes[i].LIMITE_CONTA);
        printf("Data de Vencimento: %02d/%02d/%04d\n", clientes[i].DATA_VENCIMENTO.DIA, clientes[i].DATA_VENCIMENTO.MES, clientes[i].DATA_VENCIMENTO.ANO);
        printf("Tipo de Conta: %s\n", clientes[i].TIPO);
        printf("\n");
    }
    printf("Fim do relatorio.\n");
}
//ARQUIVO
//FUNNÇÃO P/ ENCONTRAR CONTA
int encontrarConta(CADASTRO* clientes, int nClientes, const char* nomeMASK, int numeroMASK){
    for (int i = 0; i < nClientes; i++) {
        if (strcmp(clientes[i].NOME, nomeMASK) == 0 && clientes[i].NUMERO_CONTA == numeroMASK) {
            return i;
        }
    }
    return -1;
}

//ARQUIVO
//FUNÇÃO P/ VERIFICAR SENHA DO ADM
void verificarSenhaAdmin(const ADMINISTRADOR* admin, int* senhaCorreta){
    char senhaMASK[5];
    int tentativa = 0;
    const int maxTentativas = 3;

    *senhaCorreta = 0;

    while (tentativa < maxTentativas){
        system ("cls");
        printf("Digite a senha de administrador: ");
        scanf("%4s", senhaMASK);
        while (getchar() != '\n');

        if (strcmp(admin->senha_ADM, senhaMASK) == 0) {
            printf("Senha correta! Executando funcao...\n");
            Sleep(1000);
            *senhaCorreta = 1;
            return;
        } else {
            tentativa++;
            if (tentativa < maxTentativas) {
                printf("Senha incorreta! Tente novamente. Tentativa %d de %d.\n", tentativa, maxTentativas);
            } else {
                printf("Senha incorreta! Numero maximo de tentativas atingido.\n");
                return;
            }
        }
    }
}

//ARQUIVO
//FUNÇÃO P/ ESCREVER NO ARQUIVO (FUNCIONARIO)
void escreverNoArquivo_Funcionario(const char* nomeArquivo, CADASTRO* funcionario){
    FILE *arquivo = fopen(nomeArquivo, "a");  // Abrir no modo de anexar
    if (arquivo == NULL){
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "Codigo do Funcionario: %s\n", funcionario->CODIGO_FUNCIONARIO);
    fprintf(arquivo, "Cargo: %s\n", funcionario->CARGO);
    fprintf(arquivo, "Nome: %s\n", funcionario->NOME);
    fprintf(arquivo, "CPF: %s\n", funcionario->CPF);
    fprintf(arquivo, "Data de Nascimento: %02d/%02d/%04d\n", funcionario->DATA_NASCIMENTO.DIA, funcionario->DATA_NASCIMENTO.MES, funcionario->DATA_NASCIMENTO.ANO);
    fprintf(arquivo, "Telefone: %s\n", funcionario->TELEFONE);
    fprintf(arquivo, "Endereco: %s\n", funcionario->ENDERECO);
    fprintf(arquivo, "CEP: %s\n", funcionario->CEP);
    fprintf(arquivo, "Local: %s\n", funcionario->LOCAL);
    fprintf(arquivo, "Numero da Casa: %s\n", funcionario->NUMERO_CASA);
    fprintf(arquivo, "Bairro: %s\n", funcionario->BAIRRO);
    fprintf(arquivo, "Cidade: %s\n", funcionario->CIDADE);
    fprintf(arquivo, "Estado: %s\n", funcionario->ESTADO);
    fprintf(arquivo, "Senha: %s\n", funcionario->SENHA);
    fprintf(arquivo, "\n");

    fclose(arquivo);
}

//FUNÇÃO P/ O CADASTRO DE FUNCIONARIOS
void cadastro_funcionarios(CADASTRO** funcionarios, int* n, const char* nomeArquivo, CADASTRO* clientes, int* j) {

    int senhaCorreta;
    verificarSenhaAdmin(&admin, &senhaCorreta);

    if (!senhaCorreta) {
        printf("Acesso negado. Encerrando o cadastro de funcionario.\n");
        return;
    }

    void *temp = realloc(*funcionarios, (*n + 1) * sizeof(CADASTRO));
    if (temp == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    *funcionarios = (CADASTRO *)temp;

    CADASTRO *novoFuncionario = &(*funcionarios)[*n];

    system("cls");
    printf("Digite o codigo de funcionario: ");
    fgets(novoFuncionario->CODIGO_FUNCIONARIO, sizeof(novoFuncionario->CODIGO_FUNCIONARIO), stdin);
    novoFuncionario->CODIGO_FUNCIONARIO[strcspn(novoFuncionario->CODIGO_FUNCIONARIO, "\n")] = 0;
    getchar();

    printf("Insira o cargo: ");
    fgets(novoFuncionario->CARGO, sizeof(novoFuncionario->CARGO), stdin);
    novoFuncionario->CARGO[strcspn(novoFuncionario->CARGO, "\n")] = 0;
    getchar();

    printf("Insira o nome do funcionario: ");
    fgets(novoFuncionario->NOME, sizeof(novoFuncionario->NOME), stdin);
    novoFuncionario->NOME[strcspn(novoFuncionario->NOME, "\n")] = 0;
    getchar();
	
    printf("Insira o CPF do funcionario: ");
    fgets(novoFuncionario->CPF, sizeof(novoFuncionario->CPF), stdin);
    novoFuncionario->CPF[strcspn(novoFuncionario->CPF, "\n")] = 0;
    getchar();
	
    printf("Insira a data de nascimento do funcionario (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &novoFuncionario->DATA_NASCIMENTO.DIA, &novoFuncionario->DATA_NASCIMENTO.MES, &novoFuncionario->DATA_NASCIMENTO.ANO);
    getchar();

    printf("Insira um telefone de contato: ");
    fgets(novoFuncionario->TELEFONE, sizeof(novoFuncionario->TELEFONE), stdin);
    novoFuncionario->TELEFONE[strcspn(novoFuncionario->TELEFONE, "\n")] = 0;
    getchar();
    
    printf("Insira o endereco do funcionario: ");
    fgets(novoFuncionario->ENDERECO, sizeof(novoFuncionario->ENDERECO), stdin);
    novoFuncionario->ENDERECO[strcspn(novoFuncionario->ENDERECO, "\n")] = 0;
    getchar();
    
    printf("Insira o CEP do funcionario: ");
    fgets(novoFuncionario->CEP, sizeof(novoFuncionario->CEP), stdin);
    novoFuncionario->CEP[strcspn(novoFuncionario->CEP, "\n")] = 0;
    getchar();
    
    printf("Insira o Local do funcionario: ");
    fgets(novoFuncionario->LOCAL, sizeof(novoFuncionario->LOCAL), stdin);
    novoFuncionario->LOCAL[strcspn(novoFuncionario->LOCAL, "\n")] = 0;
    getchar();
    
    printf("Insira o Numero da casa: ");
    fgets(novoFuncionario->NUMERO_CASA, sizeof(novoFuncionario->NUMERO_CASA), stdin);
    novoFuncionario->NUMERO_CASA[strcspn(novoFuncionario->NUMERO_CASA, "\n")] = 0;
    getchar();
    
    printf("Insira o Bairro de moradia do funcionario: ");
    fgets(novoFuncionario->BAIRRO, sizeof(novoFuncionario->BAIRRO), stdin);
    novoFuncionario->BAIRRO[strcspn(novoFuncionario->BAIRRO, "\n")] = 0;
    getchar();
    
    printf("Insira a cidade do funcionario: ");
    fgets(novoFuncionario->CIDADE, sizeof(novoFuncionario->CIDADE), stdin);
    novoFuncionario->CIDADE[strcspn(novoFuncionario->CIDADE, "\n")] = 0;
    getchar();
    
    printf("Insira o estado de moradia do funcionario: ");
    fgets(novoFuncionario->ESTADO, sizeof(novoFuncionario->ESTADO), stdin);
    novoFuncionario->ESTADO[strcspn(novoFuncionario->ESTADO, "\n")] = 0;
    getchar();
    
    printf("Insira a senha do funcionario: ");
    fgets(novoFuncionario->SENHA, sizeof(novoFuncionario->SENHA), stdin);
    novoFuncionario->SENHA[strcspn(novoFuncionario->SENHA, "\n")] = 0;
    getchar();
    
    (*n)++;
    printf("Funcionario cadastrado com sucesso.\n");

    escreverNoArquivo_Funcionario("dadosFuncionario.txt", novoFuncionario);
    menuVolta(&admin, funcionarios, nomeArquivo, clientes, n, j);
}
//ARQUIVO
//FUNÇÃO P/ O CADASTRO DE CONTA POUPANCA
void CP(CADASTRO* clientes, int* n, const char* nomeArquivo, CADASTRO** funcionarios, int* j) {
    system("cls");
    printf("Para realizar o cadastro da sua Conta Poupanca precisamos de algumas informacoes.\n\n");

    printf("Insira a agencia: ");
    fgets(clientes[*n].AGENCIA, sizeof(clientes[*n].AGENCIA), stdin);
    clientes[*n].AGENCIA[strcspn(clientes[*n].AGENCIA, "\n")] = 0;
        getchar();


    printf("Insira o numero da conta: ");
    scanf("%d", &clientes[*n].NUMERO_CONTA);
        getchar();

    printf("Insira o seu nome: ");
    fgets(clientes[*n].NOME, sizeof(clientes[*n].NOME), stdin);
    clientes[*n].NOME[strcspn(clientes[*n].NOME, "\n")] = 0;
    getchar();

    printf("Insira o seu CPF: ");
    fgets(clientes[*n].CPF, sizeof(clientes[*n].CPF), stdin);
    clientes[*n].CPF[strcspn(clientes[*n].CPF, "\n")] = 0;
    getchar();

    printf("Insira a data de nascimento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &clientes[*n].DATA_NASCIMENTO.DIA, &clientes[*n].DATA_NASCIMENTO.MES, &clientes[*n].DATA_NASCIMENTO.ANO);
    getchar();

    printf("Insira um telefone de contato: ");
    fgets(clientes[*n].TELEFONE, sizeof(clientes[*n].TELEFONE), stdin);
    clientes[*n].TELEFONE[strcspn(clientes[*n].TELEFONE, "\n")] = 0;
    getchar();

    printf("Insira o CEP: ");
    fgets(clientes[*n].CEP, sizeof(clientes[*n].CEP), stdin);
    clientes[*n].CEP[strcspn(clientes[*n].CEP, "\n")] = 0;
    getchar();
    
    printf("Insira o endereco: ");
    fgets(clientes[*n].ENDERECO, sizeof(clientes[*n].ENDERECO), stdin);
    clientes[*n].ENDERECO[strcspn(clientes[*n].ENDERECO, "\n")] = 0;
    getchar();
	    
    printf("Insira o local: ");
    fgets(clientes[*n].LOCAL, sizeof(clientes[*n].LOCAL), stdin);
    clientes[*n].LOCAL[strcspn(clientes[*n].LOCAL, "\n")] = 0;
    getchar();
    
    printf("Insira o numero da casa: ");
    fgets(clientes[*n].NUMERO_CASA, sizeof(clientes[*n].NUMERO_CASA), stdin);
    clientes[*n].NUMERO_CASA[strcspn(clientes[*n].NUMERO_CASA, "\n")] = 0;
    getchar();
    
    printf("Insira o bairro: ");
    fgets(clientes[*n].BAIRRO, sizeof(clientes[*n].BAIRRO), stdin);
    clientes[*n].BAIRRO[strcspn(clientes[*n].BAIRRO, "\n")] = 0;
    getchar();
    
    printf("Insira a cidade: ");
    fgets(clientes[*n].CIDADE, sizeof(clientes[*n].CIDADE), stdin);
    clientes[*n].CIDADE[strcspn(clientes[*n].CIDADE, "\n")] = 0;
    getchar();
    
    printf("Insira o estado: ");
    fgets(clientes[*n].ESTADO, sizeof(clientes[*n].ESTADO), stdin);
    clientes[*n].ESTADO[strcspn(clientes[*n].ESTADO, "\n")] = 0;
    getchar();
    
    printf("Insira a senha: ");
    fgets(clientes[*n].SENHA, sizeof(clientes[*n].SENHA), stdin);
    clientes[*n].SENHA[strcspn(clientes[*n].SENHA, "\n")] = 0;
    getchar();
    
    printf("Insira o limite da conta: ");
    scanf("%d", &clientes[*n].LIMITE_CONTA);
    getchar();
    
    printf("Insira a data de vencimento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &clientes[*n].DATA_VENCIMENTO.DIA, &clientes[*n].DATA_VENCIMENTO.MES, &clientes[*n].DATA_VENCIMENTO.ANO);
    getchar();
	    
    strcpy(clientes[*n].TIPO, "CP");

    (*n)++;
    system ("cls");
    printf("Cadastro da Conta Poupanca realizado com sucesso.\n");
     menuVolta(&admin, funcionarios, nomeArquivo, clientes, n, j); 
}


//ARQUIVO
//FUNÇÃO P/ O CADASTRO DE CONTA CORRENTE
void CC(CADASTRO* clientes, int* n, const char* nomeArquivo, CADASTRO** funcionarios, int* j) {
    system("cls");
    printf("Para realizar o cadastro da sua Conta Corrente precisamos de algumas informacoes.\n\n");

    printf("Insira a agencia: ");
    fgets(clientes[*n].AGENCIA, sizeof(clientes[*n].AGENCIA), stdin);
    clientes[*n].AGENCIA[strcspn(clientes[*n].AGENCIA, "\n")] = 0;
    getchar();
    
    printf("Insira o numero da conta: ");
    scanf("%d", &clientes[*n].NUMERO_CONTA);
    getchar();
    
    printf("Insira o seu nome: ");
    fgets(clientes[*n].NOME, sizeof(clientes[*n].NOME), stdin);
    clientes[*n].NOME[strcspn(clientes[*n].NOME, "\n")] = 0;
    getchar();
    
    printf("Insira o seu CPF: ");
    fgets(clientes[*n].CPF, sizeof(clientes[*n].CPF), stdin);
    clientes[*n].CPF[strcspn(clientes[*n].CPF, "\n")] = 0;
    getchar();
    
    printf("Insira a data de nascimento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &clientes[*n].DATA_NASCIMENTO.DIA, &clientes[*n].DATA_NASCIMENTO.MES, &clientes[*n].DATA_NASCIMENTO.ANO);
    getchar();
    
    printf("Insira um telefone de contato: ");
    fgets(clientes[*n].TELEFONE, sizeof(clientes[*n].TELEFONE), stdin);
    clientes[*n].TELEFONE[strcspn(clientes[*n].TELEFONE, "\n")] = 0;
    getchar();
    
    printf("Insira o endereco: ");
    fgets(clientes[*n].ENDERECO, sizeof(clientes[*n].ENDERECO), stdin);
    clientes[*n].ENDERECO[strcspn(clientes[*n].ENDERECO, "\n")] = 0;
    getchar();
    
    printf("Insira o CEP: ");
    fgets(clientes[*n].CEP, sizeof(clientes[*n].CEP), stdin);
    clientes[*n].CEP[strcspn(clientes[*n].CEP, "\n")] = 0;
    getchar();
    
    printf("Insira o local: ");
    fgets(clientes[*n].LOCAL, sizeof(clientes[*n].LOCAL), stdin);
    clientes[*n].LOCAL[strcspn(clientes[*n].LOCAL, "\n")] = 0;
    getchar();
    
    printf("Insira o numero da casa: ");
    fgets(clientes[*n].NUMERO_CASA, sizeof(clientes[*n].NUMERO_CASA), stdin);
    clientes[*n].NUMERO_CASA[strcspn(clientes[*n].NUMERO_CASA, "\n")] = 0;
    getchar();
    
    printf("Insira o bairro: ");
    fgets(clientes[*n].BAIRRO, sizeof(clientes[*n].BAIRRO), stdin);
    clientes[*n].BAIRRO[strcspn(clientes[*n].BAIRRO, "\n")] = 0;
    getchar();
    
    printf("Insira a cidade: ");
    fgets(clientes[*n].CIDADE, sizeof(clientes[*n].CIDADE), stdin);
    clientes[*n].CIDADE[strcspn(clientes[*n].CIDADE, "\n")] = 0;
    getchar();
    
    printf("Insira o estado: ");
    fgets(clientes[*n].ESTADO, sizeof(clientes[*n].ESTADO), stdin);
    clientes[*n].ESTADO[strcspn(clientes[*n].ESTADO, "\n")] = 0;
    getchar();
    
    printf("Insira a senha: ");
    fgets(clientes[*n].SENHA, sizeof(clientes[*n].SENHA), stdin);
    clientes[*n].SENHA[strcspn(clientes[*n].SENHA, "\n")] = 0;
    getchar();
	    
    printf("Insira o limite da conta: ");
    scanf("%d", &clientes[*n].LIMITE_CONTA);
    getchar();
    
    printf("Insira a data de vencimento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &clientes[*n].DATA_VENCIMENTO.DIA, &clientes[*n].DATA_VENCIMENTO.MES, &clientes[*n].DATA_VENCIMENTO.ANO);
    getchar();
    
    strcpy(clientes[*n].TIPO, "CC");

    (*n)++;
    system ("cls");
    printf("Cadastro da Conta Corrente realizado com sucesso.\n");
 menuVolta(&admin, funcionarios, nomeArquivo, clientes, n, j);
}


//ARQUIVO
//FUNÇÃO P/ CONSULTA DE CLIENTE
char* consultarCliente(CADASTRO* clientes, int n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    char nomeCliente[LIM];
    system("cls");
    printf("Digite o nome do cliente para consulta: ");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    nomeCliente[strcspn(nomeCliente, "\n")] = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(clientes[i].NOME, nomeCliente) == 0) {
            char* informacoes = (char*)malloc(1024 * sizeof(char));
            if (informacoes == NULL) {
                perror("Erro ao alocar memória");
                return NULL;
            }
            snprintf(informacoes, 1024,
                "Nome: %s\nCPF: %s\nData de Nascimento: %02d/%02d/%04d\nTelefone: %s\nEndereco: %s\nCEP: %s\nLocal: %s\nNumero da casa: %s\nBairro: %s\nCidade: %s\nEstado: %s\nAgencia: %s\nNumero da Conta: %d\nSaldo: %d\nLimite da Conta: %d\nData de Vencimento: %02d/%02d/%04d\nTipo de Conta: %s\n",
                clientes[i].NOME,
                clientes[i].CPF,
                clientes[i].DATA_NASCIMENTO.DIA, clientes[i].DATA_NASCIMENTO.MES, clientes[i].DATA_NASCIMENTO.ANO,
                clientes[i].TELEFONE,
                clientes[i].ENDERECO,
                clientes[i].CEP,
                clientes[i].LOCAL,
                clientes[i].NUMERO_CASA,
                clientes[i].BAIRRO,
                clientes[i].CIDADE,
                clientes[i].ESTADO,
                clientes[i].AGENCIA,
                clientes[i].NUMERO_CONTA,
                clientes[i].SALDO,
                clientes[i].LIMITE_CONTA,
                clientes[i].DATA_VENCIMENTO.DIA, clientes[i].DATA_VENCIMENTO.MES, clientes[i].DATA_VENCIMENTO.ANO,
                clientes[i].TIPO);
            return informacoes;
        }
    }
    printf("Cliente nao encontrado.\n");
    return NULL;
    menuVolta(admin, funcionarios, nomeArquivo, clientes, &n, j); 
}

//FUNÇÃO P/ ESCREVER NO ARQUIVO (CLIENTE)
void escreverNoArquivo_Cliente(const char* nomeArquivo, CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, int* j) {
    char* informacoes = consultarCliente(clientes, *n, admin, funcionarios, nomeArquivo, j); // Corrigir a chamada para consultarCliente
    if (informacoes == NULL) {
        return;
    }

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        free(informacoes);
        return;
    }

    fprintf(arquivo, "%s\n", informacoes);
    fclose(arquivo);
    free(informacoes);
    menuVolta(admin, funcionarios, nomeArquivo, clientes, n, j); 
}


//FUNÇÃO P/ MENU PRINCIPAL DE FUNCIONARIO
void telaFuncionario(ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, CADASTRO* clientes, int* n, int* j) {
    int senhaCorreta;
    verificarSenhaAdmin(admin, &senhaCorreta);
    system("cls");

    if (!senhaCorreta) {
        printf("Acesso negado. Encerrando o acesso ao menu do funcionario.\n");
        return;
    }

    int opcao;
    do {
        system("cls");
        printf("Menu do funcionario.\n\n");
        printf("1. Abertura de conta\n");
        printf("2. Encerramento de conta\n");
        printf("3. Consultar dados\n");
        printf("4. Alterar dados\n");
        printf("5. Cadastro de funcionarios\n");
        printf("6. Gerar relatorios\n");
        printf("7. Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrar(funcionarios, clientes, nomeArquivo, n, j);
                break;
            case 2:
                encerrarConta(admin, funcionarios, clientes, nomeArquivo, n);
                break;
            case 3:
                gerarRelatorios(clientes, *funcionarios,(int) n, nomeArquivo, admin);
               break;
            case 4:
                alterarDados(clientes,(int) n, funcionarios, nomeArquivo);
                break;
            case 5:
                cadastro_funcionarios(funcionarios, n, nomeArquivo, clientes, j);
                break;
            case 6:
                alterarDados(clientes,(int) n,funcionarios, "dadosClientes.txt");
                break;
            case 7:
                printf("Saindo do menu do funcionario.\n");
                break;
            default:
                printf("Opcao invalida. Digite o numero de alguma opcao presente no menu:\n");
                break;
        }
    } while (opcao != 7);
}



//ARQUIVO
//FUNÇÃO P/ MENU DE CADASTRO DE CONTA CC OU CP
void cadastrar(CADASTRO** funcionarios, CADASTRO* clientes, const char* nomeArquivo, int* n, int* j) {
    int opcao;
    do {
        system("cls");
        printf("*CADASTRO DE CONTA*\n");
        printf("\n1. Conta Corrente\n");
        printf("2. Conta Poupanca\n");
        printf("3. Voltar para o menu anterior\n");
        printf("\nDigite o numero da opcao desejada: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                CC(clientes, n, nomeArquivo, funcionarios, j);
                escreverNoArquivo_Cliente(nomeArquivo, clientes, n, &admin, funcionarios, j);
                break;
            case 2:
                CP(clientes, n, nomeArquivo, funcionarios, j);
                escreverNoArquivo_Cliente(nomeArquivo, clientes, n, &admin, funcionarios, j);
                break;
            case 3:
                printf("Saindo do cadastro.\n");
                Sleep(1000);
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 3);
}

//ARQUIVO
//VARIAVEL P/ REALIZAÇÃO DE VERIFICACAO DE TENTATIVA DE SENHA NAS FUNÇÕES A SEGUIR
int tentativaSenha=0;

//FUNÇÃO P/ VISUALIZAR O SALDO DO CLIENTE
void saldo(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    char senhaM[50];
    system("cls");
    printf("SALDO\n\n");

    while (tentativaSenha < 3) {
        printf("Digite sua senha: ");
        scanf("%49s", senhaM);

        if (strcmp(senhaM, clientes[*n].SENHA) != 0) {
            printf("\nSenha incorreta, tente novamente.\n\n");
        } else {
            printf("Seu saldo é de: R$ %d\n", clientes[*n].SALDO);
            menuVolta(admin, funcionarios, nomeArquivo, clientes, n, j);
            break;
        }
        tentativaSenha++;

        if (tentativaSenha == 3) {
            system("cls");
            printf("Limite de tentativas de senha atingido. Programa encerrado.\n");
            exit(0);
        }
    }
}


//FUNÇÃO P/ A REALIZAÇÃO DO DEPOSITO DO CLIENTE
void deposito(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    float valorDep;

    system("cls");
    printf("Digite o valor do deposito: \n");
    scanf("%f", &valorDep);
    printf("Valor de %.2f depositado na conta com sucesso!\n", valorDep);

    clientes[*n].SALDO =clientes[*n].SALDO+ valorDep;
    printf ("Novo Saldo: %s",clientes[*n].SALDO);
menuVolta(admin, funcionarios, nomeArquivo, clientes, n, j); ;
}

//FUNÇÃO P/ A REALIZAÇÃO DO SAQUE DO CLIENTE
void saque(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    char senhaM[50];
    float valorSaq;

    system("cls");

    while(tentativaSenha < 3){
        printf("Digite sua senha: ");
        scanf("%49s", senhaM);

        if(strcmp(senhaM, clientes[*n].SENHA) != 0){
            printf("\nSenha incorreta, tente novamente.\n\n");
        } else {
            printf("\nDigite o valor do saque: ");
            scanf("%f", &valorSaq);

            if(valorSaq > clientes[*n].SALDO){
                printf("\nSaldo insuficiente\n");
            } else {
                printf("Saque realizado com sucesso!\n");
                clientes[*n].SALDO -= valorSaq;
                menuVolta(admin, funcionarios, nomeArquivo, clientes, n, j); 
                 break;
            }
        }
        tentativaSenha++;

        if (tentativaSenha == 3){
            system("cls");
            printf("Limite de tentativas de senha atingido. Programa encerrado.\n");
            exit(0);
        }
    }
}

//FUNÇÃO P/ A VISUALIZAÇÃO DO EXTRATO DO CLIENTE NO EXCEL
int* extrato(CADASTRO* clientes, int* n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    FILE *arq;
    char senhaM[50];

    system("cls");

    while(tentativaSenha < 3){
        printf("Digite sua senha: ");
        scanf("%49s", senhaM);

        if(strcmp(senhaM, clientes[*n].SENHA) != 0){
            printf("\nSenha incorreta, tente novamente.\n\n");
        } else {
            printf("\nAbrindo Extrato: ");
            system("start Extrato.csv");
            if (arq == NULL){
                printf("Erro na abertura do arquivo\n");
                system("pause");
                exit(0);
            }
            menuVolta(admin, funcionarios, nomeArquivo, clientes, n, j);;
            break;
        }

        tentativaSenha++;

        if (tentativaSenha == 3){
            system("cls");
            printf("Limite de tentativas de senha atingido. Programa encerrado.\n");
            exit(0);
        }
    }
}

//FUNÇÃO P/ A CONSULTA DO LIMITE DA CONTA DO CIENTE
int consultLimite(CADASTRO* clientes, int n, ADMINISTRADOR* admin, CADASTRO** funcionarios, const char* nomeArquivo, int* j) {
    float valorLimite;
    char senhaM[50];
    system("cls");

    while (tentativaSenha < 3) {
        printf("Digite sua senha: ");
        scanf("%49s", senhaM);

        if (strcmp(senhaM, clientes[n].SENHA) != 0) {
            printf("\nSenha incorreta, tente novamente.\n\n");
        } else {
            printf("\nDigite o valor: ");
            scanf("%f", &valorLimite);

            int clienteEncontrado = 0;
            for (int i = 0; i < n; i++) {
                if (valorLimite <= (clientes[i].SALDO + clientes[i].LIMITE_CONTA)) {
                    printf("O limite de %.2f esta disponivel para o cliente %s.\n", valorLimite, clientes[i].NOME);
                    clienteEncontrado = 1;
                    break;
                }
            }
            if (!clienteEncontrado) {
                printf("O limite de %.2f nao esta disponivel.\n", valorLimite);
            }

            menuVolta(admin, funcionarios, nomeArquivo, clientes, &n, j);
            return 0;
            break;
        }

        tentativaSenha++;

        if (tentativaSenha == 3) {
            system("cls");
            printf("Limite de tentativas de senha atingido. Programa encerrado.\n");
            exit(0);
        }
    }
    return 0;
}

//FUNÇÃO DO MENU PRINCIPAL DO BANCO
void menuPrincipal() {
    int opcao;
    int n = 0, j = 0;
    const char* nomeArquivo = "dadosClientes.txt";
    CADASTRO *clientes = NULL;
    CADASTRO *funcionarios = NULL;

    do {
        system("cls");
        printf("########     ###    ##    ##  ######   #######     ##     ##    ###    ##       ##     ##    ###    ########  ######## ########\n");
        printf("##     ##   ## ##   ###   ## ##    ## ##     ##    ###   ###   ## ##   ##       ##     ##   ## ##   ##     ## ##       ##     ##\n");
        printf("##     ##  ##   ##  ####  ## ##       ##     ##    #### ####  ##   ##  ##       ##     ##  ##   ##  ##     ## ##       ##     ##\n");
        printf("########  ##     ## ## ## ## ##       ##     ##    ## ### ## ##     ## ##       ##     ## ##     ## ##     ## ######   ########\n");
        printf("##     ## ######### ##  #### ##       ##     ##    ##     ## ######### ##        ##   ##  ######### ##     ## ##       ##   ##\n");
        printf("##     ## ##     ## ##   ### ##    ## ##     ##    ##     ## ##     ## ##         ## ##   ##     ## ##     ## ##       ##    ##\n");
        printf("########  ##     ## ##    ##  ######   #######     ##     ## ##     ## ########    ###    ##     ## ########  ######## ##     ##\n");

        printf("\n\n*SEJA BEM VINDO!*\n\n");
        printf("1. Area de funcionario\n");
        printf("2. Area de cliente\n");
        printf("3. Sair\n");
        printf("\nDigite o numero da opcao desejada: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                clientes = (CADASTRO *)realloc(clientes, (n + 1) * sizeof(CADASTRO));
                if (clientes == NULL) {
                    printf("Erro ao alocar memoria\n");
                    exit(1);
                }
                telaFuncionario(&admin, &funcionarios, nomeArquivo, clientes, &n, &j);
                break;
            case 2:
                do {
                    system("cls");
                    printf("Menu do Cliente:\n\n");
                    printf("1. Saldo\n");
                    printf("2. Deposito\n");
                    printf("3. Saque\n");
                    printf("4. Extrato\n");
                    printf("5. Consultar limite\n");
                    printf("6. Sair\n");
                    printf("\nDigite o numero da opcao desejada: ");
                    scanf("%d", &opcao);

                    switch (opcao) {
                        case 1:
                            saldo(clientes, &n, &admin, &funcionarios, nomeArquivo, &j);
                            break;

                        case 2:
                            deposito(clientes, &n, &admin, &funcionarios, nomeArquivo, &j);
                            break;

                        case 3:
                            saque(clientes, &n, &admin, &funcionarios, nomeArquivo, &j);
                            break;

                        case 4:
                            extrato(clientes, &n, &admin, &funcionarios, nomeArquivo, &j);
                            break;

                        case 5:
                            consultLimite(clientes, n, &admin, &funcionarios, nomeArquivo, &j);
                            break;

                        case 6:
                            break;
                        default:
                            printf("Opcao invalida. Digite o numero de alguma opcao presente no menu:\n");
                    }
                } while (opcao != 6);
                break;
            case 3:
                printf("Saindo do programa.\n");
                free(clientes);
                free(funcionarios);
                Sleep(1000);
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
                exit(1);
        }
    } while (opcao != 3);
}

