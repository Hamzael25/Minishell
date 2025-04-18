# Minishell

Minishell est un projet de l'école 42 qui consiste à recréer un shell UNIX minimaliste en C. Il permet d'exécuter des commandes interactives avec un comportement proche de **bash**.

## Fonctionnalités

- Exécution de commandes, pipes (`|`) et redirections (`<`, `>`, `<<`, `>>`)
- Expansions (`$VAR`, `$?`) et gestion des guillemets
- Builtins : `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`
- Gestion des signaux (`Ctrl+C`, `Ctrl+D`, etc.)
- Support de `heredoc` et de la bibliothèque `readline`

## Compilation et exécution

```bash
make
./minishell
```

C'est tout !

---

Projet réalisé dans le cadre du cursus de l'école 42. Court en apparence, long à faire. Une vraie immersion dans les entrailles du shell UNIX.

