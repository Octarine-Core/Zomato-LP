#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "sisutil.h"
#include "LP_Leitura.h"
#define PATH "./utilizadores/"
#define URL "http://ip-api.com/line"

/*
 * 
 *
 * Manegedor de utilizadores, e dos seus ficheiros.
 * 
 * Ver o README
 *
 */

//Verificar se nome existe em utilizadores, usado frequentemente para evitar ler ficheiros que não existem
//
//Declaração precoce da estutura infoTag, que será renomeada para info
struct infoTag;

int usrExiste(char* nome) {

    FILE *fich;
    char path[32] = PATH;
    strcat(path, nome);
    strcat(path, ".txt");

    if (fich = fopen(path, "r")) {
        fclose(fich);
        return 1;
    } else {
        return 0;
    }
}
//Gera um path relativo ao ficheiro de execução para um determinado user (com .txt)

char* getPath(char* nome) {

    char fileNome[32];

    char* path = malloc(100);

    strcpy(fileNome, nome);

    strcpy(path, PATH);

    strcat(fileNome, ".txt");
    strcat(path, fileNome);
    return path;
}

//Função mais complexa do programa, com 4 valores de retorno

int login(char* nome, char* pass) {

    char *fpw = malloc(32);
    FILE *fch;

    if (usrExiste(nome) == 1) {
        if (fch = fopen(getPath(nome), "r")) {
            // Return -1 se não conseguir ler pass
            if (fgets(fpw, 33, fch) == NULL) {

                fclose(fch);
                return (-1);
            } else {

                // Tira o newline da fpw (file password)
                fpw[strlen(fpw) - 1] = 0;
                // Return 1 se login sucedido
                if (strcmp(pass, fpw) == 0) {

                    fclose(fch);
                    return 1;
                }// Return 0 se pass errada
                else {


                    fclose(fch);
                    return 0;
                }
            }


        }// Return -2 se erro ao abrir ficheiro
        else {
            return (-2);
        }

    }// Return -3 se usr nao existe
    else
        return (-3);
}

// Escreve dados(pass, e localização) no ficheiro automaticamente. Usa a Ip-Api.com e a libcurl

int autoLoc(char * nome, const char * pass) {

    FILE* fch;
    CURL *curl;
    CURLcode res;
    char password[32];
    strcpy(password, pass);
    const char* constline;
    char* line = malloc(64);

    if (fch = fopen(getPath(nome), "w+")) {

        // Escreve a pass no ficheiro

        fputs(password, fch);
        fputs("\n", fch);
        // Escreve os dados de localização 
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, URL);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fch);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        fputs("0", fch);
        fclose(fch);

        fch = fopen(getPath(nome), "r");
        fgets(line, 64, fch);
        fgets(line, 64, fch);

        if (strcmp(line, "success") == 0) {

            return 1; //Pedido bem sucedido

        } else {

            return -1;
        }
    } else {
        return 0;
    }
}
//Escreve os dados manualmente (1 para bem sucedido, 0 para usr já existe, -1 para erro inesperado

int manLoc(char* nome, char* pass, info dados) {
    if (usrExiste(nome) == 0) {
        FILE* fch;
        if (fch = fopen(getPath(nome), "w+")) {
            fputs(pass, fch);
            fputs("\n", fch);
            fputs("\n", fch);
            fputs(dados.Pais, fch);
            fputs("\n\n\n", fch);
            fputs(dados.Regiao, fch);
            fputs("\n", fch);
            fputs(dados.Cidade, fch);
            fputs("\n", fch);
            fputs(dados.Postal, fch);
            fputs("\n", fch);
            for (int i = 0; i < 7; i++)
                fputs("\n", fch);
            fputs(dados.Morada, fch);
            fclose(fch);
            return 1;
        } else {
            return -1;
        }
    } else {
        return 0;
    }
}
//Tenta remover um ficheiro, 1 para bem-sucedido, 0 para erro, -1 para login errado

int apagarConta(char* nome, char* pass) {
    const char* path = getPath(nome);

    if (login(nome, pass)) {
        if (remove(path) == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

//Lê um dado especificado

char* lerLoc(char* nome, char flag) {

    //nome = nome do usr,
    //flag = O que lêr( P = País, R = Região, C = Cidade, O = Codigo Postal da cidade

    FILE* fch;
    char* res = malloc(512);
    int line;

    switch (flag) {
            //País
        case 'P':
            line = 3;
            break;
            //Região
        case 'R':
            line = 6;
            break;
            //Cidade
        case 'C':
            line = 7;
            break;
            //Codigo Postal (da cidade, xxxx-000)
        case 'O':
            line = 8;
            break;
            //Morada
        case 'M':
            line = 16;
            break;

    }
    if (fch = fopen(getPath(nome), "r")) {
        fseek(fch, 0, SEEK_SET);
        for (int i = 0; i < line; i++) {

            fgets(res, 256, fch);
        }
        fclose(fch);
        return (res);
    }
}


// Muda a pass

int mudarPass(char* nome, char* pass, char* novaPass) {

    // Um ficheiro temporário (TEMP) fica com os dados, e depois o ficheiro original é apagado, para 
    // renomear o novo ficheiro TEMP para o nome do utilizador. É a maneira mais fácil de editar uma
    // linha, estranhamente.

    FILE* fch;
    FILE* fchTemp;
    char* nomeTemp = "TEMP";
    char* line = malloc(64);

    int pos = 0;
    const char* oldName = getPath(nomeTemp);
    const char* newName = getPath(nome);


    
        printf("Login bem sucedido\n");
        if (fch = fopen(getPath(nome), "r+")) {
            printf("Acedendo a conta... \n");
            printf("Antiga pass %s, nova pass %s", pass, novaPass);
            fchTemp = fopen(getPath(nomeTemp), "w");
            fputs(novaPass, fchTemp);
            while (fgetc(fch) != '\n') {
                pos++;
            }
            fseek(fch, pos, SEEK_SET);

            while (fgets(line, 64, fch) != NULL) {
                fputs(line, fchTemp);
            }

            fclose(fch);
            fclose(fchTemp);
            remove(newName);
            rename(oldName, newName);
            return 1;
        } else {
            return (-1);
        }
    
}

//Função que devolve uma instância da estrutura "info", usando o stdin (input do teclado),
//Nao usa "scanf()" porque dá problemas quando usa espaços, ou newlines. A cada string é retrado o \n
//E subsituido por um \0 (NULL)

info introduzirInfo() {
    info dadosTemp;
    getchar();
    printf("Introduzir Pais:");
    fgets(dadosTemp.Pais, 32, stdin);
    if ((strlen(dadosTemp.Pais) > 0) && (dadosTemp.Pais[strlen(dadosTemp.Pais) - 1] == '\n'))
        dadosTemp.Pais[strlen(dadosTemp.Pais) - 1] = '\0';
    printf("Introduzir Regiao:");
    fgets(dadosTemp.Regiao, 32, stdin);
    if ((strlen(dadosTemp.Regiao) > 0) && (dadosTemp.Regiao[strlen(dadosTemp.Regiao) - 1] == '\n'))
        dadosTemp.Regiao[strlen(dadosTemp.Regiao) - 1] = '\0';
    printf("Introduzir Cidade:");
    fgets(dadosTemp.Cidade, 32, stdin);
    if ((strlen(dadosTemp.Cidade) > 0) && (dadosTemp.Cidade[strlen(dadosTemp.Cidade) - 1] == '\n'))
        dadosTemp.Cidade[strlen(dadosTemp.Cidade) - 1] = '\0';
    printf("Introduzir Codigo Postal:");
    fgets(dadosTemp.Postal, 32, stdin);
    if ((strlen(dadosTemp.Postal) > 0) && (dadosTemp.Postal[strlen(dadosTemp.Postal) - 1] == '\n'))
        dadosTemp.Postal[strlen(dadosTemp.Postal) - 1] = '\0';
    printf("Introduzir Morada:");
    fgets(dadosTemp.Morada, 256, stdin);
    if ((strlen(dadosTemp.Morada) > 0) && (dadosTemp.Morada[strlen(dadosTemp.Morada) - 1] == '\n'))
        dadosTemp.Morada[strlen(dadosTemp.Morada) - 1] = '\0';

    return dadosTemp;
}

void menu_user() {
    printf("********************  GESTÃO DE UTILIZADORES  *********************\n");
    printf("*                                                                 *\n");
    printf("*  A- Criar Conta Automaticamente (acesso à internet necessário)  *\n");
    printf("*  B- Criar Conta manualmente                                     *\n");
    printf("*  C- Pesquisa de restaurantes                                    *\n");
    printf("*  E- Consultar informações da conta                              *\n");
    printf("*  F- Mudar uma palavra-passe                                     *\n");
    printf("*  Q- Sair do sistema de utilizadores                             *\n");
    printf("*                                                                 *\n");
    printf("*******************************************************************\n");
}

char* usrMenu() {

    int buff;
    char opc;
    char *nome = malloc(32);
    char *pass = malloc(32);
    char *novaPass = malloc(32);
    do {
        do {
            menu_user();
            //scanf(" %c", &opc);
            readChar(&opc, "Introduza a opção: ");

        } while (opc != 'A' && opc != 'B' && opc != 'C' && opc != 'D' && opc != 'Q' && opc != 'E' && opc != 'F' && opc != 'a' && opc != 'b' && opc != 'c'&& opc != 'd'&& opc != 'e'&& opc != 'f'&& opc != 'q');
        switch (opc) {
            case 'a':
            case 'A':
                system("clear");
                printf("\t\tCriação Automática da Conta\n");
                
                //printf("Introduzir nome: \n");
                //scanf("%s", nome);
                readString(nome, sizeof (nome), "Introduzir nome: ");
                /*
                                printf("Introduzir password para a conta... \n");
                                scanf("%s", pass);
                 * */
                readString(pass, sizeof (pass), "Introduzir password: ");

                printf("\n\n\n\n");


                if (usrExiste(nome) != 1) {

                    autoLoc(nome, pass);
                    printf("Conta criada automaticamente para o utilizador %s, com "
                            "password %s\n", nome, pass);
                } else {
                    printf("O utilizador já existe.\n");

                }
                printf("\n\n\n\n");
                break;
            case 'b':
            case 'B':
                system("clear");
                printf("\t\tCriação Manual da Conta\n");
                
                /*
                printf("Introduzir nome...\n");
                scanf("%s", nome);
                 */
                readString(nome, sizeof (nome), "Introduzir nome: ");
                /*
                printf("Introduzir pass...\n");
                scanf("%s", pass);
                 */
                readString(pass, sizeof (pass), "Introduzir password: ");

                if (manLoc(nome, pass, introduzirInfo()))
                    printf("Conta criada com user: %s e password %s\n", nome, pass);
                else
                    printf("Conta já existe.\n");

                printf("\n\n\n\n");
                break;
            case 'c':
            case 'C':
                //Gostava que nesta opção depois de fazer o login, redirecionasse para o meu menu de pesquisa
                readString(nome, sizeof (nome), "Introduzir username: \n");

                readString(pass, sizeof (pass), "Introduzir password: \n");

                if (login(nome, pass) == 1) {
                    printf("Login bem-sucedido!\n");
                    return lerLoc(nome, 'C');
                } else if (login(nome, pass) == 0)
                    printf("Password errada.\n");
                else if (login(nome, pass) == -1)
                    printf("Erro a ler password.\n");
                else if (login(nome, pass) == -2)
                    printf("Erro a ler ficheiro.\n");
                else if (login(nome, pass) == -3)
                    printf("Utilizador não existe.\n");

                printf("\n\n\n\n");
                break;
                
            case 'd':
            case 'D':
                
                system("clear");
                printf("\t\tApagar Conta\n");
                
                readString(nome, sizeof (nome), "Introduzir username: ");
                readString(pass, sizeof (pass), "Introduzir password: ");

                if (apagarConta(nome, pass) == 1) {
                    printf("Apagou conta do utilizador %s\n", nome);
                } else if (apagarConta(nome, pass) == -1) {
                    printf("Password errada!\n");
                } else {
                    printf("Erro!\n");
                }
                printf("\n\n\n\n");
                break;

            case 'e':
            case 'E':
                system("clear");
                printf("\t\tVisualização de Informações de Conta\n");
                
                //printf("Fazer Login\n");

                readString(nome, sizeof (nome), "Introduzir nome: ");
                readString(pass, sizeof (pass), "Introduzir password para a conta: ");

                if (login(nome, pass) == 1) {
                    printf("\n\n");
                    printf("%s", lerLoc(nome, 'P'));
                    printf("%s", lerLoc(nome, 'R'));
                    printf("%s", lerLoc(nome, 'C'));
                    printf("%s", lerLoc(nome, 'O'));
                    printf("%s\n\n", lerLoc(nome, 'M'));
                    printf("Introduza qualquer numero para continuar..");
                    scanf("%d", &buff);
                } else {
                    printf("Username ou password incorreto, tente novamente...\n");
                }
                printf("\n\n\n\n");
                break;

            case 'f':
            case 'F':
                system("clear");
                printf("\t\tAlteração da Password\n");
                
                readString(nome, sizeof (nome), "Introduzir nome: ");
                readString(pass, sizeof (pass), "Introduzir password: ");

                if (login(nome, pass)) {
                    readString(novaPass, sizeof (novaPass), "Introduzir nova password para a conta: \n");
                    mudarPass(nome, pass, novaPass);
                } else {
                    printf("Login errado!\n");
                }
                printf("\n\n\n\n");
                break;
        }
    } while (opc != 'Q' && opc != 'q');
    return "0";
}
