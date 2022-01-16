# Tp de synthèse — Ensea in the shell

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
Une fois le text recupéré, nous devons l'exécuter si il s'agit d'une commande. Ceci est fait à l'aide de la ligne de code suivante, 

    execlp(buffer, buffer, (char*) NULL);
        
Avant d'exécuter une ligne de code, nous devons faire un *fork()*. En effet, nous voulons que notre shell soit toujours actif au retour du programme appelé.
Pour ce faire, seul le processus fils exécute la commande. Le processus père attend que le processus fils retourne pour reprendre l'exécution du shell.

---

>**Question 3** Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d

L'entièreté du code s'exécute dans une boucle infinie. Pour le quitter il suffit donc d'un *break()* quand la commande ''exit'' est reconnue. La principale subtilité de cette étape est la ligne :
    
    if(strncmp(buffer, EXIT_TXT, strlen(EXIT_TXT)) == 0)
    
Nous ne comparons pas l'intégralité de la chaine saisie dans le shell mais seulements les premiers caractères (ceux de ''exit''). Ceci permet d'éliminer les problèmes de comparaisons dus à la présence du \n à la fin de la ligne saisie dans le shell.

---
    
>**Question 4** Affichage du code de retour (ou du signal) de la commande précédente dans le prompt
    
Plutôt d'exécuter un *wait(NULL)*, le père exécute un *wait(&status)*. Ceci permet d'attendre que le fils change d'état (se termine) et de stocker son statut afin d'y accéder plus tard à l'aide de 
    
    WEXITSTATUS(status)
    
Ici, on accède au code de fin renvoyé par le fils. On peut également savoir comment le fils s'est terminé.
    
---
    
>**Question 5** Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime

La mesure du temps d'exécution d'un programme se fait à l'aide de la fonction *clock_gettime()*. Cette fonction est appelée à la création du processus fils et à sa terminaison. La différence de ces deux valeurs nous permet d'obtenir le temps d'exécution du processus. 
    
---
    
>**Question 6** Exécution d’une commande complexe (avec arguments)
    
Nous utilisons un exemple pour illustrer le fonctionnement des fonctions utilisées pour répondre à cette question. Supposons que la commande saisie dans le shell soit 
    
    $ fortune -s osfortune
    
Pour exécuter une telle commande nous utiliserons la fonction :
    
    int execvp (const char *filename, char *const argv[]);
    
Notons que *argv[]* est un tableau contenant des chaînes de caractères. Dans notre exemple, 
    
    filename = "fortune";
    argv[0] = "fortune";
    argv[1] = "-s";
    argv[2] = "osfortune";
    
Le problème est donc de construire *argv[]* à partir de la chaîne de caratères "fortune -s osfortune".
On commence par compter le nombre d'espaces présents dans la chaîne, on en déduit le nombre d'arguments. On parcourt une seconde fois la chaîne afin recopier dans *argv[]* les pointeurs de début de chaque mots. On remplace également tous les espaces par des"\0" afin de "découper" la chaîne de caractères récupérée du shell. On obtient alors :
    
        "fortune\0-s\0osfortune\0"
         ^        ^   ^
         |        |   |
    argv[0]  argv[1]  argv[2]
    
Nous pouvons maintenant utiliser la fonction *execvp()* pour exécuter notre commande.
    
---
    
>**Question 7** Gestion des redirections vers stdin et stdout avec "<" et ">"
    
Nous devons maintenant traiter une ligne du type :
    
    $ ls > filelist.txt
    
Nous commençons par trouver l'éventuel ">". Si il est présent dans la chaîne de caractères, on change le fichier de retour pour "filelist.txt" puis on exécute la commande en amont du ">" comme précédement.   
    
    
    
