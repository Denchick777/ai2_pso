//
// Created by Denis Chernikov on 12.11.2017.
//

#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include "pso.h"
#include "random.h"

ConfigPSO ConfigPSO_init() {
    ConfigPSO res = {NULL,  // Particle creator pointer
                     NULL,  // Fitness function pointer
                     0,  // Number of dimensions in a particle
                     100,  // Swarm size
                     100,  // Iterations threshold
                     1.0,  // Coefficient C0
                     2.0,  // Coefficient C1
                     3.0,  // Coefficient C2
                     1.0,  // Inertia initial value
                     0.0,  // Inertia lowest value threshold
                     0.999  // Inertia decreasing step
    };
    return res;
}

__Particle __Particle_init(int dimensions, double *initPosition) {
    int i;
    size_t new_size = sizeof(double) * dimensions;

    double *best = (double *) malloc(new_size);
    double *vel = (double *) malloc(new_size);
    for (i = 0; i < dimensions; ++i) {
        best[i] = initPosition[i];
        vel[i] = 0.0;
    }

    __Particle res = {initPosition, best, -1, vel};

    return res;
}

void __Particle_free(__Particle *p) {
    free(p->position);
    free(p->bestPosition);
    free(p->velocity);
}

double *executePSO(ConfigPSO *conf, _Bool printIters) {
    if (conf->particleCreator == NULL ||
        conf->fitnessFunction == NULL ||
        conf->dimensions < 1 ||
        conf->swarmSize < 1 ||
        conf->iterThreshold < 1 ||
        conf->c1 + conf->c2 <= 4 ||
        ((conf->inertInit > conf->inertThreshold) ^ (conf->inertStep >= 0))) {
        return NULL;
    }

    int i, j;
    double inert = conf->inertInit;
    double phi = conf->c1 + conf->c2;
    double normal_coef = 2.0 * conf->c0 / (fabs(2.0 - phi - sqrt(phi * phi - 4.0 * phi)));

    __Particle **swarm = (__Particle **) malloc(sizeof(__Particle *) * conf->swarmSize);
    for (i = 0; i < conf->swarmSize; ++i) {
        swarm[i] = (__Particle *) malloc(sizeof(__Particle));
    }
    __Particle *bestParticle = NULL;
    __Particle newParticle;

    for (i = 0; i < conf->swarmSize; ++i) {
        newParticle = __Particle_init(conf->dimensions, conf->particleCreator());
        newParticle.bestFitness = conf->fitnessFunction(newParticle.position);
        *swarm[i] = newParticle;
        if (bestParticle == NULL || bestParticle->bestFitness > newParticle.bestFitness) {
            bestParticle = swarm[i];
        }
    }

    int iterCounter = 0;
    double tmpFitness;
    __Particle *cur;

    while (iterCounter++ < conf->iterThreshold) {
        for (i = 0; i < conf->swarmSize; ++i) {
            if (swarm[i] != bestParticle) {
                cur = swarm[i];
                for (j = 0; j < conf->dimensions; ++j) {
                    cur->velocity[j] = (inert * cur->velocity[j] +
                                        conf->c1 * getRandomDouble() * (cur->bestPosition[j] - cur->position[j]) +
                                        conf->c2 * getRandomDouble() * (bestParticle->position[j] - cur->position[j])
                                       ) * normal_coef;
                    cur->position[j] = cur->position[j] + cur->velocity[j];
                }
                tmpFitness = conf->fitnessFunction(cur->position);
                if (tmpFitness < cur->bestFitness) {
                    cur->bestFitness = tmpFitness;
                    for (j = 0; j < conf->dimensions; ++j) {
                        cur->bestPosition[j] = cur->position[j];
                    }
                    if (tmpFitness < bestParticle->bestFitness) {
                        bestParticle = cur;
                    }
                }
            }
        }
        if (printIters) {
            printf("Iteration: %d; Best fitness: %.15f\n", iterCounter, bestParticle->bestFitness);
        }
        if (inert >= conf->inertThreshold) {
            inert *= conf->inertStep;
        }
    }

    for (i = 0; i < conf->dimensions; ++i) {  // TODO remove
        printf("best[%d] %.15f\n", i, bestParticle->bestPosition[i]);
    }

    double *res = (double *) malloc(sizeof(double) * conf->dimensions);
    for (i = 0; i < conf->dimensions; ++i) {
        res[i] = bestParticle->bestPosition[i];
    }

    for (i = 0; i < conf->swarmSize; ++i) {
        __Particle_free(swarm[i]);
        free(swarm[i]);
    }
    free(swarm);

    return res;
}
