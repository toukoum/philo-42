# Philo Guideline

![Alt text](image/showRoom.png?raw=true "Title")

---

### RUN le programme

```c
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- time en milliseconde

**Structure `table`** : Cette structure englobe les philosophes et les fourchettes.

**Fourchettes et Mutex** : Les fourchettes sont représentées par un tableau de mutex. Il y a autant de mutex (fourchettes) que de philosophes.

**Initialisation des Philosophes** : Chaque philosophe est initialisé avec deux fourchettes, l'une à droite (index **`i`**) et l'autre à gauche (index **`(i + 1) % nombre_total_de_philosophes`**).

**Prise de Fourchettes** : Lorsqu'un philosophe prend une fourchette, il verrouille le mutex correspondant à cette fourchette, assurant qu'il est le seul à l'utiliser à ce moment.

**Logs** : Les messages de log sont affichés en utilisant un mutex commun pour l'affichage. Cela assure que les messages de différents philosophes ne se mélangent pas et que l'affichage est ordonné.

```c
typedef struct s_philo
{
	size_t				rank;  // id du philo
	pthread_t			thread;  // thread qui représente la vie de philo, son corps
	pthread_mutex_t		*fork_left; // pointeur vers la bonne fourchette
	pthread_mutex_t		*fork_right; // de la table
	long long			last_meal; // temps du dernier repas 
	t_table				*table; // pointeur vers la table
	size_t				count_meal;
}						t_philo;

// struct for contain all the philosophers
struct					s_table
{
	t_philo				*philos; // tableau de philo (tout mes philos)
	pthread_t			philo_life; // thread de monitoring qui verifie a chaque instant si les philo sont toujours en vie
	pthread_mutex_t		*forks; // tableau de mutex (fourchette) il faut lock les bonnes pour graille
	pthread_mutex_t		m_dead; // mutex for the end 
	pthread_mutex_t		m_count_meal; // mutex for count_meal_value of thread
	pthread_mutex_t		m_log; // mutex for printing
	pthread_mutex_t		m_last_meal; // mutex for the timestamp of the last meal
	size_t				number_philo; // nombre de philo
	size_t				time_to_die; // temps qu'il ont avant de mourir (ms)
	size_t				start_time_simu; // temps du début de la simulation
	size_t				time_to_eat; // temps pour graille
	size_t				time_to_sleep; // temps pour midor
	size_t				must_eat; // nb de fois qu'il doiv graille, 0 si pas présent
		int					dead; // fin de la simulation
};
```

**Pourquoi avoir un mutex pour les messages de chaque philosophe?**

pour verrouiller le printage du message au philosophe qui le demande, comme ca il est le seul a pouvoir ecrire sur la sortie, et les message arrive dans un ordre cohérent

---

### Défis a résoudre:

⇒ **Deadlock** (interblocage) ⇒ quand des thread attendent de lock les ressources que l’autre a et inversement. 

**Résolution** les philos avec un rank paire commence par prendre leur fourchette droite puis gauche et inversement

```c
static void	assign_forks(t_philo *philo, size_t i)
{
	if (philo->rank % 2) // impair
	{
		philo->fork_left = &philo->table->forks[i]; // ordre "normal"
		philo->fork_right = &philo->table->forks[(i + 1)
			% philo->table->number_philo];
	}
	else // pair 
	{
		philo->fork_right = &philo->table->forks[i]; // ordre "inversé"
		philo->fork_left = &philo->table->forks[(i + 1)
			% philo->table->number_philo];
	}
}
```

**Résolution2** les philo impair commence par attendre time_to_eat, ce sont donc les philo pair qui vont choper les fourchette en premier et pouvoir se regaler la chique !

⇒ **Starvation** (famine): Quand un thread n’obtient jamais les ressources qu’il souhaite. Donc par ex un philo n’arrive jamais a avoir les deux fourchette, ses potes les prennent toujours avant lui.

**Résolution** calcul du temp de “time_to_think” adéquat, en fonction de combien de temps le philo a avant de devoir remanger, on calcule le temp qu’il a pour “penser” comme ca il ne demande pas TOUT LE TEMPS une fourchette dès qu’il a fini de dormir 

```c

Time_to_think = time_to_die - time_since_last_meal - time_to_eat / 2
if (time_to_think <= 0)
	time_to_think = 0;
else if (time_to_think > 600) // pour pas qu'un philo pense trop longtemps
	time_to_think = 200;
msg_log(THINK);
ft_wait(time_to_think);
```

**⇒ Problème de synchronisation** Si les thread ne commence pas en même temps, cela introduit des potentiel problème de deadlock ou de monopolisation de fourchette.

**Résolution** il faut donc que tout les thread commence en meme temps, on definit une heure de début de programme et quand on lance les thread, on attend cette heure de départ pour commencer les hostilités.

```c

time_start = get_time() + (table.number_philo * 20);
void	synch_start(long long time_start)
{
	while (get_time() < time_start)
	{
		continue ;
	}
}
```

---

- Conversion tab
    
    1 seconde = 1 000 milliseconde
    
    1 seconde = 1 000 000 microseconde
    

---

## Functions used

<aside>
✅ **usleep(useconds_t usec)**

</aside>

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Sleeping for 500000 microseconds...\n");
    usleep(500000);
    printf("Done sleeping.\n");
    return 0;
}
```

<aside>
✅ **gettinmeofday(struct timeval *restrict tv, struct timezone *restrict tz)**

</aside>

le deuxieme argument est toujours NULL.

The current time is expressed in elapsed seconds and microseconds since 00:00:00, January 1, 1970

```c
#include <sys/time.h>
#include <stdio.h>

int main() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  printf("seconds : %ld\nmicro seconds : %ld",
    current_time.tv_sec, current_time.tv_usec);

  return 0;
}
```

```c
struct    timeval  {
  time_t        tv_sec ;   //used for seconds
  suseconds_t       tv_usec ;   //used for microseconds
}
```

<aside>
✅ **int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr,
void *(*start_routine)(void *), void *restrict arg);**

</aside>

The `pthread_create()` function starts a new thread in the calling process. The new thread starts execution by invoking `start_routine()`; `arg` is passed as the sole argument of `start_routine()`

<aside>
✅ **int pthread_join(pthread_t thread, void **retval);**

</aside>

The `pthread_join()` function waits for the thread specified by `thread` to terminate. If that thread has already terminated, then `pthread_join()` returns immediately.

---

# BONUS

## Plan

Il y a N philosophes

Les philosophes sont représenté par des child process fork().

Il y a N fourchette qui sont représenté par un sémaphores sem_t *forks.

### **Étape 1: Initialisation**

1. **Définir les Structures :**
    - [ ]  **`t_philo`** pour chaque philosophe.
    - [ ]  **`s_table`** pour la table, incluant les sémaphores et les PIDs des enfants.
2. **Initialiser les Sémaphores :**
    - [ ]  Créez et initialisez les sémaphores nécessaires (**`sem_forks`**, **`sem_log`**, **`sem_dead`**, **`sem_eat_full`**, etc.).
3. **Fork des Processus Enfants :**
    - [ ]  Créez **`N`** processus enfants pour représenter les **`N`** philosophes.
    - [ ]  Stockez les PIDs dans **`child_pids`** de **`s_table`**.

Malloc un tableau de philo avant de fork, sinon galère pour la mémoire.

### **Étape 2: Routine des Philosophes**

Dans chaque processus enfant (philosophe) :

1. **Boucle Infinie :**
    - [ ]  Chaque philosophe exécute une boucle infinie où il effectue les actions de manger, dormir et penser.
2. **Manger :**
    - [ ]  Utiliser **`sem_wait`** sur **`sem_forks`** pour prendre deux fourchettes.
    - [ ]  Simuler l'action de manger, puis **`sem_post`** sur **`sem_forks`** pour les remettre.
3. **Dormir :**
    - [ ]  Simuler l'action de dormir. (time_to_sleep)
4. **Penser :**
    - [ ]  Simuler l'action de penser (voir calcul dans le code).

### **Étape 3: Surveillance de la Vie**

Dans chaque processus enfant (philosophe) :

1. **Créer un Thread de Surveillance :**
    - [ ]  Le thread vérifie en continu si le philosophe est encore en vie.
    - [ ]  Si le philosophe meurt (par exemple, n'a pas mangé dans le temps imparti), le thread fait **`sem_post`** sur **`sem_dead`**  (comme ca main process est informé de la mort d’un philo).

### **Étape 4: Thread de Surveillance**

1. **Créer un Processus de Surveillance pour la vie d’un philo:**
    - [ ]  Ce processus attend le sémaphore **`sem_dead`** avec **`sem_wait`**.
    - [ ]  Si **`sem_dead`** est incrémenté, cela signifie qu'un philosophe est mort.
2. **Créer un Processus de Surveillance pour le nombre de repas d’un philosophe:**
    - [ ]  dès qu’un philo a assez mangé, son child process qui le représente envoie un post sur le semaphore sem_eat_full
    - [ ]  des que sem_eat_full est ≥ nombre de philos, on termine la simulation comme si un philo été mort
3. **Terminer les Philosophes :**
    - [ ]  Utilisez **`kill`** pour envoyer un signal de terminaison à chaque processus enfant en utilisant les PIDs stockés dans le tableau de philo t_philos **philos

### **Étape 5: Nettoyage**

1. kill de tous les philo ⇒ sem_close() des deux semaphore locale de chaque philos
2. Join les thread de surveillance
3. close de tout les semaphores ouvert dans le main process
4. free du tableau de philo

```c
typedef struct s_philo
{
	size_t				rank; // rank of the child process, id of the philo
	pid_t         pid_philo; // the pid of the philo
	long long			last_meal; // time of the last meal of the philo
	size_t				count_meal; // number of eat of the philo
	sem_t         *sem_last_meal; // binary semaphore for access to last meal
	sem_t         *sem_count_meal // binary semaphore for access to count meal
	pthread_t     handle_life; // thread for handle the life of the philo
	t_table       *table.       // ptr to the table, IT'S A COPY OF TABLE IN CHILD PROCESS
}						t_philo;

struct					s_table
{
	sem_t				*sem_forks; // Global semaphore for forks
	sem_t				*sem_log; // Global semaphore for print message
	sem_t				*sem_dead; // Global semaphore for death of one philo
	sem_t.      *sem_eat_full // Global semaphore for know how many philo has enough eaten
	set_t       *sem_set_end // to inform the thread gluttony that is the end of the simulation
	pid_t				*child_pids; // array of child pid
	size_t				number_philo;
	size_t				time_to_die;
	size_t				start_time_simu;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				must_eat;
};
```

Truc qui m’on bien fait galéré sa mere:

- valgrind memory, erreur de still reachable car je ne fermait pas tout mes semaphore en particulier ces deux,
    
    

```c
sem_close(((*philos) + i)->sem_count_meal);
sem_close(((*philos) + i)->sem_last_meal);
```

- Erreur de semaphore avec helgrind: en fait tu ne peux pas sem_open un semaphore a 0 puis faire sem_post en ayant un autre thread qui fait sem_wait dessus, sinon tu as cette erreur

```c
==15366== Thread #3: Bug in libpthread: sem_wait succeeded on semaphore without prior sem_post
==15366==    at 0x486FB04: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)
==15366==    by 0x109C6B: check_eat_enough (process_check_life.c:64)
==15366==    by 0x486F41B: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)
==15366==    by 0x491D5C7: start_thread (pthread_create.c:442)
==15366==    by 0x4985EDB: thread_start (clone.S:79)
==15366==
```

du coup il faut faire sem_open(”name”, nb_philos)

puis ensuite faire des sem_post.

- Tout initialiser à la base, faire un tableau de philos plutot que de les intialiser dans un child process, sinon galere pour la memoire
- Quand tu fais un fork, tout les philos herite de toute la memoire alloué, donc pas besoin d’ouvrir un nouveau semaphore pour le partager avec le main process car t_philo contient un pointeur vers la table
- Quand tu fais un kill, le thread ouvert dans le child process que tu veux kill n’a pas besoin d’être join, on ne saura jamais pourquoi mdr
