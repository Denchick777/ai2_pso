//
// Created by Denis Chernikov on 12.11.2017.
//

#ifndef AI2_PSO_H
#define AI2_PSO_H

#include <stddef.h>

/**
 * Represents a set of arguments for PSO algorithm execution.
 */
typedef struct ConfigPSO {
    /**
     * Create a new particle instance for PSO algorithm.
     * NOTE: You're advised to use 'free()' function after returned particle will be used.
     *
     * @return New particle
     */
    double *(*particleCreator)();

    /**
     * Count fitness value for the given particle assuming given arguments.
     * NOTE: less fitness number - better particle is.
     *
     * @param part Particle to count fitness for
     * @return Fitness value for the given particle
     */
    double (*fitnessFunction)(double *part);

    /**
     * Amount of dimensions of a single particle.
     */
    int dimensions;

    /**
     * Size of the set of randomly generated particles.
     */
    int swarmSize;

    /**
     * Maximum amount of iterations (moves of each particle).
     */
    int iterThreshold;

    /**
     * C0 coefficient - current velocity ratio (used in coefficient normalisation)
     */
    double c0;

    /**
     * C1 coefficient - Personal (cognitive) acceleration coefficient
     */
    double c1;

    /**
     * C2 coefficient - Global (social) acceleration coefficient
     */
    double c2;

    /**
     * Inertia initial value (better in range (0, 1].
     */
    double inertInit;

    /**
     * Inertia minimal value (can't go lower; better in range [0, 1)).
     */
    double inertThreshold;

    /**
     * Step of inertia decreasing (curInertia * inertiaStep = newInertia).
     */
    double inertStep;
} ConfigPSO;

/**
 * Initialise an object of type ConfigPSO - set of arguments for PSO algorithm.
 *
 * @return ConfigPSO
 */
ConfigPSO ConfigPSO_init();

/**
 * Represents a particle object for PSO algorithm.
 */
typedef struct __Particle {
    /**
     * Current particle's position.
     */
    double *position;

    /**
     * Best ever particle's own position (goes together with 'bestFitness').
     */
    double *bestPosition;

    /**
     * Best ever particle's own fitness value (goes together with 'bestPosition').
     */
    double bestFitness;

    /**
     * Last particle movement's velocity for each dimension.
     */
    double *velocity;
} __Particle;

/**
 * Initialise an object of type __Particle - set of properties of a particle for PSO algorithm.
 * NOTE: You're advised to use '__Particle_free()' function after this particle will be used.
 *
 * @param initPosition Initial particle's position
 * @return __Particle
 */
__Particle __Particle_init(int dimensions, double *initPosition);

/**
 * Free all allocated memory under this particle.
 *
 * @param p Particle
 */
void __Particle_free(__Particle *p);

/**
 * Generate the best particle out of swarm using PSO algorithm.
 * NOTE: You're advised to use 'free()' function after returned particle will be used.
 *
 * NOTES:
 * particle creator and fitness function should be defined;
 * amount of dimensions should be positive;
 * swarm size should be positive;
 * iterations threshold should be positive;
 * 'funcMax' should be greater than 'funcMin';
 * sum of C1 and C2 should be greater than 4 (according to the normalisation formula);
 * combination of inertia arguments should provide a finite number of inertia changing steps.
 *
 * @param conf Set of parameters for PSO algorithm (ConfigPSO object)
 * @param printIters Do you want to print out info about each iteration's best fitness?
 * @return Array with values of the best particle, NULL if wrong configuration argument(s).
 */
double *executePSO(ConfigPSO *conf, _Bool printIters);

#endif //AI2_PSO_H
