# Tp de synthèse -- Ensea in the shell

Le sujet a été réalisé avec Eclipse. Le contenu de ce *repository* est l'ensemble du projet.
Le fichier enseashX.c correspond à la question X.

---

>**Question 1** Affichage d’un message d’accueil, suivi d’un prompt simple.

La subtilité de cette question est de ne pas utiliser *printf()*. Nous préférons le 

    write(int filedes, const void *buffer, size_t size);

Nous retournons du texte en écrivant dans le ''fichier'' *STDOUT_FILENO*.

---

>**Question 2** Exécution de la commande saisie et retour au prompt

La ligne suivante permet de lire une commande   

    read(STDIN_FILENO, buffer, BUFFER_SIZE);
        
En effet, le ''fichier'' *STDIN_FILENO* contient le texte saisi par l'utilisateur.
Une fois le text recupéré, nous devons l'executer si il s'agit d'une commande. Ceci est fait à l'aide de la ligne de code suivante, 

    execlp(buffer, buffer, (char*) NULL);
        
Avant d'executer une ligne de code, nous devons faire un *fork()*. En effet, nous voulons que notre shell soit toujours actif au retour du programme appelé.
Pour ce faire, seul le processus fils execute la commande. Le processus père attend que le processus fils retourne pour reprendre l'execution du shell.

---

>**Question 3** Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d

L'entièreté du code s'execute dans une boucle infinie. Pour le quitter il suffit donc d'un *break()* quand la commande ''exit'' est reconnue. La principale subtilité de cette étape est la ligne :
    
    if(strncmp(buffer, EXIT_TXT, strlen(EXIT_TXT)) == 0)
    
En effet, on ne compare pas l'intégralité de la chaine saisie dans le shell mais seulements les premiers caractères (ceux de ''exit''). Ceci permet d'éliminer les problèmes de comparaisons dus à la présence du \n à la fin de la ligne saisie dans le shell.

---
    
>**Question 4** Affichage du code de retour (ou du signal) de la commande précédente dans le prompt
    
Plutôt d'executer un ''wait(NULL)'', le père execute un ''wait(&status)''. Ceci permet d'attendre que le fils change d'état (se termine) et de stocker son statut afin d'y accéder plus tard à l'aide de 
    
    WEXITSTATUS(status)
    
Ici, on accède au code de fin renvoyé par le fils. On peut également savoir comment le fils s'est terminé.
    
---
    
>**Question 5** Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime
    
    
    
    
    
    
    
