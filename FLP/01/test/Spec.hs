{-# OPTIONS_GHC -F -pgmF htfpp #-}
-- |
-- Description : Testy modulů 'Formula2BDD.DNF' a 'Formula2BDD.Data.DNF'.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
import Test.Framework
import Test.Framework.BlackBoxTest

import {-@ HTF_TESTS @-} DNFTests
import {-@ HTF_TESTS @-} BDDTests
import IntegrationTests

-- import ParseDNFTest
{-
eqVarTest = do
    assertBool "x == x" (Var "x" == Var "x")
    assertBool "x == x" (Pos (Var "x") == Pos (Var "x"))

splitOnTest = do
    assertEqual "a,b,c,d" ["a", "b", "c", "d"] (splitComma "a,b,c,d")
-}

main :: IO()
main = do
    bbts <- integrationTests
    htfMain(htf_importedTests ++ [makeTestSuite "Black Box Tests" bbts])
