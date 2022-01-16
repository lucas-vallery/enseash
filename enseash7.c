#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>


#define BUFFER_SIZE 128
#define DECIMAL 10
#define MILLION  1000000.0

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

int countSpace(char* message){
	int count = 0;
	for(int i = 0; i < (int) strlen(message); i++)
		count += (message[i] == ' ')?1:0;
	return count;
}

char** fillArgv (char* message){
	int numberOfSpaces = countSpace(message);
	char** argv = (char **) malloc((numberOfSpaces + 2)*sizeof(char*));

	argv[0] = message;
	int argvPos = 1;


	for(int i = 0; i < (int) strlen(message); i++){
		if(message[i] == ' '){
			argv[argvPos++] = message + i + 1;

			message[i]='\0';
		}
	}
	argv[numberOfSpaces+1] = (char*) NULL;
	return argv;
}

void printArgv(char** argv, int lenArgv){
	for(int i = 0; i < lenArgv; i++){
		printToConsole(argv[i]);
		printToConsole("\n");
	}
}

char* checkForRedirection (char* message){
	char* pos = strchr(message, '>');

	if(pos != NULL){
		*pos = '\0';

		//On retire les ' ' avant '>'
		char* posCpy = pos - 1;
		while(strncmp(posCpy, " ", strlen(" ")) == 0){
			*posCpy = '\0';
			posCpy--;
		}

		//On retire les ' ' après '>'
		pos ++;
		while(strncmp(pos, " ", strlen(" ")) == 0)
			pos++;

		return pos;
	}

	return (char*) NULL;
}

void mainLoop(){
	char buffer[BUFFER_SIZE];
	int status;
	struct timespec start, stop;
	float delta_ms = 0;
	char** argv;
	char* redirection;

	while(1){

		clearBuffer(buffer);

		//Retour du prompt
		if(WIFSIGNALED(status)){
			sprintf(buffer, "enseash [SIGN:%d|%.2f ms]%% ", WTERMSIG(status), delta_ms);
			printToConsole(buffer);
		} else if (WIFEXITED(status)) {
			sprintf(buffer, "enseash [EXIT:%d|%.2f ms]%% ", WEXITSTATUS(status), delta_ms);
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


		redirection = checkForRedirection(buffer);

		argv = fillArgv(buffer);

		//Sauvegarde du temps au lancement du processus
		if(clock_gettime(CLOCK_REALTIME, &start) == -1){
			printToConsole("clock_gettime à échoué");
			exit(EXIT_FAILURE);
					}

		pid_t c_pid = fork();

		if(c_pid == -1){
			printToConsole("fork à échoué");
			exit(EXIT_FAILURE);
		}
		//Programme du fils
		else if(c_pid == 0){
			if(redirection != NULL) {
				int fd = open(redirection, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

				dup2(fd, STDOUT_FILENO);
			}

			execvp(buffer, argv);

			if(buffer[0] != '\0'){
				printToConsole("Le processus n'existe pas\n");
			}
			exit(EXIT_FAILURE);
		}
		//Programme du père
		else{
			wait(&status);
			//Sauvegarde du temps à l'arrêt du processus
			if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
				printToConsole("clock_gettime à échoué");
				exit(EXIT_FAILURE);
			}
		}

		//Calcul du temps d'execution du précédent fils
		delta_ms = (float)(stop.tv_sec - start.tv_sec)*1000 + (float)(stop.tv_nsec - start.tv_nsec) / MILLION;
	}

	printToConsole((char*) BYE_TXT);
	exit(EXIT_SUCCESS);
}


int main(){
	printToConsole((char*) WELCOME_TXT);
	mainLoop();
	return 0;
}
