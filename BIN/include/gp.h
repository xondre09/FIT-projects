#ifndef GP_H_
#define GP_H_

/** @file   gp.h
 *  @author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 *  @brief  Algoritmus genetického programování.
 */

#include <random>       // std::rand
#include <iostream>
#include <ctime>        // std::time

#include "expression.h"
#include "individual.h"

#ifdef DEBUG
#define DEBUG_STDERR(x) (std::cerr << x << std::endl)
#define DEBUG_STDOUT(x) (std::cout << x << std::endl)
#else
#define DEBUG_STDERR(x)
#define DEBUG_STDOUT(x)
#endif

class GeneticPrograming {
public:
    /** Algoritmus genetického programování.
     * 
     *  @param minDeep[in]    minimální hloubka stromů
     *  @param maxDeep[in]    maximální hloubka stromů
     *  @param nVariables[in] počet proměnných
     */
    GeneticPrograming(int minDeep, int maxDeep, int nVariables);

    /** Ohodnocení jedince. Kriteriem ohodnocení je nelinearita funkce.
     * 
     *  @param individum jedince k ohodnoceni
     */
    Individual* run(int popSize, int terminateEvaluate);

    int minDeep;           /**< Minimální hloubka stromu. */
    int maxDeep;           /**< Maximální hloubka stromu. */
    int terminateFitness;  /**< Maximální hodnota fitness funkce. */
private:
    int varsCount;         /**< Počet proměnných hledané funkce. */
    Functions functions;   /**< Množina funkcí, ze kterých se generuje výraz. */
    context_t context;     /**< Permutace všech možných kombinací proměnných
                                funkce. */

    /** Ohodnocení jedince. Kriteriem ohodnocení je nelinearita funkce.
     * 
     *  @param individum jedince k ohodnoceni
     */
    void fitness(Individual* individum);

    /** Výběr jedince z populace. Využit algoritmus turnaje s výběrem 3 jedinců 
     *  a vyhrává nejlepší z nich.
     * 
     *  @param population[in] populace ze ktere se vybere jedinec
     *  @return vyherce rurnaje
     */
    Individual* select(std::vector<Individual*> &population);

    /** Mutace typu 'point'. Zvolí se náhodně jeden uzel a náhodně se změní 
     *  funkce.
     * 
     *  @param individual[in/out] jedinec z populace, který zmutuje
     */
    void pointMutate(Individual* individum);

    /** Mutace typu 'branch'. Náhodně se zvolí podstrom, který je nahrazen nově
     *  vygenerovaným stromem. Zachovává se maximální hloubka stromu.
     * 
     *  @param individual[in/out] jedinec z populace, který zmutuje
     */
    void branchMutate(Individual* individum);
    
    /** Mutace jedince. Náhodně se zvolí jedna z: 'point', 'branch'.
     * 
     *  @param individual[in/out] jedinec z populace, který zmutuje
     */
    void mutate(Individual* individum);

    /** Křížení dvou jedinců. Nahodne se zvoli dva podstromy s kořenovým uzlem
     *  ve stejné úrovni původního stromu. Podstromy se následně prohodí a tím 
     *  je zachována podmínka maximální úrovně stromu.
     * 
     *  @param parentA[in/out] jedinec z populace
     *  @param parentB[in/out] jedince z populace
     */
    void cross(Individual* parentA, Individual* parentB);

    /** Generování stromu, který je úplně vyvážený.
     * 
     *  @param deep[in] hloubka vygenerovaného výrazu
     *  @return vygenerovaný výraz
     */
    Expression* generateFullExpression(int deep);

    /** Vygenerování stromu, který není vyvážený.
     * 
     *  @param maxDeep[in] maximální hloubka stromu
     *  @param minDeep[in] minimální hloubka stromu
     *  @return vygenerovaný výraz
     */
    Expression* generateGrowExpression(int maxDeep, int minDeep);

    /** Generování populace o zadané velikosti. Polovina jedinců se generuje 
     *  metodou 'full' a druhá metodou 'grow'.
     * 
     *  @param population[out] vygenerovaná populace
     *  @param popSize[in]     velikost populace
     */ 
    void generatePopulation(std::vector<Individual*> &population, int popSize);
};

#endif // GP_H_
