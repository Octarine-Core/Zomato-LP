/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Zomato.c
 * Author: Rebeca Teixeira
 *
 * Created on 3 de Outubro de 2017, 12:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <curl/curl.h>
#include "LP_Leitura.h"
#include "sisutil.h"


/* 
 * Para utilizar a libcurl é necessário instalar as bibliotecas Net->curl, Libs->libcurl-devel e Libs->libcurl4, a partir do cygwin.
 * 
 * É ainda necessário adicionar a opção -lcurl nas "Additional Options" do Linker,
 * nas propriedades do projeto
 */

/*
 * Estrutura para guardar o texto recebido
 */
struct MemoryStruct {
    char *memory;
    size_t size;
};

/*
 * Utilizado para guardar o resultado de obter a página pedida
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("Não existe memória suficiente (realloc devolveu NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void menu_pesquisa() {
    printf("\t MENÚ DE PESQUISA \t\n");
    printf("1 - Pesquisa pelo nome\n");
    printf("2 - Pesquisa pelo tipo de cozinha\n");
    printf("3 - Pesquisa pela cidade\n");
    printf("0 - Voltar ao menu inicial\n");
}

void dados(char *url) {

    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    //menu_pesquisa();

    chunk.memory = malloc(1); /* apontador onde se guardarão os dados devolvidos pela página. Será aumentado à medida que são obtidos os dados */
    chunk.size = 0;

    /* inicializar a sessão curl */
    curl_handle = curl_easy_init();

    /* 
     acrescentar headers necessários
     TODO: é necessário especificar a chave
     */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "user-key: 1d3ee42fd4b1c25df099e532e8f8ae69");
    headers = curl_slist_append(headers, "Accept: application/json");

    res = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* especificar a função para onde se pretende enviar os dados recebidos  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* passar a estrutura onde serão guardados so dados (chunk) */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* executar o curl */
    res = curl_easy_perform(curl_handle);

    /* verificar ocorrência de erros */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() falhou: %s\n",
                curl_easy_strerror(res));
    }

    /**
     * 
     * Tratar a resposta recebida de forma a imprimir o nome de cada restaurante devolvido
     */

    char *token = strstr(chunk.memory, "address");

    char *toke = strstr(chunk.memory, "name");

    char *tok = strstr(chunk.memory, "average_cost_for_two");

    char unicode[6];
    //system("clear");
    while (token != NULL) {


        //token para o nome
        toke += 7;
        int i = 0;
        while (toke[i] != '\0' && toke[i] != '\"')
            i++;
        char* name = (char*) malloc(sizeof (char) * i + 1);
        memcpy(name, toke, i);
/*
        i=0;
        while(name[i] != '\0')
        {
            if(name[i] == "\'")
            {
                i=0;
                for(i=0;i<6;i++)
                {
                    strcpy(unicode[i],name[i]);
                    printf("Unicode: %s", unicode);
                }
            }
        }
 * */

        printf("Nome: %s\n", name);

        char* novoToke = strstr(toke, "name");


        if (novoToke == toke) {
            free(toke);
            toke = NULL;
        } else toke = novoToke;


        //token para a morada
        token += 10;
        i = 0;
        while (token[i] != '\0' && token[i] != '\"')
            i++;
        char* address = (char*) malloc(sizeof (char) * i + 1);
        memcpy(address, token, i);

        printf("Morada: %s\n", address);

        char* novoToken = strstr(token, "address");


        if (novoToken == token) {
            free(token);
            token = NULL;
        } else token = novoToken;


        //token para o preço médio
        tok += 22;
        i = 0;
        while (tok[i] != '\0' && tok[i] != ',')
            i++;
        char * price = (char *) malloc(sizeof (char) * i + 1);
        price[i] = '\0';
        memcpy(price, tok, i);
        printf("Preço médio: %s\n", price);
        char * novoTok = strstr(tok, "average_cost_for_two");

        if (novoTok == tok) {
            free(tok);
            tok == NULL;
        } else tok = novoTok;

        printf("\n");
    }

    /* limpar */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

}

/*
void feed(char localização[100]) {
    //Necessário utilizar a função location para obter entity_id e entity_type
    //para usar na location_details

    CURL *curl_handle;
    CURLcode res;

    //    char *id;
    //  char id1[100];
    char url[] = "https://developers.zomato.com/api/v2.1/location_details?entity_id=311&entity_type=city";

    struct MemoryStruct chunk;

    chunk.memory = malloc(1); /* apontador onde se guardarão os dados devolvidos pela página. Será aumentado à medida que são obtidos os dados 
    chunk.size = 0;

    /* inicializar a sessão curl 
    curl_handle = curl_easy_init();

    /* 
     acrescentar headers necessários
     TODO: é necessário especificar a chave
     
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "user-key: 1d3ee42fd4b1c25df099e532e8f8ae69");
    headers = curl_slist_append(headers, "Accept: application/json");

    res = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* especificar a função para onde se pretende enviar os dados recebidos  
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* passar a estrutura onde serão guardados so dados (chunk) 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* executar o curl 
    res = curl_easy_perform(curl_handle);

    /* verificar ocorrência de erros 
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() falhou: %s\n",
                curl_easy_strerror(res));
    } else {
        printf("%ld bytes recebidos\n", (long) chunk.size);

        //imprimir a resposta: 
        //printf("Resposta recebida: %s", chunk.memory);
    }

    /**
 * 
 * Tratar a resposta recebida de forma a imprimir o nome de cada restaurante devolvido
     

    char *token = strstr(chunk.memory, "name");

    while (token != NULL) {
        token += 7;
        int i = 0;
        while (token[i] != '\0' && token[i] != '\"')
            i++;
        char* name = (char*) malloc(sizeof (char) * i + 1);
        memcpy(name, token, i);

        printf(" %s\n", name);

        char* novoToken = strstr(token, "name");


        if (novoToken == token) {
            free(token);
            token = NULL;
        } else token = novoToken;
    }

    /* limpar 
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

}
 */

void obter_entitys(char localidade[200], char * x_ent_type, char * y_ent_id) {

    CURL *curl_handle;
    CURLcode res;

    char *id;
    char url[] = "https://developers.zomato.com/api/v2.1/locations?query=";

    struct MemoryStruct chunk;

    //menu_pesquisa();

    chunk.memory = malloc(1); /* apontador onde se guardarão os dados devolvidos pela página. Será aumentado à medida que são obtidos os dados */
    chunk.size = 0;

    /* inicializar a sessão curl */
    curl_handle = curl_easy_init();

    /* 
     acrescentar headers necessários
     TODO: é necessário especificar a chave
     */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "user-key: 1d3ee42fd4b1c25df099e532e8f8ae69");
    headers = curl_slist_append(headers, "Accept: application/json");

    res = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    //Lê a string, validando o seu tamanho e concatenata a string introduzida
    //no fim do url
    strcat(url, localidade);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* especificar a função para onde se pretende enviar os dados recebidos  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* passar a estrutura onde serão guardados so dados (chunk) */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* executar o curl */
    res = curl_easy_perform(curl_handle);

    /* verificar ocorrência de erros */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() falhou: %s\n",
                curl_easy_strerror(res));
    }

    /**
     * 
     * Tratar a resposta recebida de forma a imprimir o nome de cada restaurante devolvido
     */

    char *token = strstr(chunk.memory, "entity_type");

    char *toke = strstr(chunk.memory, "entity_id");

    //int n_letras_id = strlen("entity_id") + 3;

    while (token != NULL) {
        token += 14;
        int i = 0;
        while (token[i] != '\0' && token[i] != '\"')
            i++;

        char* entity_type = (char*) malloc(sizeof (char) * i + 1);
        entity_type[i] = '\0';

        memcpy(entity_type, token, i);

        strcpy(x_ent_type, entity_type);

        char* novoToken = strstr(token, "entity_type");

        if (novoToken == token) {
            free(token);
            token = NULL;
        } else token = novoToken;

        toke += 11;
        i = 0;
        while (toke[i] != '\0' && toke[i] != '\"' && toke[i] != ',')
            i++;

        char* entity_id = (char*) malloc(sizeof (char) * i + 1);
        entity_id[i] = '\0';

        memcpy(entity_id, toke, i);

        strcpy(y_ent_id, entity_id);

        char* novoToke = strstr(toke, "entity_id");

        if (novoToke == toke) {
            free(toke);
            toke = NULL;
        } else toke = novoToke;

    }
}

void pesquisa_nome(char nome[200]) {

    //setlocale(LC_ALL, "Portuguese");

    char url[] = "https://developers.zomato.com/api/v2.1/search?q=";
    strcat(url, nome);
    dados(url);
}

void menu() {

    printf("1 - Pesquisar\n");
    printf("2 - Gestão de Utilizadores\n");
    printf("0 - Sair\n");
}

void obter_city_id(char localidade[200], char * city_id1) {

    CURL *curl_handle;
    CURLcode res;

    char url[] = "https://developers.zomato.com/api/v2.1/locations?query=";

    struct MemoryStruct chunk;

    chunk.memory = malloc(1); /* apontador onde se guardarão os dados devolvidos pela página. Será aumentado à medida que são obtidos os dados */
    chunk.size = 0;

    /* inicializar a sessão curl */
    curl_handle = curl_easy_init();

    strcat(url, localidade);

    /* 
     acrescentar headers necessários
     TODO: é necessário especificar a chave
     */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "user-key: 1d3ee42fd4b1c25df099e532e8f8ae69");
    headers = curl_slist_append(headers, "Accept: application/json");

    res = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    //Lê a string, validando o seu tamanho e concatenata a string introduzida
    //no fim do url

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* especificar a função para onde se pretende enviar os dados recebidos  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* passar a estrutura onde serão guardados so dados (chunk) */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* executar o curl */
    res = curl_easy_perform(curl_handle);

    /* verificar ocorrência de erros */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() falhou: %s\n",
                curl_easy_strerror(res));
    }

    char *token = strstr(chunk.memory, "city_id");

    while (token != NULL) {
        token += 9;
        int i = 0;
        while (token[i] != '\0' && token[i] != '\"' && token[i] != ',')
            i++;

        char* city_id = (char*) malloc(sizeof (char) * i + 1);
        city_id[i] = '\0';

        memcpy(city_id, token, i);

        strcpy(city_id1, city_id);

        char* novoToken = strstr(token, "city_id");

        if (novoToken == token) {
            free(token);
            token = NULL;
        } else token = novoToken;

    }

    //LIMPAR A CHUNK
}

void obter_cuisine_id(char * city_id, char * cozinha_id) {

    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    char url[] = "https://developers.zomato.com/api/v2.1/cuisines?city_id=";
    int j = 0;

    chunk.memory = malloc(1); /* apontador onde se guardarão os dados devolvidos pela página. Será aumentado à medida que são obtidos os dados */
    chunk.size = 0;

    /* inicializar a sessão curl */
    curl_handle = curl_easy_init();

    strcat(url, city_id);

    /* 
     acrescentar headers necessários
     TODO: é necessário especificar a chave
     */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "user-key: 1d3ee42fd4b1c25df099e532e8f8ae69");
    headers = curl_slist_append(headers, "Accept: application/json");

    res = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    //Lê a string, validando o seu tamanho e concatenata a string introduzida
    //no fim do url

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* especificar a função para onde se pretende enviar os dados recebidos  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* passar a estrutura onde serão guardados so dados (chunk) */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* executar o curl */
    res = curl_easy_perform(curl_handle);

    /* verificar ocorrência de erros */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() falhou: %s\n",
                curl_easy_strerror(res));
    }

    char *token = strstr(chunk.memory, "cuisine_name");
    char * toke = strstr(chunk.memory, "cuisine_id");

    while (token != NULL) {
        token += 15;
        int i = 0;
        while (token[i] != '\0' && token[i] != '\"')
            i++;

        char* cuisine_name = (char*) malloc(sizeof (char) * i + 1);
        cuisine_name[i] = '\0';

        memcpy(cuisine_name, token, i);

        char* novoToken = strstr(token, "cuisine_name");

        if (novoToken == token) {
            free(token);
            token = NULL;
        } else token = novoToken;


        toke += 12;
        i = 0;
        while (toke[i] != '\0' && toke[i] != '\"' && toke[i] != ',')
            i++;

        char* cuisine_id = (char*) malloc(sizeof (char) * i + 1);
        cuisine_id[i] = '\0';

        memcpy(cuisine_id, toke, i);

        char* novoToke = strstr(toke, "cuisine_id");

        if (novoToke == toke) {
            free(toke);
            toke = NULL;
        } else toke = novoToke;

        printf("%s: %s\n", cuisine_id, cuisine_name);


    }
    readString(cozinha_id, sizeof (cozinha_id), "Introduza o ID da cozinha que "
            "pretende: ");

}

void pesquisa_local(char entity_id[20], char entity_type[20]) {

    setlocale(LC_ALL, "");
    
    char url[] = "https://developers.zomato.com/api/v2.1/search?entity_id=";
    strcat(url, entity_id);
    strcat(url, "&entity_type=");
    strcat(url, entity_type);
    dados(url);
}

void pesquisa_tipo_cozinha(char entity_id[20], char entity_type[20], char local[200], char cuisine_id[20]) {

    char url[] = "https://developers.zomato.com/api/v2.1/search?entity_id=";

    strcat(url, entity_id);

    strcat(url, "&entity_type=");

    strcat(url, entity_type);

    strcat(url, "&q=");

    strcat(url, local);

    strcat(url, "&cuisines=");

    strcat(url, cuisine_id);

    dados(url);
}

int main(void) {
    int opcao, opcao_pesquisa;
    char nome[200], localizacao[100], entity_type[20], entity_id[20],
            city_id[20], cozinha[25];
    do {
        do {
            //feed();
            menu();
            readInt(&opcao, 0, 2, "Introduza a opção que pretende: ");
        } while (opcao > 2 || opcao < 0);

        switch (opcao) {
            case 1:
                do {
                    do {
                        menu_pesquisa();
                        readInt(&opcao_pesquisa, 0, 3, "Introduza a opção de pesquisa que pretende: ");
                    } while (opcao_pesquisa > 3 || opcao_pesquisa < 0);
                    switch (opcao_pesquisa) {

                        case 1:
                            //Pesquisa pelo nome
                            system("clear");
                            readString(nome, sizeof (nome), "Nome: ");
                            pesquisa_nome(nome);
                            break;

                        case 2:
                            system("clear");
                            readString(localizacao, sizeof (localizacao), "Local: ");
                            obter_city_id(localizacao, city_id);
                            //printf("City_id: %s\n", city_id);
                            obter_cuisine_id(city_id, cozinha);
                            //printf("Cuisine_id: %s", cozinha);
                            obter_entitys(localizacao, entity_type, entity_id);
                            pesquisa_tipo_cozinha(entity_id, entity_type, localizacao, cozinha);
                            break;

                        case 3:
                            /*Pesquisa pelo tipo de cozinha. A localização é
                            obrigatória para a função, logo será pedido ao user se quer a localizaçáo
automática ou manual.*/
                            system("clear");
                            readString(localizacao, sizeof (localizacao), "Local: ");
                            obter_entitys(localizacao, entity_type, entity_id);
                            pesquisa_local(entity_id, entity_type);
                            break;
                    }
                    
                } while (opcao_pesquisa != 0);
                    break;
            // sistema de utilizadores
            case 2:
                    //Se o utilizador fez login bem-sucedido, e tem a cidade
                strcpy(localizacao, usrMenu());
                strtok(localizacao, "\n"); //remove o newline
                if(strcmp(localizacao, "0") != 0){
                    printf("À procura de restaurantes em %s... \n", localizacao);
                    obter_entitys(localizacao, entity_type, entity_id);
                    pesquisa_local(entity_id, entity_type);
                }
                break;
                
        }
    } while (opcao != 0);
    return 0;
}