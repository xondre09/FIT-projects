{-# OPTIONS_GHC -F -pgmF htfpp #-}
-- |
-- Module      : BDDTests
-- Description : Testy modulů 'Formula2BDD.BDD' a 'Formula2BDD.Data.BDD'.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module BDDTests where
import Test.Framework

import Formula2BDD
import Formula2BDD.Utils

xorBdd = createBDD [[Pos (Var "a"), Neg (Var "b")], [Neg (Var "a"), Pos (Var "b")]]
emptyBdd = createBDD []
emptyTestBdd = createBDD [[]]
favBdd = createBDD [[Pos (Var "a"), Pos (Var "b"), Pos (Var "c")], [Pos (Var "a"), Pos (Var "b"), Neg (Var "c")]]

test_xor = do
    assertEqual "a b\n0 0 0\n0 1 1\n1 0 1\n1 1 0" (showTable xorBdd)
    assertEqual "a->b->0\na->b=>1\na=>b->1\na=>b=>0" (showBDD xorBdd)
    assertEqual "a->b->0\na->b=>1\na=>b->1\na=>b=>0" (showBDD (reduceBDD xorBdd))

test_emptyFormula = do
    assertEqual "\n0" (showTable emptyBdd)
    assertEqual "0" (showBDD emptyBdd)
    assertEqual "0" (showBDD (reduceBDD emptyBdd))

test_emptyTerm = do
    assertEqual "\n1" (showTable emptyTestBdd)
    assertEqual "1" (showBDD emptyTestBdd)
    assertEqual "1" (showBDD (reduceBDD emptyTestBdd))

test_favExample = do
    assertEqual "a b c\n0 0 0 0\n0 0 1 0\n0 1 0 0\n0 1 1 0\n1 0 0 0\n1 0 1 0\n1 1 0 1\n1 1 1 1" (showTable favBdd)
    assertEqual "a->b->c->0\na->b->c=>0\na->b=>c->0\na->b=>c=>0\na=>b->c->0\na=>b->c=>0\na=>b=>c->1\na=>b=>c=>1" (showBDD favBdd)
    assertEqual "a->0\na=>b->0\na=>b=>1" (showBDD (reduceBDD favBdd))

test_splitComma = assertEqual (splitComma "a,b,c,d") ["a", "b", "c", "d"]
