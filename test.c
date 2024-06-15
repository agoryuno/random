#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "sample_wor.h"

int main() {
    srand(time(NULL));

    double probs[10] = {0.1, 0.05, 0.05, 0.1, 0.2, 0.08, 0.02, 0.1, 0.15, 0.15};

    double sum_prob = 0.0;
    for (int i=0; i<10; i++) {
        sum_prob += probs[i];
    }
    assert(sum_prob == 1.0);

    SamplerState* sampler = get_sampler(probs, 10);

    #ifdef DEBUG_PRINT
        printf("main(): received sampler, root label = %d\n", sampler->root->label);
    #endif

    for (int i=0; i<=11; i++) {
        printf("sampled: %d\n", sample_wor(sampler));
    }

    assert(sum_probs(sampler) == 1.0);

    for (int i = 0; i < sampler->N; i++) {
        printf("%d: ", i);
        printf("%d | WT = %f, G = %f \n", sampler->nodes[i]->label,
            sampler->nodes[i]->WT,
            sampler->nodes[i]->G);
    }
    destroy_sampler(sampler);

    sampler = get_sampler(probs, 10);
    for (int i=0; i<10; i++) {
        printf("sampled: %d\n", sample_wor(sampler));
    }
    return 0;
}