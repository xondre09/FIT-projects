#ifndef FUNCTION_H_
#define FUNCTION_H_

/** @file   function.h
 *  @author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 *  @brief  Implementace funkcí výrazu.
 */

#include <random>
#include <vector>

typedef std::vector<int> truthTable_t;          /**< Pravdivostní tabulka. */
typedef std::vector<truthTable_t> context_t;

#include "expression.h"

class Expression;

/** Rozhraní funkce. */
class Function {
public:
    /** Vykonání funkce. Pokud má funkce njaké potomky, tak vykoná nejdřív je
     *  a na nich provede svoji operaci.
     *  
     *  @param expr[in]    výraz
     *  @param context[in] hodnoty proměnných
     *  @return pravdivostní tabulky
     */
    virtual truthTable_t eval(Expression* expr, context_t* context) = 0;

    /** Arita funkce.
     * 
     * @return arita funkce
     */
    virtual int arity();

    /** Jedná se o operátor?
     * 
     *  @return pokud je funkce operátor, tak vrací true, jinak false
     */
    virtual bool isOperator();

    /** Jedná se o proměnnou?
     * 
     *  @return pokud se jedná o proměnnou, tak vrací true, jinak false
     */
    virtual bool isVariable();

    /** Textová reprezentace funkce/proměnné.
     * 
     *  @return textová reprezentace funkce/proměnné
     */
    virtual std::string toString();

    /** Destruktor. */
    virtual ~Function() = default;
protected:
    std::string name_;  /**< jméno funkce */
    int arity_;         /**< arita funkce */
    bool isOperator_;   /**< je funkce oerátor? */
    bool isVariable_;   /**< je funkce proměnná? */
};


/** Proměnná. */
class Variable : public Function {
public:
    Variable(int index);
    ~Variable() = default;
    truthTable_t eval(Expression* expr, context_t* context);
private:
    int index;
};

class And : public Function {
public:
    And();
    ~And() = default;
    truthTable_t eval(Expression* expr, context_t* context);
};

class Nand : public Function {
public:
    Nand();
    ~Nand() = default;
    truthTable_t eval(Expression* expr, context_t* context);
};

class Or : public Function {
public:
    Or();
    ~Or() = default;
    truthTable_t eval(Expression* expr, context_t* context);

};

class Nor : public Function {
public:
    Nor();
    ~Nor() = default;
    truthTable_t eval(Expression* expr, context_t* context);
};

class Xor : public Function {
public:
    Xor();
    ~Xor() = default;
    truthTable_t eval(Expression* expr, context_t* context);
};

class Nxor : public Function {
public:
    Nxor();
    ~Nxor() = default;
    truthTable_t eval(Expression* expr, context_t* context);
};

class Functions {
public:
    Functions(int varsCount);
    Function* randomVariable();
    Function* randomOperation();

    ~Functions();
private:
    std::vector<Function*> variables;
    std::vector<Function*> operators;
};

#endif // FUNCTION_H
