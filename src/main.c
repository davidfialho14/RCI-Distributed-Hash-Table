#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "defines.h"
#include "common.h"
#include "communication.h"

int main(int argc, char const *argv[]) {

	//inicializacao
	if(initializeCommunication(argc, argv) != 0) exit(-1); //inicailizacao falhou
	putok("inicializacao completa\n");

	int maxFd = curNode.fd;	//descritor com valor mais elevado
	char buffer[BUFSIZE];	//buffer utilizado para fazer a leitura dos descritores
	int inputReady = 0;		//indica se existe input disponivel para ler
	fd_set readFds;			//conjunto de fds prontos para ler
	int quit = FALSE;
	while(!quit) {

		//inicializar conjunto de fds de leitura
		FD_ZERO(&readFds);

		//adicionar listen fd ao conjunto de fds
		FD_SET(curNode.fd, &readFds);
		//adicionar stdin ao conjunto de fds
		FD_SET(STDIN_FILENO, &readFds);
		//adicionar fd das ligacoes existentes

		putdebug("CurNode - ring: %d id: %d ip: %s port: %s fd: %d",
				curRing, curNode.id, curNode.ip, curNode.port, curNode.fd);
		putdebug("SucciNode - id: %d ip: %s port: %s fd: %d",
						succiNode.id, succiNode.ip, succiNode.port, succiNode.fd);
		putdebug("PrediNode - id: %d ip: %s port: %s fd: %d",
						prediNode.id, prediNode.ip, prediNode.port, prediNode.fd);

		//esperar por descritor pronto para ler
		inputReady = select(maxFd + 1, &readFds, NULL, NULL, NULL);
		if(inputReady <= 0) {
			puterror("main", "select falhou");
			sleep(5);
			continue;
		}

		if(FD_ISSET(curNode.fd, &readFds)) {	//testar se o listen fd esta pronto para leitura
			struct sockaddr_in addr;
			socklen_t addrlen = sizeof(addr);
			bzero(&addr, addrlen);

			//aceitar ligacao
			int connectionFd;
			if( (connectionFd = accept(curNode.fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
				puterror("main", "ligacao nao foi aceite");
			} else {
				//adionar descritor ao conjunto de descritores de ligacao
				putdebug("nova ligacao %d addr: %s", connectionFd, inet_ntoa(addr.sin_addr));
			}
		}

		if(FD_ISSET(STDIN_FILENO, &readFds)) { //testar se o utilizador executou um comando

			//ler comando do utilizador
			fgets(buffer, BUFSIZE, stdin);

			//executar comando do utilizador
			putdebug("processar comando do utilizador");
		}

		//ler fds de ligacoes correntes
	}

	return 0;
}
