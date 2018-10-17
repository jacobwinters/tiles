import Data.List

size = SIZE
states = [0.. STATES-1]
baseMatrices =
#ifdef brick
  allBrickMatrices
#else
  allMatrices
#endif

allCombinations :: [[a]] -> [[a]]
allCombinations [] = [[]]
allCombinations (x:rest) = (:) <$> x <*> allCombinations rest

allColumns :: [[Int]]
allColumns = allCombinations $ take size $ repeat states

allMatrices :: [[[Int]]]
allMatrices = allCombinations $ take size $ repeat allColumns

-- This only makes sense when size is even
allBrickMatrices :: [[[Int]]]
allBrickMatrices = map makeBrick $ allCombinations $ take (size `quot` 2) $ repeat allColumns where
  makeBrick halfMatrix = halfMatrix ++ map (rotateList $ size `quot` 2) halfMatrix

rotateList :: Int -> [a] -> [a]
rotateList n list = drop n list ++ take n list

rotateRows :: [[a]] -> [[a]]
rotateRows = map rotateRow where
  rotateRow (first:rest) = rest ++ [first]
  rotateRow [] = []

rotateColumns :: [[a]] -> [[a]]
rotateColumns = transpose . rotateRows . transpose

reflect :: [[a]] -> [[a]]
reflect = map reverse

rotate :: [[a]] -> [[a]]
rotate = reflect . transpose

permuteStates :: [[Int]] -> [[[Int]]]
permuteStates matrix = map (permuteStatesInMatrix matrix) (permutations states)

permuteStatesInMatrix :: [[Int]] -> [Int] -> [[Int]]
permuteStatesInMatrix matrix permutation = map (map (permutation !!)) matrix

allVariations :: [[Int]] -> [[[Int]]]
allVariations matrix = do
  rotations <- take 4 $ iterate rotate matrix
--  The other transformations end up producing all variations of the patterns, so we don't need transpositions
--  transpositions <- [rotations, transpose rotations]
--  reflections <- [transpositions, reflect transpositions]
  reflections <- [rotations, reflect rotations]
  rowRotations <- take size $ iterate rotateRows reflections
  columnRotations <- take size $ iterate rotateColumns rowRotations
  statePermutations <- permuteStates columnRotations
  return statePermutations

filteredMatrices :: [[[Int]]]
filteredMatrices = nub $ map (maximum . allVariations) baseMatrices

result :: [(Int, [[Int]])]
result = map matrixResult filteredMatrices where
  matrixResult matrix = (length $ nub $ allVariations matrix, matrix)

-- Assumes 1 digit integers
matrixToString :: [[Int]] -> String
matrixToString = concatMap $ concatMap show

resultToString :: (Int, [[Int]]) -> String
resultToString (variations, matrix) = show variations ++ " " ++ matrixToString matrix

main = mapM (putStrLn . resultToString) result
