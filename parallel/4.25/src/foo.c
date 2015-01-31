#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

struct gas
{
    sem_t       e,
                o,
                * h;

    unsigned    dox;
    size_t      l;
};

static
bool gas_init(struct gas * g, size_t n)
{
    sem_init(&g->e, 0, 1);
    sem_init(&g->o, 0, 0);

    g->h = calloc(n, sizeof(sem_t));

    for ( size_t i = 0;   i < n;   ++i )
    {
        sem_init(&g->h[i], 0, 0);
    }

    g->l = 0u;
}

static
void gas_h_ready(struct gas * g, unsigned id)
{
    sem_wait(&g->e);

    // put id into the list

    g->l++;

    if ( g->dox != 0 )
    {
        g->dox--;
        sem_post(&g->o);
    }
    else
    {
        sem_post(&g->e);
    }

    sem_wait(&g->h[id]);
}

static
void gas_o_ready(struct gas * g)
{
    size_t      d = 0u;

    while ( d < 2u )
    {
        sem_wait(&g->e);

        if ( g->l == 0u )
        {
            g->dox++;

            sem_post(&g->e);
            sem_wait(&g->o);
        }

        // Fetch a hydrogen id from the list

        g->l--;
        d++;

        sem_post(&g->e);
    }

    //sem_post(&g->h[left hydrogen]);
    //sem_post(&g->h[right hydrogen]);
}

extern
int main(int argc, char * argv[])
{
    puts(PACKAGE_STRING);
    return 0;
}
