/*****************
**  Vit Pavlik  **
**   xpavli0a   **
**    251301    **
**              **
**   Created:   **
**  2022-11-28  **
**              **
** Last edited: **
**  2022-11-28  **
*****************/

/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf, INFINITY
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO done
    c->size = 0;
    c->obj = malloc(sizeof(struct obj_t) * cap);
    if (c->obj == NULL)
    {
        dfmt("allocating failed for cluster_t at %p", (void*)c);
        c->capacity = 0;
        return;
    }
    dfmt("allocated memory for %d obj_ts in cluster_t at %p", cap, (void*)c);
    c->capacity = cap;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO done

    /* Naplneni nedefinovanych polozek shluku nulovym objektem asi neni 
    *  potreba, ale uz jsem to napsal, tak to tu necham. */
    struct obj_t empty_obj;
    empty_obj.id = 0;
    empty_obj.x = 0;
    empty_obj.y = 0;

    for (int i = 0; i < c->capacity; i++)
    {
        c->obj[i] = empty_obj;
        dfmt("zeroed %dth element of cluster_t at %p", i, (void*)c);
    }

    c->size = 0;
    dfmt("set size to zero for cluster_t at %p", (void*)c);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO done
    dfmt(
        "appending to cluster_t at %p (size %d, capacity %d)", 
        (void*)c, 
        c->size, 
        c->capacity
        );

    if (c->size == c->capacity)
    {
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
        if (c->obj == NULL)
        {
            debug("resizing failed, aborting append_cluster");
            return;
        }
    }
    c->obj[c->size] = obj;
    c->size += 1;

    dfmt(
        "appended successfully to cluster_t at %p (size %d, capacity %d)", 
        (void*)c, 
        c->size, 
        c->capacity
        );
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO done
    dfmt(
        "merging clusters at "
        "%p (size - %d, capacity %d) and " 
        "%p (size - %d, capacity %d)", 
        (void*)c1, 
        c1->size, 
        c1->capacity, 
        (void*)c2, 
        c2->size, 
        c2->capacity
        );

    for (int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);

    dfmt(
        "merged clusters at "
        "%p (size - %d, capacity %d) and " 
        "%p (size - %d, capacity %d)", 
        (void*)c1, 
        c1->size, 
        c1->capacity, 
        (void*)c2, 
        c2->size, 
        c2->capacity
        );
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO done
    /*
        iteruje skrze pole a v momente kdy narazi na polozku k odstraneni
        posouva v kazde dalsi iteraci prvky o jeden vlevo
    */
    int offset = 0;
    for (int i; i < narr; i++)
    {
        carr[i + offset] = carr[i];
        if (i == idx)
        {
            offset = -1;
        }
    }
    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO done
    float xdelta = o1->x - o2->x;
    float ydelta = o1->y - o2->y;
    return sqrtf(xdelta * xdelta + ydelta * ydelta);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO done
    float min_distance = INFINITY;
    float current_distance;
    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; j++)
        {
            if (
                (current_distance = obj_distance(c1->obj + i, c2->obj + j)) 
                < 
                min_distance
               )
            {
                min_distance = current_distance;
            }
        }
    }
    return min_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO done
    debug("find neighbours called");
    float min_dst = INFINITY;
    float dst;
    for (int i = 0; i < narr; i++)
    {
        for (int j = 0; j < narr; j++)
        {
            dst = cluster_distance(carr + i, carr + j);
            if (i != j && dst < min_dst)
            {
                min_dst = dst;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    
    /* omezeni delky radku pro vstup */
    const int int_maxlen = 10;  // 2^31 ma 10 cifer
    const int first_line_maxlen = int_maxlen + 7 + 1;  // format 'count='
    const int cluster_line_maxlen = 3 * int_maxlen + 3 + 1;  // id, x, y, 3 ' '
    
    /* pokus o otevreni souboru */
    FILE *file_obj;
    if ((file_obj = fopen(filename, "r")) == NULL)
    {
        *arr = NULL;
        fprintf(stderr, "\"%s\" either does not exist or cluster is lacking"
        " permission to open it\n", filename);
        return -1;
    }

    /* pokus o zjisteni poctu shluku */
    int cluster_count = 0;
    char first_line[first_line_maxlen];

    if (fgets(first_line, first_line_maxlen, file_obj) == NULL)
    {
        *arr = NULL;
        fprintf(stderr, "Given file is apparently empty.\n");
        return -1;
    }
    dfmt("first line of %s:\n%s", filename, first_line);
    
    if (sscanf(first_line, "count=%i", &cluster_count) != 1)
    {
        *arr = NULL;
        fprintf(stderr, "Wrong formatted first line.\n");
        return -1;
    }
    dfmt("looking for %d clusters", cluster_count);

    /* V tento moment uz docela jiste mame pocet shluku,
    *  pojdme alokovat pamet. */
    if ((*arr = malloc(cluster_count * sizeof(struct cluster_t))) == NULL)
    {
        fprintf(
                stderr, "Memory for %d clusters couldn't be allocated\n", 
                cluster_count
               );
        return -1;
    }

    /* Prostor pro struktury shluku mame, nyni je nacteme ze souboru */
    char curr_clust_line[cluster_line_maxlen];
    struct cluster_t current_cluster;
    struct obj_t current_obj;
    int id, x, y;
    char abort = 0;

    /* iterace skrze radky souboru */
    for (int cluster_no = 0; cluster_no < cluster_count; cluster_no++)
    {
        /* nacteni radku */
        if (fgets(curr_clust_line, cluster_line_maxlen, file_obj) == NULL)
        {
            fprintf(stderr, "Error parsing line %d\n", cluster_no + 2);
            abort = 1;
        }
        dfmt("%dth line of %s:\n%s", cluster_no, filename, curr_clust_line);

        /* parsovani radku */
        if (sscanf(curr_clust_line, "%i %i %i", &id, &x, &y) != 3)
        {
            fprintf(stderr, "Wrong format on line %d\n", cluster_no + 2);
            abort = 1;
        }
        dfmt("parameters read: id=%d, x=%d, y=%d", id, x, y);

        /* kontrola jestli neskoncil soubor */
        if (feof(file_obj) && cluster_no < cluster_count - 1)
        {
            fprintf(
                    stderr,
                    "Too few clusters (should be %d, found %d)\n", 
                    cluster_count, 
                    cluster_no + 1
                   );
            abort = 1;
        }

        /* pokus o inicializaci jednoobjektoveho shluku */
        init_cluster(&current_cluster, 1);
        clear_cluster(&current_cluster);
        if (current_cluster.obj == NULL)
        {
            fprintf(stderr, "Couldnt allocate memory for cluster objects.\n");
            abort = 1;
        }

        /* abort pokud nektery z pokusu vyse selhal */
        if (abort)
        {
            free(*arr);
            *arr = NULL;
            return -1;
        }

        /* Jelikoz nic vyse neselhalo:
        *  Nyni by melo jit bez problemu ulozit 
        *  nactena data z aktualniho radku */
        current_obj.id = id;
        current_obj.x = x;  // pozor, zde probiha konverze int na float
        current_obj.y = y;
        /* funkce append_cluster volana z nasledujiciho radku by nemela
        *  realokovat (cluster by vzdy mel mit kapacitu), 
        *  proto ji ponecham takto */
        append_cluster(&current_cluster, current_obj);
        (*arr)[cluster_no] = current_cluster;
    }
    return cluster_count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    
    /* Chyba: nespravny pocet argumentu */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: cluster [FILE] [CLUSTER_COUNT]\n");
        return 1;
    }

    /* Chyba: spatny format nebo hodnota poctu shluku */
    int target_cluster_no;
    if (!sscanf(argv[2], "%i", &target_cluster_no) || target_cluster_no < 1)
    {
        fprintf(stderr, "Invalid cluster count: \"%s\"\n", argv[2]);
        fprintf(stderr, "Usage: cluster [FILE] [CLUSTER_COUNT]\n");
        return 1;
    }
    dfmt("desired cluster count is %d", target_cluster_no);

    /* Nacteni objektu ze souboru */
    int cluster_count = load_clusters(argv[1], &clusters);

    /* Chyba: Nepodarilo se nacist shluky ze souboru (chybova hlaseni 
    *  implementovana v load_clusters) */
    if (cluster_count == -1 || clusters == NULL)
    {
        fprintf(stderr, "Aborting execution...\n");
        return 1;
    }
    debug("successfully loaded clusters");

    /* Chyba: Chceme po programu vice shluku nez je objektu v souboru */
    if (target_cluster_no > cluster_count)
    {
        fprintf(
                stderr, 
                "Can't make %d clusters out of %d objects\n", 
                target_cluster_no, 
                cluster_count
               );
        return 1;
    }

    /* Nasledujici cyklus merguje sousedy nez 
    *  je dosazeno ciloveho poctu shluku */
    char done = cluster_count == target_cluster_no;
    int idx1;
    int idx2;
    struct cluster_t tmp_cluster;
    while (!done)
    {
        find_neighbours(clusters, cluster_count, &idx1, &idx2);
        merge_clusters(clusters + idx1, clusters + idx2);

        /* Swap */
        tmp_cluster = clusters[cluster_count - 1];
        clusters[cluster_count - 1] = clusters[idx2];
        clusters[idx2] = tmp_cluster;

        clear_cluster(clusters + cluster_count - 1);  // uvolneni pameti
        cluster_count--;
        done = cluster_count == target_cluster_no;
    }
    print_clusters(clusters, cluster_count);

    /* Uvolneni pameti */
    for (int i; i < cluster_count; i++)
    {
        clear_cluster(clusters + i);
    }
    free(clusters);
    
    return 0;
}
