-- |
-- Module      : Main
-- Description : Aplikace pro převod formulí v distunktní normální formě na
--               binární rozhodovací stromy.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module Main where

import System.Environment (getArgs)

import Formula2BDD        (createBDD, parseDNF, reduceBDD, showBDD, showDNF,
                           showTable)

-- | Hlavní program. Aplikace provede zpracování argumentů příkazové řádky a
--   následně zpracuje formuli v disjunktní normální formě ze souboru nebo
--   standardního vstupu. Výstupem programu na standartní výstup může být:
--     - Zpracovaná formule ve stejném formátu jako na vstupu.
--     - Tabulka stavů zadané formule.
--     - Binární rozhodovací strom zadané formule.
--     - Redukovaný binární strom zadané formule.
main :: IO()
main = do
    args <- getArgs
    let (switch, file) = procArgs args
    content <- if file == stdin then getContents else readFile file
    let dnf = parseDNF content
    if switch == "-i" then
        putStrLn $ showDNF dnf
    else do
        let bdd = createBDD dnf
        case switch of
             "-t" -> putStrLn $ showTable bdd
             "-b" -> putStrLn $ showBDD bdd
             "-r" -> putStrLn . showBDD $ reduceBDD bdd

-- | Označení standartního vstupu v argumentu příkazové řádky (prázdnýý
--   řetězec).
stdin :: String
stdin = ""

-- | Zpracování argumentů příkazové řádky.
procArgs :: [String] -> (String, String)
procArgs [] = error "Usage: ?"
procArgs [x] = procArgs (x:[""])
procArgs [x,y]
    | x == "-i" = (x, y)
    | x == "-t" = (x, y)
    | x == "-b" = (x, y)
    | x == "-r" = (x, y)
    | otherwise = error $ show x
