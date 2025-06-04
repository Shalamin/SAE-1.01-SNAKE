# 🐍 SAE 1.01 - Snake

Ce projet est un jeu Snake développé en langage C dans le cadre de la SAE 1.01 du BUT Informatique.
L'objectif était de concevoir un jeu en console basé sur le célèbre Snake, avec une interface rudimentaire et une gestion simple des entrées clavier.

## 📁 Structure du projet

```
.
├── version1/            # Première version du projet. Le serpent se deplace tout seul
├── version2/            # Deuxième version du projet. Le serpent se deplace grâce au mouvement du joueur
├── version3/            # troisième version du projet. Ajout des collisions
├── version4/            # quatrième et dernière version du projet. Ajout de pommes et de pavés de collisions.
├── version_julien/      # Version d'un ami pour l'aider. Version non-réaliser par mes soins
├── version_constance/   # Version d'une amie pour l'aider. Version non-réaliser par mes soins
└── README.md            # Documentation du projet
```

## 🧠 Objectifs pédagogiques

- Comprendre la structuration d’un projet en langage C
- Utiliser des pointeurs, structures et tableaux
- Implémenter des fonctionnalités ludiques avec gestion du clavier

## ▶️ Compilation et exécution

### Prérequis

- Un compilateur C (GCC)
- Un terminal sous Linux/macOS ou compatible avec ANSI (sous Windows, utiliser Git Bash ou WSL)

### Compilation

Dans le terminal, à la racine du projet :

```bash
  cd /<nom_version>
```


### Lancer le jeu

```bash
./snake
```

## 🎮 Règles du jeu

- Le serpent se déplace automatiquement vers la droite
- Le joueur utilise les touches `z` (haut), `q` (gauche), `s` (bas) et `d` (droite) pour changer la direction
- Le but est de manger les pommes (`@`) sans se mordre ni toucher les murs et les différents pavés

## 💾 Fonctionnalités

- Système de score
- Interface textuelle simple
- Détection des collisions

## 🧑‍💻 Équipe de développement

Projet réalisé dans le cadre de la SAE 1.01 (BUT Informatique – Semestre 1)

- **[Shalamin](https://github.com/Shalamin)**


## 📄 Licence

Projet académique — utilisation non commerciale uniquement.
