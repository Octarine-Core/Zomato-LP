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

#define MSG_NO_RESULTS "Não foram encontrados resultados.\n"

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
    printf("\n* * * * *   MENÚ DE PESQUISA  * * * * * * *\n");
    printf("*                                         *\n");
    printf("*      1 - Pesquisa pelo nome             *\n");
    printf("*      2 - Pesquisa pelo tipo de cozinha  *\n");
    printf("*      3 - Pesquisa pela cidade           *\n");
    printf("*      0 - Voltar ao menu inicial         *\n");
    printf("*                                         *\n");
    printf("* * * * * * * * * * * * * * * * * * * * * *\n");
}

int comparacao_ASCII(char code[5]) {
    int caracter;


    /*Caracteres sem acentuação, pois o char é apenas um byte logo 
     * os caracteres ASCII com o código até 128 dão
     * No entanto os caracteres do 128-255
     * têm dois bytes e o char não assume*/

    if (strcmp(code, "u00e1") == 0)
        //caracter = "á";
        caracter = 97;
    else if (strcmp(code, "u00e0") == 0)
        //caracter = "à";
        caracter = 97;
    else if (strcmp(code, "u00e2") == 0)
        //caracter = "â";
        caracter = 97;
    else if (strcmp(code, "u00e3") == 0)
        // caracter = "ã";
        caracter = 97;
    else if (strcmp(code, "u00e4") == 0)
        caracter = 97;
        //caracter = "ä";
    else if (strcmp(code, "u00c1") == 0)
        caracter = 65;
        //caracter = "Á";
    else if (strcmp(code, "u00c0") == 0)
        caracter = 65;
        //caracter = "À";
    else if (strcmp(code, "u00c2") == 0)
        //caracter = "Â";
        caracter = 65;
    else if (strcmp(code, "u00c3") == 0)
        //caracter = "Ã";
        caracter = 65;
    else if (strcmp(code, "u00c4") == 0)
        //caracter = "Ä";
        caracter = 65;
    else if (strcmp(code, "u00e9") == 0)
        //caracter = "é";
        caracter = 101;
    else if (strcmp(code, "u00e8") == 0)
        //caracter = "è";
        caracter = 101;
    else if (strcmp(code, "u00ea") == 0)
        //caracter = "ê";
        caracter = 101;
    else if (strcmp(code, "u00c9") == 0)
        //caracter = "É";
        caracter = 69;
    else if (strcmp(code, "u00c8") == 0)
        //caracter = "È";
        caracter = 69;
    else if (strcmp(code, "u00ca") == 0)
        //caracter = "Ê";
        caracter = 69;
    else if (strcmp(code, "u00cb") == 0)
        //caracter = "";
        caracter = 69;
    else if (strcmp(code, "u00ed") == 0)
        //caracter = "í";
        caracter = 105;
    else if (strcmp(code, "u00ec") == 0)
        //caracter = "ì";
        caracter = 105;
    else if (strcmp(code, "u00ee") == 0)
        //caracter = "î";
        caracter = 105;
    else if (strcmp(code, "u00ef") == 0)
        //caracter = "ï";
        caracter = 105;
    else if (strcmp(code, "u00cd") == 0)
        //caracter = "Í";
        caracter = 73;
    else if (strcmp(code, "u00cc") == 0)
        //caracter = "Ì";
        caracter = 73;
    else if (strcmp(code, "u00ce") == 0)
        //caracter = "Î";
        caracter = 73;
    else if (strcmp(code, "u00cf") == 0)
        //caracter = "Ï";
        caracter = 73;
    else if (strcmp(code, "u00f3") == 0)
        //caracter = "ó";
        caracter = 111;
    else if (strcmp(code, "u00f2") == 0)
        //caracter = "ò";
        caracter = 111;
    else if (strcmp(code, "u00f4") == 0)
        //caracter = "ô";
        caracter = 111;
    else if (strcmp(code, "u00f5") == 0)
        //caracter = "õ";
        caracter = 111;
    else if (strcmp(code, "u00f6") == 0)
        //caracter = "ö";
        caracter = 111;
    else if (strcmp(code, "u00d3") == 0)
        //caracter = "Ó";
        caracter = 79;
    else if (strcmp(code, "u00d2") == 0)
        //caracter = "Ò";
        caracter = 79;
    else if (strcmp(code, "u00d4") == 0)
        //caracter = "Ô";
        caracter = 79;
    else if (strcmp(code, "u00d5") == 0)
        //caracter = "Õ";
        caracter = 79;
    else if (strcmp(code, "u00d6") == 0)
        //caracter = "Ö";
        caracter = 79;
    else if (strcmp(code, "u00fa") == 0)
        //caracter = "ú";
        caracter = 117;
    else if (strcmp(code, "u00f9") == 0)
        //caracter = "ù";
        caracter = 117;
    else if (strcmp(code, "u00fb") == 0)
        //caracter = "û";
        caracter = 117;
    else if (strcmp(code, "u00fc") == 0)
        //caracter = "ü";
        caracter = 117;
    else if (strcmp(code, "u00da") == 0)
        //caracter = "Ú";
        caracter = 85;
    else if (strcmp(code, "u00d9") == 0)
        //caracter = "Ù";
        caracter = 85;
    else if (strcmp(code, "u00db") == 0)
        //caracter = "Û";
        caracter = 85;
    if (strcmp(code, "u00e7") == 0)
        //caracter = 347;
        caracter = 99;
        //caracter = "ç";
    else if (strcmp(code, "u00c7") == 0)
        //caracter = "Ç";
        caracter = 67;
    else if (strcmp(code, "u00f1") == 0)
        //caracter = "ñ";
        caracter = 110;
    else if (strcmp(code, "u00d1") == 0)
        //caracter = "Ñ";
        caracter = 78;

    return caracter;
}

void unicode(char * array) {
    setlocale(LC_ALL, "Portuguese");
    int i = 0, encontrado = 0, res = -1, j;
    char code[5];
    int caracter, exit = 0;
    int tamanho = strlen(array);
    while (array[i] != '\0' && encontrado == 0 && exit == 0) {
        i = 0;
        while (encontrado != 1) {
            if (array[i] == 92) {
                encontrado = 1;
                res = i;
            }
            i++;
        }
        if (res > tamanho) {
            exit = 1;
            array [tamanho] = '\0';
        } else {
            j = 0;
            while (encontrado == 1) {
                if (res != -1) {
                    for (i = res + 1; i < res + 6; i++) {
                        code[j] = array[i];
                        j++;
                    }
                    code[j] = '\0';
                    caracter = comparacao_ASCII(code);
                    array[res] = caracter;
                    i = res;
                    while (array[i] != '\0') {
                        array[i + 1] = array[i + 6];
                        i++;
                    }
                    encontrado = 0;
                }
            }
        }
        i++;
    }
}

int dados(char *url) {
    //printf("\e[1;1H\e[2J");
    system("clear");
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;


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

    char *to = strstr(chunk.memory, "aggregate_rating");

    char *t = strstr(chunk.memory, "rating_text");

    int pos = 0, success = 1, j = 0;


    //system("clear");
    while (token != NULL) {

        j++;

        //token para o nome
        toke += 7;
        int i = 0;
        while (toke[i] != '\0' && toke[i] != '\"')
            i++;
        char* name = (char*) malloc(sizeof (char) * i + 1);

        name[i] = '\0';
        memcpy(name, toke, i);
        unicode(name);
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
        address[i] = '\0';
        memcpy(address, token, i);

        unicode(address);
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

        char * novoTok = strstr(tok, "average_cost_for_two");

        if (novoTok == tok) {
            free(tok);
            tok == NULL;
        } else tok = novoTok;

        printf("Preço médio: %s EUR\n", price);

        //token para o rating
        to += 19;
        i = 0;
        while (to[i] != '\0' && to[i] != '\"' && to[i] != ',')
            i++;
        char* rating = (char*) malloc(sizeof (char) * i + 1);
        memcpy(rating, to, i);
        rating[i] = '\0';

        char* novoTo = strstr(to, "aggregate_rating");

        if (novoTo == to) {
            free(to);
            to = NULL;
        } else to = novoTo;

        //token para o rating text
        t += 14;
        i = 0;
        while (t[i] != '\0' && t[i] != '\"' && t[i] != ',')
            i++;
        char* rating_text = (char*) malloc(sizeof (char) * i + 1);
        memcpy(rating_text, t, i);
        rating_text[i] = '\0';

        char* novoT = strstr(t, "rating_text");

        if (novoT == t) {
            free(t);
            t = NULL;
        } else t = novoT;

        printf("Rating: %s(%s)\n", rating, rating_text);

        printf("\n");

    }
    if (j == 0) {
        printf(MSG_NO_RESULTS);
    }

    return j;

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();


}

int obter_entitys(char localidade[200], char * x_ent_type, char * y_ent_id) {

    CURL *curl_handle;
    CURLcode res;

    char url[] = "https://developers.zomato.com/api/v2.1/locations?query=";

    struct MemoryStruct chunk;

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

    char *tok = strstr(chunk.memory, "title");

    char res1[5];

    while (token != NULL) {
        tok += 8;
        int i = 0;
        while (tok[i] != '\0' && tok[i] != '\"')
            i++;

        char* city_name = (char*) malloc(sizeof (char) * i + 1);
        city_name[i] = '\0';
        memcpy(city_name, tok, i);
        unicode(city_name);



        char* novoTok = strstr(tok, "title");

        if (novoTok == tok) {
            free(tok);
            tok = NULL;
        } else tok = novoTok;

        do {
            printf("A zona detetada foi: %s\nDeseja utilizar a zona detetada?\n\t"
                    "Y - Sim\n\tN - Não\n", city_name);
            readString(res1, sizeof (res1), "Resposta: ");

            //Validação da resposta do utilizador
            if (strcmp(res1, "Y") == 0 || strcmp(res1, "y") == 0 || strcmp(res1, "Yes") == 0 || strcmp(res1, "yes") == 0 || strcmp(res1, "Sim") == 0 || strcmp(res1, "sim") == 0 || strcmp(res1, "Oui") == 0 || strcmp(res1, "oui") == 0 || strcmp(res1, "Si") == 0 || strcmp(res1, "si") == 0 || strcmp(res1, "S") == 0 || strcmp(res1, "s") == 0) {
                token += 14;
                i = 0;
                while (token[i] != '\0' && token[i] != '\"')
                    i++;

                char* entity_type = (char*) malloc(sizeof (char) * i + 1);
                entity_type[i] = '\0';

                memcpy(entity_type, token, i);

                strncpy(x_ent_type, entity_type, sizeof (x_ent_type));

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

                strncpy(y_ent_id, entity_id, sizeof (y_ent_id));

                char* novoToke = strstr(toke, "entity_id");

                if (novoToke == toke) {
                    free(toke);
                    toke = NULL;
                } else toke = novoToke;

                return 1;
            } else if (strcmp(res1, "N") == 0 || strcmp(res1, "n") == 0 || strcmp(res1, "No") == 0 || strcmp(res1, "no") == 0 || strcmp(res1, "Nao") == 0 || strcmp(res1, "nao") == 0 || strcmp(res1, "Ne Pas") == 0 || strcmp(res1, "ne pas") == 0 || strcmp(res1, "Ne pas") == 0) {
                return 0;
            } else
                printf("Resposta considerada inválida.\n");
        } while (strcmp(res1, "N") != 0 || strcmp(res1, "n") != 0 || strcmp(res1, "No") != 0 || strcmp(res1, "no") != 0 || strcmp(res1, "Nao") != 0 || strcmp(res1, "nao") != 0 || strcmp(res1, "Ne Pas") != 0 || strcmp(res1, "ne pas") != 0 || strcmp(res1, "Ne pas") != 0 || strcmp(res1, "Y") != 0 || strcmp(res1, "y") != 0 || strcmp(res1, "Yes") != 0 || strcmp(res1, "yes") != 0 || strcmp(res1, "Sim") != 0 || strcmp(res1, "sim") != 0 || strcmp(res1, "Oui") != 0 || strcmp(res1, "oui") != 0 || strcmp(res1, "Si") != 0 || strcmp(res1, "si") != 0 || strcmp(res1, "S") != 0 || strcmp(res1, "s") != 0);
    }

    /* limpar */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();
}

void pesquisa_nome(char nome[200]) {
    char url[200] = "https://developers.zomato.com/api/v2.1/search?q=";
    //printf("URL: %s\n", url);
    strcat(url, nome);
    //system("clear");
    dados(url);
}

void menu() {

    printf("\n* * * * *  MENÚ PRINCIPAL * * * * * *\n");
    printf("*                                   *\n");
    printf("*      1 - Pesquisar (convidado)    *\n");
    printf("*      2 - Gestão de Utilizadores   *\n");
    printf("*      0 - Sair                     *\n");
    printf("*                                   *\n");
    printf("* * * * * * * * * * * * * * * * * * *\n");
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

    /* limpar */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();
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

        printf("%s: %s\t\t", cuisine_id, cuisine_name);

        j++;
        if (j % 3 == 0)
            printf("\n");

    }

    /* limpar */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    readString(cozinha_id, sizeof (cozinha_id), "Introduza o ID da cozinha que "
            "pretende: ");

}

void pesquisa_local(char entity_id[20], char entity_type[20]) {
    setlocale(LC_ALL, "");

    char url[200] = "https://developers.zomato.com/api/v2.1/search?entity_id=";
    strcat(url, entity_id);
    strcat(url, "&entity_type=");
    strcat(url, entity_type);
    //system("clear");
    dados(url);
}

void pesquisa_tipo_cozinha(char entity_id[20], char entity_type[20], char local[200], char cuisine_id[20]) {

    char url[200] = "https://developers.zomato.com/api/v2.1/search?entity_id=";
    strcat(url, entity_id);
    strcat(url, "&entity_type=");
    strcat(url, entity_type);
    strcat(url, "&q=");
    strcat(url, local);
    strcat(url, "&cuisines=");
    strcat(url, cuisine_id);
    //system("clear");
    dados(url);
}

int main(void) {
    int opcao, opcao_pesquisa, res = 0;
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
                        printf("\n");
                        menu_pesquisa();
                        readInt(&opcao_pesquisa, 0, 4, "Introduza a opção de pesquisa que pretende: ");
                    } while (opcao_pesquisa > 4 || opcao_pesquisa < 0);
                    switch (opcao_pesquisa) {

                        case 1:
                            //Pesquisa pelo nome
                            system("clear");
                            printf("\t\t Pesquisa pelo Nome do Restaurante\n");
                            readString(nome, sizeof (nome), "Nome: ");
                            pesquisa_nome(nome);
                            char url[200] = "https://developers.zomato.com/api/v2.1/search?q=";
                            strcat(url, nome);
                            break;

                        case 2:
                            /*Pesquisa pelo tipo de cozinha. A localização é
                            obrigatória para a função, logo será pedido ao user se quer a localizaçáo
automática ou manual.*/
                            system("clear");
                            printf("\t\t Pesquisa pelo Tipo de Cozinha\n");
                            readString(localizacao, sizeof (localizacao), "Localização: ");
                            res = obter_entitys(localizacao, entity_type, entity_id);
                            if (res == 1) {
                                obter_city_id(localizacao, city_id);
                                obter_cuisine_id(city_id, cozinha);
                                pesquisa_tipo_cozinha(entity_id, entity_type, localizacao, cozinha);
                            }
                            break;

                        case 3:
                            /*Para poder fazer a pesquisa pela localização
                             é necessário obter os entitys (entity_id e entity_type)
                             para isso é fundamental ir obtê-los a outra função
                             designada por "location" fornecida pela API*/
                            system("clear");
                            printf("\t\t Pesquisa pela Localização\n");
                            readString(localizacao, sizeof (localizacao), "Localização: ");
                            res = obter_entitys(localizacao, entity_type, entity_id);
                            if (res == 1)
                                pesquisa_local(entity_id, entity_type);
                            break;

                    }
                } while (opcao_pesquisa != 0);
                break;
            case 2:
                system("clear");
                strcpy(localizacao, usrMenu());
                strtok(localizacao, "\n"); //remove o newline
                if (strcmp(localizacao, "0") != 0) {
                    printf("À procura de restaurantes em %s... \n", localizacao);
                    obter_entitys(localizacao, entity_type, entity_id);
                    pesquisa_local(entity_id, entity_type);
                }
        }
    } while (opcao != 0);
    return 0;
}