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

