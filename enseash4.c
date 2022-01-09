#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128
#define DECIMAL 10

const char* WELCOME_TXT = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char* EXIT_TXT = "exit";
const char* BYE_TXT = "Bye bye...\n\0";

void printToConsole(char* message){
	if(write(STDOUT_FILENO, message, strlen(message)) == -1)
		exit(EXIT_FAILURE);
}

void clearBuffer(char buffer[BUFFER_SIZE]){
	for(int i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = '\0';
	}
}

void mainLoop(){
	char buffer[BUFFER_SIZE];
	int status;

	while(1){
		//Attendre une commande
		clearBuffer(buffer);

		if(WIFSIGNALED(status)){
			sprintf(buffer, "enseash [SIGN:%d]%% ", WTERMSIG(status));
			printToConsole(buffer);
		} else if (WIFEXITED(status)) {
			sprintf(buffer, "enseash [EXIT:%d]%% ", WEXITSTATUS(status));
			printToConsole(buffer);
		} else {
		printToConsole("enseash% ");
		}

		clearBuffer(buffer);

		if(read(STDIN_FILENO, buffer, BUFFER_SIZE) == -1)
			exit(EXIT_FAILURE);


		//Quitter le shell
		if(strncmp(buffer, EXIT_TXT, strlen(EXIT_TXT)) == 0){
			break;
		}


		//On retire le \n de buffer
		if(buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';


		pid_t c_pid = fork();

		//Le fork à échoué
		if(c_pid == -1){
			printToConsole("Le fork à échoué");
			exit(EXIT_FAILURE);
		}
		//Programme du fils
		else if(c_pid == 0){
			execlp(buffer, buffer, (char*) NULL);
			if(buffer[0] != '\0'){
				printToConsole("Le processus n'existe pas\n");
			}
			exit(EXIT_FAILURE);
		}
		//Programme du père
		else{
			wait(&status);
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


