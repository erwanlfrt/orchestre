# orchestre
Projet contenus multimédia

## Prérequis
Le programme est dépendant de la librairie ncurses. Si elle n'est pas installée sur votre poste, vous pouvez l'installer à l'aide des commandes suivantes :

Pour Ubuntu : sudo apt-get install libncurses5-dev libncursesw5-dev

Pour CentOS/RHEL/Scientific Linux 6.x/7.x+ and Fedora Linux 21 ou plus anciens : sudo yum install ncurses-devel

Pour Fedora Linux 22.x+ : sudo dnf install ncurses-devel

Pour Mac : brew install ncurses (nécessite le gestionnaire de paquet Homebrew)


## Compilation
Depuis la racine du projet, exécuter les commandes suivantes : 
make orchestra
make musician

Les fichiers compilés se trouveront dans le répertoire bin/.

## Exécution

  ### 1. Orchestre
    Pour lancer l'orchestre en mode serveur, depuis la racine : 
      ./bin/orchestra
    Pour que l'orchestre lise un script, depuis la racine : 
      ./bin/orchestra <script-path>

  ### 2. Musicien
  Pour lancer un musicien, depuis la racine : 
    ./bin/musician

  Ensuite, sélectionner un instrument en anglais (violin ou piano) puis sélectionner une position avec les touches "q" et "d". Appuyer sur "entrée une fois la position choisie.

  Une fois sur le menu, le petit chevron ">" à gauche indique sur quelle option vous vous situez. Si vous désirez changer d'option, utilisez les touches "z" et "d" pour naviguer de haut en bas dans le menu. Pour lancer le son ou l'arrêter, allez en haut du menu et appuyez sur "entrée".

    #### A. Menu position

      Le menu position affiche une boussole avec en vert votre position actuelle dans l'orchestre. Vous pouvez changer de positon avec les touches "q" et "d" et "o" si vous désirez vous replacer au centre.
      Appuyez sur la touche "echap" pour revenir au menu précédent.

## Information
  ### 1. Fichier script
    Un fichier script doit nécessairement contenir trois parties : CREATE, RUN, EXIT. Si votre fichier n'est pas valide alors le programme s'arrêtera et votre fichier ne sera pas joué. Ci-dessous, la documentation du contenu d'un fichier script.

    #### CREATE
      Chaque ligne doit être sous la forme "id instrument position"

    #### RUN
      Les commandes possibles sont : 
        - play <id>
        - stop <id>
        - wait <seconds>
        - direction <id> <position>
    
    Voici un exemple d'un fichier valide : 
      CREATE
      1 violin E
      2 piano E

      RUN
      play 1 
      wait 5 
      play 2
      direction 1 W
      wait 3
      stop 1

      EXIT

  ### 2. Position possible
    N, NE, E, SE, S, SW, W, NW, CENTER
