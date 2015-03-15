#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <limits.h>
#include "defines.h"

typedef struct Node {

	int id;					//id do no
	char ip[IPSIZE];		//endereco do no - pode ser em nome ou em numero
	char port[PORTSIZE];	//porto do no
	int fd;					//descritor de comunicacao do no
} Node;

extern Node curNode;
extern int curRing;
extern Node prediNode;
extern Node succiNode;
extern char startServerIp[IPSIZE];
extern char startServerPort[PORTSIZE];
extern int startServerFd;

/*****************
 * Inicialização *
 *****************/

/*
	descricao:	interpreta os argumentos de entrada lendo os valores do porto tcp
				usado nas comunicacoes do anel, o ip do servidor de arranque e o porto
				do servidor de arranque. cria o socket de escuta do no para novas ligacoes
				os dados lidos sao armazenados nas variaveis correspondentes:
					curNode.port - porto TCP
					startServerIP - endereco IP do servidor de arranque
					startServerPort - porto do servidor de arranque

	returns:	0 em caso de sucesso e um valor menor do que zero em caso de erro
	erros:		EARGS - os argumentos de entrada nao têm o formato correcto ou
						estao incompletos
*/
int initializeCommunication(int argc, const char *argv[]);

/****************************************
 * Comunicacao com servidor de arranque *
 ****************************************/

/*
	descricao: 	executa um pedido ao servidor de arranque pelo o identificador,
				endereco e porto do no de arranque do anel com id @ringId
	retorno:	retorna por referencia a informacao do no de arranque do anel
				retorna por valor um valor de erro
	erro:		caso o anel nao exista é retornado o valor -1 caso contrario é
				retornado o valor 0
*/
int getStartNode(int ringId, Node* startNode);

/*
	descricao:	regista o nó actual como sendo o nó de arranque do anel @ringId com o
				identificador @nodeId. caso o anel @ringId nao exista é criado um anel @ringId
*/
int registerAsStartingNode(int ringId, int nodeId);

#endif