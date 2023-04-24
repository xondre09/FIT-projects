-- |
-- Module      : DNFTests
-- Description : Testy aplikace metodou 'Black Box'. Vstupní soubory v adresáři
--             : 'data/tests'.
-- Copyright   : (c) Karel Ondřej (xondre09), 2019
-- Maintainer  : xondre09@stud.fit.vutbr.cz
-- License     : MIT
module IntegrationTests (integrationTests) where

import Test.Framework
import Test.Framework.BlackBoxTest

config = BBTArgs { bbtArgs_stdinSuffix    = ".in"
                 , bbtArgs_stdoutSuffix   = ".out"
                 , bbtArgs_stderrSuffix   = ".err"
                 , bbtArgs_dynArgsName    = "BBTArgs"
                 , bbtArgs_stdoutDiff     = defaultDiff
                 , bbtArgs_stderrDiff     = defaultDiff
                 , bbtArgs_verbose        = True }

integrationTests = blackBoxTests "data/tests" "./formula-2-bdd" ".in" config
