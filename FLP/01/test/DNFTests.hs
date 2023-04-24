{-# OPTIONS_GHC -F -pgmF htfpp #-}
-- |
-- Module      : DNFTests
-- Description : Testy modulů 'Formula2BDD.Formula' a 'Formula2BDD.Data.Formula'.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module DNFTests where

import Test.Framework

import Formula2BDD

test_parseVariable = assertEqual (parseVar "a") (Var "a")

test_parseLiteral = do
    assertEqual (parseLiteral "a") (Pos (Var "a"))
    assertEqual (parseLiteral "-b") (Neg (Var "b"))

test_parseConjuction = do
    assertEqual (parseConjunction "()") []
    assertEqual (parseConjunction "(a,b,c)") [Pos (Var "a"), Pos (Var "b"), Pos (Var "c")]

test_parseDNF = do
    assertEqual (parseDNF "") []
    assertEqual (parseDNF "()") [[]]
    assertEqual (parseDNF "(f,g,-d)") [[Pos $ Var "f", Pos $ Var "g", Neg $ Var "d"]]
    assertEqual (parseDNF "(a,-b)\n(-a,b)") [[Pos $ Var "a", Neg $ Var "b"], [Neg $ Var "a", Pos $ Var "b"]]

test_eqVariable = do
    assertEqual (Var "x" == Var "x") True
    assertEqual (Var "x" == Var "b") False

test_eqLiteral = do
    assertEqual (Pos (Var "x") == Pos (Var "x")) True
    assertEqual (Neg (Var "x") == Pos (Var "x")) False
