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
