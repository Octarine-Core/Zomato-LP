Sisutil, é um sistema de utilizadores baseado em ficheiros de texto para o
programa "Zomato", apesar de ter um menu básico, as funcões podem ser utilizada
por outros programas. 

Alguns potenciais problemas: 

    -> Se uma conta for criada automaticamente sem acesso à internet (ou à API),
 vai ser criado um ficheiro de texto com a pass, mas sem informações, apenas
 a dizer "Failure".

    -> Não é possível ter nomes de utilizador com caracters não aceites para nome
de ficheiro, ou com o nome 'TEMP'.

    -> O Menu é apenas de teste, algumas partes devem ser reconsideradas com mais
cuidado.

    -> Este sistema não foi desenhado com segurança em mente, os ficheiros de 
texto estão simplesment num diretorio, e qualquer um com acesso ao diretorio
consegue ver as passes dos utilizadores.

    -> O sistema não foi testado extensivamente, algum uso de scanf() pode dar
erros, ou podem occorrer buffer overflows.

O sistema usa ficheiros .txt para guardar informações acerca dos utilizadores,
tanto podem ser introduzidas manualmente, como passadas por uma struct, ou
usando as informações do IP, utilizando a API.

O sistema suporta também a leitura de dados, login (usando o nome e palavra-
passe, e falhando quando um desses está errado), mudança de palavra-pass e
apagar contas.

Exemplo de criação manual de conta (completamente manual):

//code
manLoc("Nome_de_Utilizador", "Palavra-passe", introduzirInfo());
//uncode

Exemplo de crianção semi-manual (com estrutura):

//code
info dadosAntonio; //Dados teriam de ser atribuidos aos componentes de
dadosAntonio

manLoc("Antonio", "toni69", dadosAntonio);
//uncode


http://www.ip-api.com/docs para ver a API usada para obter os dados através
do IP.
