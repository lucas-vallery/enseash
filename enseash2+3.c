#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128

const char* WELCOME_TXT = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char* EXIT_TXT = "exit";
const char* BYE_TXT = "Bye bye...\n\0";

void printToConsole(char* message){
	if(write(STDOUT_FILENO, message, strlen(message)) == -1)
		exit(EXIT_FAILURE);
}


void mainLoop(){
	char buffer[BUFFER_SIZE];

	while(1){
		//Attendre une commande
		printToConsole("enseash% \0");
		if(read(STDIN_FILENO, buffer, BUFFER_SIZE) == -1)
			exit(EXIT_FAILURE);


		//Quitter le programme
		if(strncmp(buffer, EXIT_TXT, strlen(EXIT_TXT)) == 0){
			break;
		}


		//Executer une commande
		pid_t c_pid = fork();

		buffer[strlen(buffer)-1] = '\0';		//On retire le \n de buffer

		if(c_pid == -1){						//Le fork à échoué
			printToConsole("Le fork à échoué");
			exit(EXIT_FAILURE);
		}
		else if(c_pid == 0){				  	//Programme du fils
			execlp(buffer, buffer, (char*) NULL);

			if(buffer[0] != '\0'){				//Si ce n'est pas un retour à la ligne
				printToConsole("Le processus n'existe pas\n");
			}
			exit(EXIT_FAILURE);
		}
		else{						      		//Programme du père
			wait(NULL);
		}
	}

	printToConsole((char*) BYE_TXT);
	exit(EXIT_SUCCESS);
}


int main(){

	printToConsole((char*) WELCOME_TXT);
	mainLoop();
	return 0;
}


