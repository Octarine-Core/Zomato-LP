#ifndef SISUTIL_H
# define SISUTIL_H

typedef struct infoTag{

    char Pais[32];
    char Regiao[32];
    char Cidade[32];
    char Postal[32];
    char Morada[256];

}info;


int usrExiste(char* nome);

char* getPath(char* nome);

int login(char* nome, char* pass);

int autoLoc(char* nome, const char * pass);

int manLoc(char* nome, char* pass, info dados);

int apagarConta(char* nome, char* pass);

char* lerLoc(char* nome, char flag);

info introduzirInfo();

int mudarPass(char* nome, char* pass, char* novaPass);

char* usrMenu();

#endif
