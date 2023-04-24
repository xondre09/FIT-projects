{-# OPTIONS_GHC -F -pgmF htfpp #-}
-- |
-- Description : Testy modulů 'Formula2BDD.DNF' a 'Formula2BDD.Data.DNF'.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
import Test.Framework
import Test.Framework.BlackBoxTest

config = BBTArgs { bbtArgs_stdinSuffix    = ".in"
                 , bbtArgs_stdoutSuffix   = ".out"
                 , bbtArgs_stderrSuffix   = ".err"
                 , bbtArgs_dynArgsName    = "BBTArgs"
                 , bbtArgs_stdoutDiff     = defaultDiff
                 , bbtArgs_stderrDiff     = defaultDiff
                 , bbtArgs_verbose        = True }

integrationTests = blackBoxTests "data/test" "./flp19-log" ".in" config

main :: IO()
main = do
    btts <- integrationTests;
    htfMain([makeTestSuite "Black Box Tests" btts])
