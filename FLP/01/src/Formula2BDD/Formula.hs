-- |
-- Module      : Formula2BDD.Formula
-- Description : Nástroje pro práci s formulemi v disjunktní normální formě.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module Formula2BDD.Formula (
    Var(..),
    Literal(..),
    Conjunction,
    DNF,
    isContradiction,
    removeContradiction,
    showDNF,
    parseVar,
    parseLiteral,
    parseConjunction,
    parseDNF
    ) where

import Data.Char         (isAsciiLower)
import Formula2BDD.Utils (contains, join, splitComma)

-- | Proměnná ve výrokových formulích.
-- Uchovává se název proměnné.
newtype Var = Var String
-- | Datový typ reprezentující literál.
data Literal = Pos Var | Neg Var
-- | Konjunkce literálů.
type Conjunction = [Literal]
-- | Disjunktní normální forma.
type DNF = [Conjunction]

instance Show Var where
    show (Var v) = v
instance Show Literal where
    show (Pos var) = show var
    show (Neg var) = '-' : show var
    showList xs = showChar '(' . showLiteralList xs . showChar ')'
instance Eq Var where
    (Var v0) == (Var v1) = v0 == v1
instance Eq Literal where
    (Pos v0) == (Pos v1) = v0 == v1
    (Neg v0) == (Neg v1) = v0 == v1
    _ == _ = False

showLiteralList :: [Literal] -> ShowS
showLiteralList [] s = s
showLiteralList [x] s = show x ++ s
showLiteralList (x:xs) s = show x ++ "," ++ showLiteralList xs s

-- | Funkce slouží k určení, zda daná konjunkce je kontradikce.
isContradiction :: Conjunction -> Bool
isContradiction [] = False
isContradiction (x:xs) = case x of
    Pos a -> contains xs (Neg a) || isContradiction xs
    Neg a -> contains xs (Pos a) || isContradiction xs

-- | Odstranění všech kontradikcí konjunkcí z DNF.
removeContradiction :: DNF -> DNF
removeContradiction [] = []
removeContradiction (x:xs)
    | isContradiction x = removeContradiction xs
    | otherwise = x : removeContradiction xs

-- | Převedení formule na řetězec.
showDNF :: DNF -> String
showDNF dnf = join "\n" (map show dnf)

-- | Zpracování proměnné z řetězce.
parseVar :: String -> Var
parseVar "" = error "Expected variable in [a-z]"
parseVar x
    | length x /= 1 || not (isAsciiLower (head x)) = error "Expected variable in [a-z]"
    | otherwise = Var x

-- | Zpracování literálu z řetězce.
parseLiteral :: String -> Literal
parseLiteral "" = error "Expected literal"
parseLiteral (x:xs)
    | x == '-' = Neg (parseVar xs)
    | otherwise = Pos (parseVar $ x:xs)

-- | Zpracování konjunkce literálu ve tvaru (a,b,c) ze zadaného řetězce.
parseConjunction :: String -> Conjunction
parseConjunction "" = error "Expected conjunction"
parseConjunction (x:xs)
    | null xs || not (x == '(' && last xs == ')') = error "Expected conjunction"
    | null $ init xs = []
    | otherwise = map parseLiteral (splitComma (init xs))

-- | Zpracování DNF ze zadaného řetězce. Jednotlivé konjunkce jsou odděleny
--   znakem konce řádku.
parseDNF :: String -> DNF
parseDNF str = map parseConjunction (lines str)
