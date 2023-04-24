-- |
-- Module      : Formula2BDD.BDD
-- Description : Nástroje pro práci s binárními rozhodovacími stromy
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module Formula2BDD.BDD (
    createBDD,
    reduceBDD,
    showBDD,
    showTable
    ) where

import Formula2BDD.Formula (Conjunction, DNF, Literal (Neg, Pos), Var,
                            isContradiction)
import Formula2BDD.Utils   (contains, difference, join, uniq)

data BDD = BDD {vars :: [Var], root :: Tree}

data Tree = Leaf {value :: Bool} | Node {low :: Tree, variable :: Var, high :: Tree}

instance Eq Tree where
    (Leaf a) == (Leaf b) = a == b
    (Node a0 v0 b0) == (Node a1 v1 b1) = a0 == a1 && v0 == v1 && b0 == b1
    _ == _ = False

-- | Přidání proměnné do binárního rozhodovacího stromu. Proměnná se přidá jako
--   nový uzel, který nahradí všechny listy stromu a jeho oba potomci budou
--   listy, které nahradil.
addVariableToBDD :: Var -> BDD -> BDD
addVariableToBDD var (BDD vars root) = BDD (vars ++ [var]) (addVar' var root) where
    addVar' :: Var -> Tree -> Tree
    addVar' v org@(Leaf b)     = Node org v org
    addVar' v (Node t0 val t1) = Node (addVar' v t0) val (addVar' v t1)

-- | Vytvoření binárního rozhodovacího stromu podle zadané formule v disjunktní
--   normální formě.
createBDD :: DNF -> BDD
createBDD = createBDD' (BDD [] (Leaf False)) where
    createBDD' :: BDD -> DNF -> BDD
    createBDD' bdd [] = bdd
    createBDD' org@(BDD vars root) (x:xs) = do
        let newBdd = foldr addVariableToBDD org (reverse $ uniq (map getVar x) `difference` vars)
        if isContradiction x then
            createBDD' newBdd xs
        else
            createBDD' (setBDD x newBdd) xs
    setBDD :: Conjunction -> BDD -> BDD
    setBDD conj (BDD vars root) = BDD vars (set conj root)
    getVar :: Literal -> Var
    getVar (Pos var) = var
    getVar (Neg var) = var
    set :: Conjunction -> Tree -> Tree
    set _ (Leaf v) = Leaf True
    set conj (Node t0 var t1)
        | conj `contains` Neg var = Node (set conj t0) var t1
        | conj `contains` Pos var = Node t0 var (set conj t1)
        | otherwise = Node (set conj t0) var (set conj t1)

-- | Reprezentace binárního rozhoodvacího stromu jako tabulka stavů ve formě
--   řetězce.
showTable :: BDD -> String
showTable (BDD vars root) = showHeader vars ++ join "\n" (showTable' root "") where
    showHeader :: [Var] -> String
    showHeader []   = "\n"
    showHeader vars = join " " (map show vars) ++ "\n"
    showTable' :: Tree -> String -> [String]
    showTable' (Leaf b) prefix
        | b     = [prefix ++ "1"]
        | not b = [prefix ++ "0"]
    showTable' (Node t0 var t1) prefix  = showTable' t0 (prefix++"0 ") ++ showTable' t1 (prefix++"1 ")

-- | Reprezentace binárního rozhodovacího stromu jako řetězec. Hrana 'low' je
--   reprezentována jako "->" a hrana 'high' jako "=>".
showBDD :: BDD -> String
showBDD (BDD _ root) = join "\n" (showBDD' root "" "->" "=>") where
    showBDD' :: Tree -> String -> String -> String -> [String]
    showBDD' (Leaf b) prefix _ _
        | b     = [prefix ++ "1"]
        | not b = [prefix ++ "0"]
    showBDD' (Node t0 var t1) prefix p0 p1 = showBDD' t0 (prefix++show var++p0) p0 p1 ++ showBDD' t1 (prefix++show var++p1) p0 p1

-- | Funkce redukuje zadaný binární rozhodovací strom.
reduceBDD :: BDD -> BDD
reduceBDD (BDD vars root) = BDD vars (reduceBDD' root) where
    reduceBDD' :: Tree -> Tree
    reduceBDD' org@(Leaf v) = org
    reduceBDD' (Node t0 var t1) = do
        let t0' = reduceBDD' t0
        let t1' = reduceBDD' t1
        if t0' == t1' then t0' else Node t0' var t1'
