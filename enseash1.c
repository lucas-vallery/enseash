#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* WELCOME_MESSAGE = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %% \0";

void showWelcomeMessage (){

	write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

int main() {

	showWelcomeMessage();
	return 0;
}


