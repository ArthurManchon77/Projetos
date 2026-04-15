#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>



	// DEFINIÇÃO DAS STRUCTS


typedef struct {
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct {
    char nome[50];
    char senha[20];       // 19 dígitos + '\0'
    char email[50];      // email de até 49 caracteres
} Cliente;



	// FUNÇÕES AUXILIARES


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Preço deve ser positivo
int validarPreco(float preco) {
    return preco > 0;
}

// senha deve ter 4 dígitos numéricos
int validarSenha(const char *senha) {
    return strlen(senha) >= 4;  // mínimo 4 caracteres
}
//validar email
int validarEmail(const char *email) {
    return strchr(email, '@') != NULL;
}




	//	CADASTRAR PRODUTO


void cadastrarProduto() {
    Produto p;
    FILE *arquivo;

    printf("\n=== Cadastro de Produto ===\n");
    limparBuffer();

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    do {
        printf("Preço: R$ ");
        scanf("%f", &p.preco);
        if (!validarPreco(p.preco)) {
            printf("Preço inválido! O preço deve ser positivo.\n");
        }
    } while (!validarPreco(p.preco));

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    arquivo = fopen("produtos.csv", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de produtos");
        return;
    }

    fprintf(arquivo, "%s,%.2f,%d\n", p.nome, p.preco, p.quantidade);
    fclose(arquivo);

    printf("Produto cadastrado com sucesso!\n");
}



	// LISTAR PRODUTOS


void lerProdutos() {
    Produto p;
    FILE *arquivo;

    arquivo = fopen("produtos.csv", "r");
    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado ou erro ao abrir o arquivo.\n");
        return;
    }

    printf("\n=== Lista de Produtos ===\n");

    // lê até o fim do arquivo
    while (fscanf(arquivo, " %49[^,],%f,%d", p.nome, &p.preco, &p.quantidade) == 3) {
        printf("Nome: %s | Preco: %.2f | Qtde: %d\n",
               p.nome, p.preco, p.quantidade);
    }

    fclose(arquivo);
}


	// CADASTRAR CLIENTE


void cadastrarCliente() {
    Cliente c;
    FILE *arquivo;

    printf("\n=== Cadastro de Cliente ===\n");
    limparBuffer();

    printf("Nome: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    // SENHA
    do {
        printf("Senha (mínimo 4 caracteres): ");
        fgets(c.senha, sizeof(c.senha), stdin);
        c.senha[strcspn(c.senha, "\n")] = '\0';

        if (!validarSenha(c.senha)) {
            printf("Senha inválida! Muito curta.\n");
        }
    } while (!validarSenha(c.senha));

    // EMAIL
    do {
        printf("Email: ");
        fgets(c.email, sizeof(c.email), stdin);
        c.email[strcspn(c.email, "\n")] = '\0';

        if (!validarEmail(c.email)) {
            printf("Email inválido! Deve conter '@'.\n");
        }
    } while (!validarEmail(c.email));

    arquivo = fopen("clientes.csv", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de clientes");
        return;
    }

    fprintf(arquivo, "%s,%s,%s\n", c.nome, c.senha, c.email);
    fclose(arquivo);

    printf("Cliente cadastrado com sucesso!\n");
}



	// LISTAR CLIENTES


    void lerClientes() {
    Cliente c;
    FILE *arquivo = fopen("clientes.csv", "r");
    char linha[256];

    if (arquivo == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n=== Lista de Clientes ===\n");
    printf("| %-20s | %-12s | %-30s |\n", "NOME", "SENHA", "EMAIL");
    printf("-----------------------------------------------------------------------\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        // Remove \n e \r
        linha[strcspn(linha, "\r\n")] = '\0';

        // Divide nos campos
        char *nome = strtok(linha, ",");
        char *senha = strtok(NULL, ",");
        char *email = strtok(NULL, ",");

        // Se faltar algum campo, pula
        if (!nome || !senha || !email)
            continue;

        strncpy(c.nome, nome, sizeof(c.nome));
        strncpy(c.senha, senha, sizeof(c.senha));
        strncpy(c.email, email, sizeof(c.email));

        printf("| %-20s | %-12s | %-30s |\n",
               c.nome, c.senha, c.email);
    }

    printf("-----------------------------------------------------------------------\n");
    fclose(arquivo);
}

    
  	// APAGAR PRODUTOS
    
void apagarProduto() {
    char nomeRemover[50];
    Produto p;
    FILE *arquivo, *temp;

    limparBuffer(); // limpar buffer para ler textos corretamente

    printf("\n=== Apagar Produto ===\n");
    printf("Digite o nome EXATO do produto que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0'; // remove o \n

    arquivo = fopen("produtos.csv", "r");
    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    int removido = 0;

    while (fscanf(arquivo, " %49[^,],%f,%d", p.nome, &p.preco, &p.quantidade) == 3) {
        if (strcmp(p.nome, nomeRemover) == 0) {
            removido = 1;  
        } else {
            fprintf(temp, "%s,%.2f,%d\n", p.nome, p.preco, p.quantidade);
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Apaga o arquivo original
    remove("produtos.csv");

    // Renomeia o arquivo temporário
    rename("temp.csv", "produtos.csv");

    if (removido)
        printf("\nProduto removido com sucesso!\n");
    else
        printf("\nProduto NÃO encontrado.\n");
}
 
	// APAGAR CLIENTES

void apagarCliente() {
    char nomeRemover[50];
    Cliente c;
    FILE *arquivo, *temp;
    char linha[256];

    limparBuffer(); // limpar buffer para ler corretamente

    printf("\n=== Apagar Cliente ===\n");
    printf("Digite o nome EXATO do cliente que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    arquivo = fopen("clientes.csv", "r");
    if (arquivo == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    int removido = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        // Remove quebra de linha (\n, \r)
        linha[strcspn(linha, "\r\n")] = '\0';

        // Quebra nos campos
        char *nome = strtok(linha, ",");
        char *senha = strtok(NULL, ",");
        char *email = strtok(NULL, ",");

        if (!nome || !senha || !email)
            continue;

        // Verifica se é o cliente a remover
        if (strcmp(nome, nomeRemover) == 0) {
            removido = 1; // Não grava -> apagado
        } else {
            fprintf(temp, "%s,%s,%s\n", nome, senha, email);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("clientes.csv");
    rename("temp.csv", "clientes.csv");

    if (removido)
        printf("\nCliente removido com sucesso!\n");
    else
        printf("\nCliente NÃO encontrado.\n");
}


   // MENU PRINCIPAL


void exibirMenu() {
    printf("\n============================================\n");
    printf("        SISTEMA DE CADASTRO (CSV)\n");
    printf("============================================\n");
    printf("1. Cadastrar Produto\n");
    printf("2. Listar Produtos\n");
    printf("3. Apagar Produto\n");
    printf("4. Cadastrar Clientes\n");
    printf("5. Listar Clientes\n");
    printf("6. Apagar Clientes\n");
    printf("0. Sair\n");
    printf("--------------------------------------------\n");
    printf("Escolha uma opção: ");
}



int main() {
    setlocale(LC_ALL, "Portuguese");
    

    int opcao;

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            printf("Entrada inválida! Digite um número.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: lerProdutos(); break;
            case 3: apagarProduto(); break;
            case 4: cadastrarCliente(); break;
            case 5: lerClientes(); break;
            case 6: apagarCliente(); break;
            case 0: printf("Saindo... Até mais!\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

