/**
 * Author : Oleh Momot, xmomot00
 * Date : 24.11.2022
 * 
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
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
    
    // TODO
    void *temp = malloc(sizeof(struct obj_t) * cap); // Allocating memory for array of objects with capacity == cap
    assert(temp != NULL); // Error while allocating memory
    
    c->capacity = cap;
    c->size = 0;
    c->obj = (struct obj_t*)temp;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj); // If c is NULL, no operation is performed.
    c->obj = NULL;
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
    // TODO
    if (c->size == c->capacity)
    {
        c = resize_cluster(c, c->capacity + 1);
        // if error OCCURED c == NULL !!!!!
    }
    ((c->obj) + c->size)->id = obj.id;
    ((c->obj) + c->size)->x = obj.x;
    ((c->obj) + c->size)->y = obj.y;
    c->size++; 
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

    // TODO
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

    // TODO
    return -1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    float distance = sqrtf((o1->x - o2->x) * (o1->x - o2->x) + (o1->y - o2->y) * (o1->y - o2->y));
    return distance;
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

    // TODO
    // default cl_dist is distance between the first (index - 0) object of c1 and the first (index - 0) object of c2
    float cl_dist = obj_distance(c1->obj, c2->obj); 
    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; j++)
        {
            struct obj_t *o1 = (c1->obj + i);
            struct obj_t *o2 = (c2->obj + j);
            float distance = obj_distance(o1, o2);
            if (distance < cl_dist) // Comparing two float values!!!!
                cl_dist = distance;
        }
    }
    return cl_dist;
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
    
    // TODO

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

// Returns -1 is any error occured and NULL in arr, otherwise - the number of read objects
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *objects = fopen(filename, "r"); // opening input file
    if (objects == NULL) // Checking if any error occured while openning file
    {
        *arr = NULL;
        return -1;
    }

    int N; 
    fscanf(objects, "count=%d", &N); // Reading the number of objects from input file

    struct cluster_t *temp = malloc(sizeof(struct cluster_t) * N); // Allocating space for N clusters on heap
    if (temp == NULL) // Checking if any error occured while allocation
    {
        *arr = NULL;
        return -1;
    }
    *arr = temp; // Pointing a value in (*arr) on the array of clusters allocated on heap
    for (int i = 0; i < N; i++)
    {
        init_cluster(*arr + i, 1); // Initializing i-th cluster with capacity == 1
        struct obj_t temp_obj; // temporary object to read data from input file into
        fscanf(objects, "%d %f %f", &temp_obj.id, &temp_obj.x, &temp_obj.y); // Reading i-th object data from input file
        append_cluster(*arr + i, temp_obj); // Appending i-th object to i-th cluster
    }

    fclose(objects); // closing input file
    return N;
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
// argv[] = {./cluster, FILE, N(optional)}
int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    // TODO
    
    if (argc > 3 || argc < 2) // Wrong number of arguments of command line
    {
        fprintf(stderr, "Wrong number of arguments of command line!\n");
        return 1;
    }

    char *input_file = argv[1]; // argc will always be at least 2 => nothing to worry about
    // setting number of clusters to the default value
    int n_clusters = 1; 
    // Checking if the user set the optional argument
    if (argc == 3) 
        n_clusters = atoi(argv[2]);
    
    //Checking if entered number of clusters is valid
    if (n_clusters <= 0)
    {
        fprintf(stderr, "Wrong arguments od command line! Entered number of clusters is not valid!\n");
        return 1;
    }
    // Setting the default number of clusters to the number of objects in input file
    int default_n_clusters = load_clusters(input_file, &clusters);  
    print_clusters(clusters, default_n_clusters);
    
    for (int i = 0; i < default_n_clusters; i++)
    {
        for (int j = 0; j < default_n_clusters; j++)
        {
            printf("Distance between cluster [%d] and cluster [%d] is : %f\n", i, j, cluster_distance(clusters + i, clusters + j));
        }
    }
    









    // Freeing memory allocated for objects of each cluster
    for (int i = 0; i < default_n_clusters; i++)
    {
        clear_cluster(clusters+i);
    }
    //Freeing memory, allocated for array of clusters
    free(clusters);
    return 0;
}
