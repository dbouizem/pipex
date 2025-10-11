# Pipex

![42 School](https://img.shields.io/badge/42-School-000000?style=flat-square&logo=42&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=flat-square&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat-square&logo=linux&logoColor=black)

Un programme en C qui reproduit le comportement des pipes et redirections du shell Unix, permettant de comprendre en profondeur la communication inter-processus.

## 📖 Table des matières

- [À propos du projet](#-à-propos-du-projet)
- [Comment ça fonctionne](#-comment-ça-fonctionne)
- [Installation](#-installation)
- [Usage](#-usage)
- [Partie Bonus](#-partie-bonus)
- [Implémentation technique](#-implémentation-technique)
- [Tests](#-tests)

## 🎯 À propos du projet

### Qu'est-ce que Pipex ?

Pipex est un projet de l'école 42 qui vous fait recréer le mécanisme des pipes Unix. Lorsque vous tapez cette commande dans un shell :

```bash
< file1 cmd1 | cmd2 > file2
```

Votre shell fait plusieurs choses :
1. 📖 Lit le contenu de `file1`
2. ⚙️ Exécute `cmd1` avec ce contenu en entrée
3. 🔄 Passe la sortie de `cmd1` à `cmd2` via un **pipe**
4. 💾 Écrit le résultat final dans `file2`

**Pipex reproduit exactement ce comportement** en utilisant les appels système Unix.

### Objectifs pédagogiques

Ce projet vous apprend à :

- 🔀 **Créer des processus** : Utiliser `fork()` pour créer des processus enfants
- 📡 **Communication inter-processus** : Utiliser `pipe()` pour faire communiquer les processus
- 🔄 **Redirections** : Maîtriser `dup2()` pour rediriger stdin/stdout
- 🚀 **Exécution de programmes** : Utiliser `execve()` pour lancer des commandes
- 🔍 **Gestion du PATH** : Chercher les exécutables dans les répertoires système
- ⚠️ **Gestion d'erreurs robuste** : Gérer tous les cas d'erreur possibles

## 🔧 Comment ça fonctionne

### Architecture générale

```
┌─────────────┐
│   file1     │ (fichier d'entrée)
└──────┬──────┘
       │
       ↓ (lecture avec open + dup2)
┌─────────────┐
│  Process 1  │ → exécute cmd1
│   (fork)    │
└──────┬──────┘
       │
       ↓ (pipe)
┌─────────────┐
│  Process 2  │ → exécute cmd2
│   (fork)    │
└──────┬──────┘
       │
       ↓ (écriture avec open + dup2)
┌─────────────┐
│   file2     │ (fichier de sortie)
└─────────────┘
```

### Étapes d'exécution

1. **Validation** : Vérification des arguments (4 paramètres minimum)
2. **Création du pipe** : `pipe()` crée un canal de communication
3. **Premier fork** :
   - Ouvre `file1` en lecture
   - Redirige stdin vers `file1` avec `dup2()`
   - Redirige stdout vers l'extrémité d'écriture du pipe
   - Exécute `cmd1` avec `execve()`
4. **Deuxième fork** :
   - Redirige stdin vers l'extrémité de lecture du pipe
   - Ouvre/crée `file2` en écriture
   - Redirige stdout vers `file2`
   - Exécute `cmd2` avec `execve()`
5. **Attente** : Le processus parent attend la fin des deux enfants

### Exemple concret

```bash
./pipex infile "ls -l" "wc -l" outfile
```

**Ce qui se passe :**

1. Le programme lit `infile`
2. Exécute `ls -l` (liste les fichiers du répertoire courant en détail)
3. La sortie de `ls -l` est envoyée via un pipe
4. `wc -l` reçoit cette sortie et compte le nombre de lignes
5. Le résultat (un nombre) est écrit dans `outfile`

## 🚀 Installation

### Prérequis

- GCC ou Clang
- Make
- Système Unix/Linux ou macOS

### Compilation

```bash
# Cloner le repository
git clone https://github.com/dbouizem/pipex.git
cd pipex

# Compiler la version obligatoire
make

# Compiler avec le bonus
make bonus

# Nettoyer les fichiers objets
make clean

# Nettoyer complètement
make fclean

# Recompiler
make re
```

## 💻 Usage

### Version obligatoire

```bash
./pipex file1 cmd1 cmd2 file2
```

**Équivalent shell :**
```bash
< file1 cmd1 | cmd2 > file2
```

### Exemples pratiques

```bash
# Exemple 1 : Filtrer et compter
echo -e "hello\nworld\nhello 42\nbonjour" > input.txt
./pipex input.txt "grep hello" "wc -l" output.txt
cat output.txt  # Affiche : 2

# Exemple 2 : Transformer du texte
./pipex input.txt "cat -e" "grep \$" output.txt

# Exemple 3 : Avec des chemins absolus
./pipex /tmp/input.txt "/bin/cat" "/usr/bin/wc -l" /tmp/output.txt

# Exemple 4 : Pipeline plus complexe
./pipex data.txt "sort" "uniq" result.txt
# Équivalent à : < data.txt sort | uniq > result.txt
```

## 🌟 Partie Bonus

Le bonus étend les fonctionnalités de Pipex pour gérer des cas plus avancés.

### 1. Here_doc (<<)

Le here_doc permet de fournir l'entrée directement depuis le terminal jusqu'à un mot-clé délimiteur.

**Syntaxe :**
```bash
./pipex here_doc LIMITER cmd1 cmd2 file
```

**Équivalent shell :**
```bash
cmd1 << LIMITER | cmd2 >> file
```

**Exemple pratique :**
```bash
./pipex here_doc EOF "grep hello" "wc -l" output.txt
hello world
this is a test
hello 42
another line
hello pipex
EOF
```

**Ce qui se passe :**
1. Le programme attend vos entrées ligne par ligne
2. Vous tapez du texte
3. Quand vous tapez "EOF", l'entrée s'arrête
4. `grep hello` filtre les lignes contenant "hello"
5. `wc -l` compte ces lignes
6. Le résultat est **ajouté** (mode append `>>`) à `output.txt`

**Différences avec la version obligatoire :**
- ✅ Lit depuis stdin au lieu d'un fichier
- ✅ Mode append (`>>`) au lieu d'écrasement (`>`)
- ✅ Arrêt à un délimiteur personnalisé

### 2. Multiple pipes

Le bonus permet de chaîner plus de 2 commandes.

**Syntaxe :**
```bash
./pipex file1 cmd1 cmd2 cmd3 ... cmdN file2
```

**Équivalent shell :**
```bash
< file1 cmd1 | cmd2 | cmd3 | ... | cmdN > file2
```

**Exemple avec 4 commandes :**
```bash
./pipex input.txt "cat" "grep hello" "sort" "uniq" output.txt
# Équivalent à : < input.txt cat | grep hello | sort | uniq > output.txt
```

**Architecture pour multiple pipes :**

```
file1 → cmd1 → pipe1 → cmd2 → pipe2 → cmd3 → pipe3 → cmd4 → file2
        ↓              ↓              ↓              ↓
      fork1          fork2          fork3          fork4
```

**Implémentation :**
- Création de N-1 pipes pour N commandes
- Création de N processus enfants
- Chaque processus :
  - Lit depuis le pipe précédent (ou file1 pour le premier)
  - Écrit vers le pipe suivant (ou file2 pour le dernier)
  - Exécute sa commande

## 🏗️ Implémentation technique

### Structure du code

```
pipex/
├── Makefile
├── includes/
│   └── pipex.h          # Prototypes et structures
├── srcs/
│   ├── main.c           # Point d'entrée
│   ├── parsing.c        # Validation des arguments
│   ├── path.c           # Recherche des exécutables
│   ├── process.c        # Gestion des fork et pipes
│   ├── execution.c      # Exécution des commandes
│   ├── error.c          # Gestion des erreurs
│   └── bonus/
│       ├── here_doc.c   # Gestion du here_doc
│       └── multiple.c   # Gestion des pipes multiples
└── libft/               # Bibliothèque personnelle (si utilisée)
```

### Fonctions système clés

| Fonction | Rôle |
|----------|------|
| `fork()` | Crée un processus enfant (clone du parent) |
| `pipe()` | Crée un canal de communication unidirectionnel |
| `dup2()` | Duplique un descripteur de fichier |
| `execve()` | Remplace le processus actuel par un nouveau programme |
| `access()` | Vérifie si un fichier existe et est accessible |
| `wait()`/`waitpid()` | Attend la fin d'un processus enfant |
| `open()` | Ouvre un fichier |
| `close()` | Ferme un descripteur de fichier |
| `unlink()` | Supprime un fichier (pour here_doc temporaire) |

### Algorithme simplifié

```c
// Version obligatoire (2 commandes)
int main(int ac, char **av, char **env)
{
    int     pipefd[2];
    pid_t   pid1, pid2;

    // 1. Validation
    if (ac != 5)
        error_exit("Usage: ./pipex file1 cmd1 cmd2 file2");

    // 2. Création du pipe
    pipe(pipefd);

    // 3. Premier processus
    pid1 = fork();
    if (pid1 == 0) {
        // Enfant 1
        open file1 → dup2 vers STDIN
        dup2(pipefd[1], STDOUT) → sortie vers pipe
        close(pipefd[0])
        execve(cmd1)
    }

    // 4. Deuxième processus
    pid2 = fork();
    if (pid2 == 0) {
        // Enfant 2
        dup2(pipefd[0], STDIN) → entrée depuis pipe
        open file2 → dup2 vers STDOUT
        close(pipefd[1])
        execve(cmd2)
    }

    // 5. Parent ferme le pipe et attend
    close(pipefd[0])
    close(pipefd[1])
    waitpid(pid1)
    waitpid(pid2)
}
```

### Gestion du PATH

Pour trouver où se trouve une commande (ex: `ls`), le programme :

1. Récupère la variable d'environnement `PATH`
2. Sépare les différents répertoires (split par `:`)
3. Pour chaque répertoire :
   - Concatène le chemin + `/` + nom de la commande
   - Vérifie avec `access()` si le fichier existe et est exécutable
   - Si oui, retourne ce chemin
4. Si aucun chemin ne fonctionne → erreur "command not found"

```c
// Exemple : Chercher "ls"
PATH = "/usr/local/bin:/usr/bin:/bin"
→ Teste "/usr/local/bin/ls" (non trouvé)
→ Teste "/usr/bin/ls" (non trouvé)
→ Teste "/bin/ls" (trouvé ! ✓)
→ Retourne "/bin/ls"
```

## 🧪 Tests

### Tests basiques

```bash
# Préparation
echo -e "Hello World\nBonjour 42\nHello 42\nSalut" > test.txt

# Test 1 : grep + wc
./pipex test.txt "grep Hello" "wc -l" out1.txt
< test.txt grep Hello | wc -l > out2.txt
diff out1.txt out2.txt  # Doit être identique

# Test 2 : cat + sort
./pipex test.txt "cat" "sort" out1.txt
< test.txt cat | sort > out2.txt
diff out1.txt out2.txt

# Test 3 : Fichier inexistant
./pipex nofile "cat" "wc -l" out.txt
# Doit afficher une erreur et créer out.txt vide

# Test 4 : Commande invalide
./pipex test.txt "invalid_cmd" "wc -l" out.txt
# Doit afficher "command not found"
```

### Tests bonus (here_doc)

```bash
# Test here_doc
./pipex here_doc END "cat" "wc -l" out.txt << EOF
line 1
line 2
line 3
END
EOF
cat out.txt  # Doit afficher 3

# Vérifier le mode append
echo "existing content" > out.txt
./pipex here_doc STOP "cat" "cat" out.txt << EOF
new content
STOP
EOF
cat out.txt  # Doit contenir l'ancien + le nouveau contenu
```

### Tests bonus (multiple pipes)

```bash
# Test avec 3 commandes
./pipex test.txt "cat" "grep Hello" "wc -l" out1.txt
< test.txt cat | grep Hello | wc -l > out2.txt
diff out1.txt out2.txt

# Test avec 5 commandes
./pipex numbers.txt "cat" "sort -n" "uniq" "head -3" "wc -l" out.txt
```

### Vérification des fuites mémoire

```bash
# Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./pipex test.txt "cat" "wc -l" out.txt

# Résultat attendu : "All heap blocks were freed -- no leaks are possible"
```

## ⚠️ Gestion des erreurs

Le programme gère correctement :

| Erreur | Comportement |
|--------|-------------|
| Mauvais nombre d'arguments | Affiche l'usage et quitte avec code 1 |
| Fichier d'entrée introuvable | Affiche erreur, continue avec stdin vide |
| Pas de permission lecture | Affiche "Permission denied" |
| Commande introuvable | Affiche "command not found", code 127 |
| Échec de `fork()` | Affiche erreur système et quitte |
| Échec de `pipe()` | Affiche erreur système et quitte |
| Échec de `execve()` | Affiche erreur et quitte avec code 126 |

## 📚 Ressources utiles

### Documentation système

- [fork(2)](https://man7.org/linux/man-pages/man2/fork.2.html) - Création de processus
- [pipe(2)](https://man7.org/linux/man-pages/man2/pipe.2.html) - Création de pipes
- [dup2(2)](https://man7.org/linux/man-pages/man2/dup2.2.html) - Duplication de descripteurs
- [execve(2)](https://man7.org/linux/man-pages/man2/execve.2.html) - Exécution de programmes
- [waitpid(2)](https://man7.org/linux/man-pages/man2/waitpid.2.html) - Attente de processus

### Tutoriels

- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/) - Guide sur la communication inter-processus
- [The Linux Programming Interface](http://man7.org/tlpi/) - Référence complète
