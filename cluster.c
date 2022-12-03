/**
 * Author : Oleh Momot, xmomot00
 * Date : 03.12.2022
 * 
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */

/**
 * Errors:
 * 3) count=''
 * 4) asserts do not work with NDEBUG defined
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
    
    // If any error while allocating memory occured -> c == NULL
    
    
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

/*
 Frees memory allocated for an array of objects of each of 'n_clust' clusters 
 Frees memory allocated for an array of clusters
 Sets a pointer to array of clusters to NULL
*/
void clusters_dtor(struct cluster_t **c, int n_clust)
{
    // Freeing memory allocated for objects of each cluster
    for (int i = 0; i < n_clust; i++)
    {
        clear_cluster(*c + i);
        debug("Freeing cluster ");
        dint(i);
    }
    free(*c);
    *c = NULL;
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
//void append_cluster(struct cluster_t *c, struct obj_t obj)
// returns 0 if any error occures, otherwise - returns 1
int append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if (c->size == c->capacity)
    {
        // c = resize_cluster(c, c->capacity + 1);
        c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
        if (c == NULL)
        {
            fprintf(stderr, "Error while reallocating memory in append_cluster() occured!\n");
            return 0;
        }
    }
    ((c->obj) + c->size)->id = obj.id;
    ((c->obj) + c->size)->x = obj.x;
    ((c->obj) + c->size)->y = obj.y;
    c->size++; 
    return 1;
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
//void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
//returns 0 if any error occures otherwise - return 1
int merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    if (c1->capacity < c1->size + c2->size)
    {
        if ((c1 = resize_cluster(c1, c1->size + c2->size)) == NULL)
        {
            fprintf(stderr, "Error while reallocating memory in merge_clusters() occured!\n");
            return 0;
        }
    }

    for (int i = 0; i < c2->size; i++)
    {
        struct obj_t temp_obj = *(c2->obj + i);
        
        if (!append_cluster(c1, temp_obj))
        {
            fprintf(stderr, "Error while appending object to cluster in merge_clusters() occured!\n");
            return 0;
        }
    }
    sort_cluster(c1);
    return 1;
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
    clear_cluster(carr+idx);
    for (int i = idx; i < narr - 1; i++)
    {
        (carr+i)->capacity = (carr+i+1)->capacity;
        (carr+i)->size = (carr+i+1)->size;
        (carr+i)->obj = (carr+i+1)->obj;
    }
    return narr-1;
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
    float min_dist = cluster_distance(carr, carr+1);
    *c1 = 0;
    *c2 = 1;
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            float new_dist = cluster_distance(carr+i, carr+j);
            if (new_dist < min_dist)
            {
                min_dist = new_dist;
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

// Checkes if each id in array is unique. Returns 1 - if are unique, otherwise - 0
int check_uniqueness(int *ids, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            if (ids[i] == ids[j])
                return 0;
        }
    }
    return 1;
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
    
    // Initializing both integer and float variables for count of objects in input file
    int N = 0;
    float temp_N;
    
    // Opening input file
    FILE *objects = fopen(filename, "r"); 
    
    // Checking if any error occured while openning file
    if (objects == NULL) 
    {
        fprintf(stderr, "Error while openning file occured!\n");
        *arr = NULL;
        goto err_return_N;
    }

    // Reading the count of objects from input file
    int read = fscanf(objects, "count=%f\n", &temp_N); 
    
    // Checking if any error occured while reading count from input file
    if (!read) // read == 1 if count was read, otherwise is 0
    {
        fprintf(stderr, "Error while reading count occured!\n");
        *arr = NULL;
        goto err_close_file; // input file is open => there is a need to close it
    }

    // Checking if read count is an integer value
    if (trunc(temp_N) != temp_N)
    {
        fprintf(stderr, "Count of objects in input file is not an integer value!\n");
        *arr = NULL;
        goto err_close_file; // input file is open => there is a need to close it
    }

    N = (int) temp_N;
    
    // Checking if read count is valid
    if (N <= 0)
    {
        fprintf(stderr, "Enterd cound of objects in input file is an invalid integer!\n");
        *arr = NULL;
        goto err_close_file; // input file is open => there is a need to close it
    }

    // Allocating memory for array of id-s to check theirs uniqueness
    int *ids = (int *) malloc(sizeof(int) * N); 

    // Checking if any error occured while allocating memory for an array of id-s
    if (ids == NULL)
    {
        fprintf(stderr, "Error while allocating memory for ids array occured!\n");
        *arr = NULL;
        goto err_close_file; // there is a need to close an input file, but no need to free the memory
    }

    // Allocating memory for N clusters on heap
    struct cluster_t *temp = (struct cluster_t *)malloc(sizeof(struct cluster_t) * N); 
    
    // Checking if any error occured while allocation memory for an array of clusters
    if (temp == NULL) 
    {
        fprintf(stderr, "Error while allocating memory for clusters occured!\n");
        *arr = NULL;
        goto err_free_ids; // memory for array of id-s wass allocated => freeing it
    }
    
    // Pointing a value in (*arr) on the array of clusters allocated on heap
    *arr = temp; 
    
    // Allocating memory for objects of N clusters
    for (int i = 0; i < N; i++)
    {
        // Initialization of i-th cluster 
        init_cluster(*arr + i, CLUSTER_CHUNK);
        
        // Checking if any error while initialization occured
        if ((*arr+i)->obj == NULL)
        {
            fprintf(stderr, "Error while initialization %d-th cluster occured!\n", i);
            
            // Freeing memory allocated for an array of objects of each of i clusters
            clusters_dtor(arr, i);
            
            // Freeing memory allocated for an array of clusters
            //free(*arr);
            
            //*arr = NULL;
            goto err_free_ids;
            // free(ids);
            // fclose(objects);
            // exit(1);
        }
    }

    // Reading objects from input file
    for (int i = 0; i < N; i++)
    {
        struct obj_t temp_obj; // temporary object to append into cluster 
        float id, x, y;
        int read_obj = fscanf(objects, "%f %f %f\n", &id, &x, &y); // Reading i-th object data from input file

        //Checking number of read values
        if (read_obj != 3)
        {
            fprintf(stderr, "Invalid number of data on one line in input file!\n");
            clusters_dtor(arr, N);
            //free(*arr);
            //*arr = NULL;
            goto err_free_ids;
        }

        // Checking if object id is an integer value
        if (trunc(id) != id) 
        {
            fprintf(stderr, "%d-th Object id is not integer!\n", i);
            clusters_dtor(arr, N);
            //free(*arr);
            //*arr = NULL;
            goto err_free_ids;
        }

        //Checking if object coordinates are integer values
        if (trunc(x) != x || trunc(y) != y)
        {
            fprintf(stderr, "Object coordinates are not integer!\n");
            clusters_dtor(arr, N);
            //free(*arr);
            //*arr = NULL;
            goto err_free_ids;
        }

        // Checking if coordinates of an object are not out of range
        if (x > 1000 || x < 0 || y > 1000 || y < 0) 
        {
            fprintf(stderr, "Coordinates of an object are out of range!\n");
            clusters_dtor(arr, N);
            //free(*arr);
            //*arr = NULL;
            goto err_free_ids;
        }

        temp_obj.id = (int) id;
        temp_obj.x = x;
        temp_obj.y = y;
        
        ids[i] = (int) id;
        
        // Appending i-th object to i-th cluster and checking if everything was OK
        if (!append_cluster(*arr + i, temp_obj))
        {
            fprintf(stderr, "Error while adding object into cluster in load_clusters() occured!\n");
            clusters_dtor(arr, N);
            //free(*arr);
            //*arr = NULL;
            goto err_free_ids;
        }
    }
    
    // Checking if each identifiers is unique
    if (!check_uniqueness(ids, N))
    {
        fprintf(stderr, "Ids in input file are not unique!\n");
        clusters_dtor(arr, N);
        //free(*arr);
        // *arr = NULL;
        goto err_free_ids;
    }
    err_free_ids:
    free(ids); // freeing memory allocated for an array of id-s
    debug("Freeing id-s\n");
    err_close_file:
    fclose(objects); // closing input file
    debug("Closing input file\n");
    err_return_N:
    debug("Returning N\n");
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
        exit(1);
    }

    char *input_file = argv[1]; // argc will always be at least 2 => nothing to worry about
    // setting number of clusters to the default value
    int n_clusters = 1; 
    // Checking if the user set the optional argument
    if (argc == 3) 
    {
        if (atoi(argv[2]) != atof(argv[2]))
        {
            fprintf(stderr, "Wrong arguments of command line! Entered number of clusters is not an integer value!\n");
            exit(1);
        }
        n_clusters = atoi(argv[2]);
    }
        
    
    //Checking if entered number of clusters is valid
    if (n_clusters <= 0)
    {
        fprintf(stderr, "Wrong arguments od command line! Entered number of clusters is not valid!\n");
        exit(1);
    }
    
    // Setting the default number of clusters to the number of objects in input file
    int default_n_clusters = 0;
    default_n_clusters = load_clusters(input_file, &clusters);  //memory allocation and clusters loading
    
    dint(default_n_clusters);
 
 
    // Error while loading clusters occured - major problem!
    if (clusters == NULL)
    {
        fprintf(stderr, "Error while loading clusters occured!\n");
        return 1;
    }

    if (n_clusters > default_n_clusters)
    {
        fprintf(stderr, "Invalid number of clusters. You can not make more clusters by uniting them\n");
        clusters_dtor(&clusters, default_n_clusters);
        //free(clusters);
        exit(1);
    }
    
    
    
    for (int number_clusters = default_n_clusters; number_clusters > n_clusters; number_clusters--)
    {
        int i, j;
        find_neighbours(clusters, number_clusters, &i, &j);
        if (!merge_clusters(clusters + i, clusters + j))
        {
            fprintf(stderr, "Error while merging clusters in main occured!\n");
            clusters_dtor(&clusters, number_clusters);
            //free(clusters);
            return 1;
        }
        remove_cluster(clusters, number_clusters, j);
    }
    print_clusters(clusters, n_clusters);






    
    
    clusters_dtor(&clusters, n_clusters);
    //free(clusters);
    return 0;
}
