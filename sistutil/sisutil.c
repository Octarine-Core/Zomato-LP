#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "sisutil.h"
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

int usrExiste(char* nome){

    FILE *fich;
    char path[32] = PATH;
    strcat(path, nome);
    strcat(path, ".txt");
    
    if (fich = fopen(path, "r")){
        fclose(fich);
        return 1;
    }
    else{
        return 0;
    }
}
//Gera um path relativo ao ficheiro de execução para um determinado usr (com .txt)
char* getPath(char* nome){

    char fileNome[32];
    
    char* path= malloc(100);
    
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

    if( usrExiste(nome) == 1){
        if(fch = fopen(getPath(nome), "r")){
            // Return -1 se não conseguir ler pass
            if(fgets(fpw, 33 , fch) == NULL){
                
                fclose(fch);
                return (-1);
            }
            else{

                // Tira o newline da fpw (file password)
                fpw[strlen(fpw)-1] = 0;
                // Return 1 se login sucedido
                if(strcmp(pass, fpw) == 0){

                    fclose(fch);
                    return 1;
                }
                // Return 0 se pass errada
                else{
                    
                    
                    fclose(fch);
                    return 0;
                }
            }
            
            
        }
        // Return -2 se erro ao abrir ficheiro
        else{
           return (-2);
        }
        
    }
    // Return -3 se usr nao existe
    else
        return (-3);
}

// Escreve dados(pass, e localização) no ficheiro automaticamente. Usa a Ip-Api.com e a libcurl
int autoLoc(char * nome, const char * pass){

    FILE* fch;
    CURL *curl;
    CURLcode res;
    char password[32];
    strcpy(password, pass);
    const char* constline;
    char* line = malloc(64);

    if(fch = fopen(getPath(nome), "w+")){

        // Escreve a pass no ficheiro

        fputs(password, fch);
        fputs("\n", fch);      
        // Escreve os dados de localização 
        curl = curl_easy_init();
        if(curl){
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

        if(strcmp(line, "success") == 0){

            return 1; //Pedido bem sucedido

        }
        else{

            return -1;
        }
    }
    else{
        return 0;
    }
}
//Escreve os dados manualmente (1 para bem sucedido, 0 para usr já existe, -1 para erro inesperado
int manLoc(char* nome, char* pass, info dados){
    if(usrExiste(nome) == 0){
        FILE* fch;
        if(fch = fopen(getPath(nome), "w+")){
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
            for(int i = 0; i<7; i++)
                fputs("\n", fch);
            fputs(dados.Morada, fch);
            fclose(fch);
            return 1;
        }
        else{
            return -1;
        }
    }
    else{
        return 0;
    }
}
//Tenta remover um ficheiro, 1 para bem-sucedido, 0 para erro, -1 para login errado
int apagarConta(char* nome, char* pass){
    const char* path = getPath(nome);

    if(login(nome, pass)){
        if(remove(path) == 0){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return -1;
    }
}

//Lê um dado especificado
char* lerLoc(char* nome, char flag){

    //nome = nome do usr,
    //flag = O que lêr( P = País, R = Região, C = Cidade, O = Codigo Postal da cidade

    FILE* fch;
    char* res = malloc(512);
    int line;

    switch(flag){
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
    if(fch = fopen(getPath(nome), "r")){
        fseek(fch, 0, SEEK_SET);
        for(int i = 0; i< line; i++){

            fgets(res, 256 , fch);
        }
        fclose(fch);
        return (res);
    }
}


// Muda a pass (duh)
int mudarPass(char* nome, char* pass, char* novaPass){

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


    if(login(nome, pass)){
        if(fch = fopen(getPath(nome), "r+")){
            fchTemp = fopen(getPath(nomeTemp), "w");
            fputs(novaPass, fchTemp);
            while(fgetc(fch)!= '\n'){
                pos++;
            }
            fseek(fch, pos, SEEK_SET);

            while(fgets(line, 64, fch) != NULL){
                fputs(line, fchTemp);
            }

            fclose(fch);
            fclose(fchTemp);
            remove(newName);
            rename(oldName, newName);
            return 1;
        }
        else{
            return (-1);
        }
    }
    else{
        return 0;
    }
}

//Função que devolve uma instância da estrutura "info", usando o stdin (input do teclado),
//Nao usa "scanf()" porque dá problemas quando usa espaços, ou newlines. A cada string é retrado o \n
//E subsituido por um \0 (NULL)
info introduzirInfo(){
    info dadosTemp;
    getchar();
    printf("Introduzir Pais:");
    fgets(dadosTemp.Pais, 32, stdin);
    if ((strlen(dadosTemp.Pais) > 0) && (dadosTemp.Pais[strlen (dadosTemp.Pais) - 1] == '\n'))
        dadosTemp.Pais[strlen (dadosTemp.Pais) - 1] = '\0';
    printf("Introduzir Regiao:");
    fgets(dadosTemp.Regiao, 32, stdin);
    if ((strlen(dadosTemp.Regiao) > 0) && (dadosTemp.Regiao[strlen (dadosTemp.Regiao) - 1] == '\n'))
                dadosTemp.Regiao[strlen (dadosTemp.Regiao) - 1] = '\0';
    printf("Introduzir Cidade:");
    fgets(dadosTemp.Cidade, 32, stdin);
    if ((strlen(dadosTemp.Cidade) > 0) && (dadosTemp.Cidade[strlen (dadosTemp.Cidade) - 1] == '\n'))
                dadosTemp.Cidade[strlen (dadosTemp.Cidade) - 1] = '\0';
    printf("Introduzir Codigo Postal:");
    fgets(dadosTemp.Postal, 32, stdin);
    if ((strlen(dadosTemp.Postal) > 0) && (dadosTemp.Postal[strlen (dadosTemp.Postal) - 1] == '\n'))
                dadosTemp.Postal[strlen (dadosTemp.Postal) - 1] = '\0';
    printf("Introduzir Morada:");
    fgets(dadosTemp.Morada, 256, stdin);
    if ((strlen(dadosTemp.Morada) > 0) && (dadosTemp.Morada[strlen (dadosTemp.Morada) - 1] == '\n'))
                dadosTemp.Morada[strlen (dadosTemp.Morada) - 1] = '\0';

    return dadosTemp;
}
int main(){
    /* Menu de teste das funcionalidades
     * IMPORTANTE: Este menu apresenta todas as informações, independemente do Login. Um menu mais
     * seguro deverá ser repensado
     */

    char opc;
    char *nome = malloc(32);

    char *pass = malloc(32);

    char *novaPass = malloc(32);

    info usr1 = {

    .Pais = "Portugal",
    .Regiao = "Regiao",
    .Cidade = "Cidade",
    .Postal = "4442-324",
    .Morada = "Rua aleatoria, 69, Pais de teste, Marte"

    };
    
    autoLoc("Memes", "Memes");
    do{
        do{
            printf(
                    "A- Ver se existe usr\n"
                    "B- Criar Conta\n"
                    "C- Login\n"
                    "D- Apagar Conta\n"
                    "E- Testar funcionalidades de localização\n"
                    "F- Mudar uma palavra-passe\n"
                    "Q- Sair\n");

            scanf(" %c", &opc);
            
        }while(opc != 'A' && opc != 'B' && opc != 'C' && opc != 'D' && opc != 'Q' && opc != 'E' && opc != 'F');
        switch(opc){
            case 'A':

                printf("Meter nome...\n");
                scanf("%s", nome);
                if(usrExiste(nome))
                    printf("O usr existe\n");
                
                else
                    printf("O usr não existe\n");
                
                break;
            case 'B':
                printf("Meter nome...\n");

                scanf("%s", nome);
                printf("Meter pass...\n");

                scanf("%s", pass);
                if(manLoc(nome, pass, introduzirInfo()))
                    printf("Conta criada com usr %s e pass %s\n", nome, pass);
                else
                    printf("Conta já existe\n");
                break;
            case 'C':
                printf("Meter nome...\n");
                scanf("%s", nome);

                printf("Meter pass...\n");
                scanf("%s", pass);
                if(login(nome, pass) == 1)
                    printf("Login bem-sucedido!\n");
                else if(login(nome, pass) == 0)
                    printf("Palavra-chave Errada\n");
                else if(login(nome, pass) == -1)
                    printf("Erro a ler password\n");
                else if(login(nome, pass) == -2)
                    printf("Erro a ler ficheiro\n");
                else if(login(nome, pass) == -3)
                    printf("Utilizador não existe\n");
                break;
            case 'D':
                printf("Meter nome\n");
                scanf("%s", nome);

                printf("Meter pass\n");
                scanf("%s", pass);

                if(apagarConta(nome, pass) == 1){
                    printf("Apagou conta do utilizador %s\n", nome);
                }
                else if(apagarConta(nome, pass) == -1){
                    printf("Pass Errada!\n");
                }
                else{
                    printf("Erro!\n");
                }
                break;

            case 'E' :
                printf("Meter nome\n");
                scanf("%s", nome);

                printf("%s", lerLoc(nome, 'P'));
                printf("%s", lerLoc(nome, 'R'));
                printf("%s", lerLoc(nome, 'C'));
                printf("%s", lerLoc(nome, 'O'));
                printf("%s\n", lerLoc(nome, 'M'));
                break;

            case 'F':
                printf("Meter nome\n");
                scanf("%s", nome);

                printf("Meter pass\n");
                scanf("%s", pass);

                if(login(nome, pass)){
                    printf("Meter nova pass\n");
                    scanf("%s", novaPass);
                    mudarPass(nome, pass, novaPass);
                }
                else{
                    printf("Login errado!\n");
                }
                break;
        }
    }while(opc != 'Q');
    return 0;
}
