# Tower-Defense
Programmation avancée en C, L3 informatique, Université Gustave Eiffel

## Rapport de projet Tower-Defense réalisé par la collaboration entre HNAIEN Achraf & HONORE Mathieu.

### Compilation & lancement
Après avoir dézippé le projet, pour le compiler il vous suffit de faire une 'make' dans le répértoire du projet. Ce make créera alors un executable nommé 'game' il créera aussi tout les fichier objet dans le dossier 'obj'. Pour lancer le jeu il vous suffit de faire './game' dans le répértoire 'bin'. Le ocde c se trouve dans le dossier 'src' et les entête de ces fichiers dans le dossier 'include'.
Vous pouvez lancer le programme avec -h ou --help pour avoir un rapide résumé des choses que vous pouvez faire dans le jeu.

### La notice pour jouer.
Une fois dans le jeu, plusieurs possibilités s'offrent à vous. Si vous voulez acheté des choses ou utiliser le menu de droite, vous devez utiliser le clique gauche. Si vous avez achetés des gemmes et quelles sont présente dans l'inventaire vous pouvez les selectioner pour les mettre soit dans les tours, soit dans les cases de fusions. Une fois dans les cases de fusions vous pouvez recuperer la gemme dans votre inventaire par la même manière. Pour placer des tour il vous suffit d'appuyer sur le bouton tower (le bouton deviendra alors vert, vous serez donc en mode "Construction"), vous pourrez ensuite la placer en appuyant sur une case de la carte. En mode construction vous ne pouvez plus vous servir du menu à droite (sauf pour annuler le mode contrcution).
Pour augmenter la reserve de mana vous devez appuyer sur le bouton de la reserve de mana à droite.
Si vous avez placé une gemme sur une tour, vous pouvez faire clique droit sur la tour pour recuperer la gemme dans votre inventaire, à conditions que votre inventaire ne soit pas plein.
Vous pouvez ensuite appuyer sur 'espace' pour lancer une vague de monstre à n'miporte quel moment. Les monstres apparaissent automatiquement au bout de 35s (sauf pour la première vague). Vous gagnerez du mana si il reste du temps avant la prochaine vague et que vous déclanchez la vague.
Enfin, si vous en avez marre de jouer a notre jeu... Vous pouvez appuyer sur 'echap' pour quitter le jeu.

### Quelques.. subtilités
Les gemmes que vous achetez sont pures elle dispose donc d'un type, les types appliquent des effets différents aux monstres (cf sujet). La gemme achetée dispose d'un type et d'une teinte completements aléatoires.
Vous pouvez augmenter le niveau de la gemme que vous allez acheter (à l'aide du '+' et du '-') le prix augmente par conséquent...
Vous pouvez fusionner les gemmes seulement si leur niveau sont égaux. Si l'inventaire est plein et que vos cases de fusions sont pleines, pas de panique, vous pouvez selectionner les gemmes et les interchangés (pour fusionner des gemmes de niveau équivalent et ainsi gagner une places).
Le niveau des gemmes est marqué au centre de la gemme pour faciliter les fusions.

### La finalité 
Lorsqu'un monstre rentre dans votre camp il vous fais predre du mana.
Le jeu se termine une fois que vous n'avez plus de mana et qu'un monstre rentre dans votre quand ou lorsque vous appuyez sur 'echap'.
Sinon les vagues spawn à l'infini mais elles deviennent de plus en plus dur.