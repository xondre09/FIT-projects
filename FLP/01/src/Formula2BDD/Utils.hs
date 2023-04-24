-- |
-- Module      : Formula2BDD.Utils
-- Description : Pomocné nástroje.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module Formula2BDD.Utils (
    contains,
    uniq,
    join,
    splitComma,
    difference
    ) where

-- | Funkce pro zjištění, zda se daná položka nachází v seznamu. Použití funkce
--   elem přehazovalo pořadí proměnných.
contains :: Eq a => [a] -> a -> Bool
contains xs y = foldr (\ x -> (||) (x == y)) False xs

-- | Vytváří seznam obsahující pouze unikátní položky ve stejném pořadí, jak se
--   nacházejí ve vstupním seznamu.
uniq :: Eq a => [a] -> [a]
uniq = uniq' [] where
    uniq' :: Eq a => [a] -> [a] -> [a]
    uniq' xss [] = []
    uniq' xss (x:xs)
        | xss `contains` x = uniq' xss xs
        | otherwise = x : uniq' (x:xss) xs

-- | Operace ... nad seznami.
difference :: Eq a => [a] -> [a] -> [a]
difference [] _ = []
difference (x:xs) ys
    | ys `contains` x = difference xs ys
    | otherwise = x : difference xs ys

-- | Spojení položek v seznamu do jednoho řetězce, kde jsou jednotlivé položky
--   odděleny zadaným znakem.
join :: String -> [String] -> String
join d []     = ""
join d [x]    = x
join d (x:xs) = x ++ d ++ join d xs

-- | Rozdělí řetězec na seznam podřetězců, kde jako odělovací symbol slouží znak
--   čárky.
splitComma :: String -> [String]
splitComma "" = [""]
splitComma x = do
    let item = takeWhile (/=',') x
    if x == item then
        [item]
    else
        item : splitComma (drop (length item + 1) x)
