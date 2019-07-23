# Tile pattern enumeration and counting... thingy
This code was built to answer the question, "how many distinct patterns can you make by tiling a 4x4 monochrome bitmap?" Since then it's grown a few more options. There are two implementations, one in C and one in Haskell. They produce identical results. The C implementation is significantly faster.

You can [see the results in your browser](#results) or [download the raw data](https://gitlab.com/jacobwinters/tiles/-/jobs/artifacts/master/download?job=run).

## What do you mean by "distinct patterns?"
An image with one black pixel in the bottom right is distinct from an image with one black pixel in the top left, but they produce identical results when tiled:

<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2/1111111111111110.png">

In general, if one image is a translation of another, the two images will produce identical tilings.

The first versions of this program only considered images that were translations of each other to be equivalent, but this produced too many patterns to look through. The current version considers images that can be turned into each other by some sequence of translations, 90° rotations, transpositions, reflections, and color permutations to produce equivalent tiled patterns. This reduces the number of pattern equivalence classes enough for us to look through examples of all of them.


## Options
The program can list tilings of square bitmaps of any size with either 2 or 3 colors. (The Haskell version supports any number of colors; the C program needs manual adjustment to support more.) It can also restrict its investigation to patterns with a given symmetry.

The options are compile-time constants. They're defined as preprocessor macros so you can specify them on the compiler command line:

```
clang tiles.c       -DSIZE=<size> -DSTATES=<color count> -D<symmetry type-{brick|none}>
ghc   tiles.hs -cpp -DSIZE=<size> -DSTATES=<color count> -D<symmetry type-{brick|none}>
```

### Symmetries
#### Brick
A pattern has brick-like symmetry when the bottom half of a tile is the top half shifted horizontally by half its width.

These patterns have brick-like symmetry:

<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2_brick/1111100011110010.png">
<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2_brick/1100110000110011.png">
<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2_brick/1100100100110110.png">

These patterns don't:

<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2/1111110011110010.png">
<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2/1111110000000011.png">
<img src="https://jacobwinters.com/tiled-patterns/results/4x4_2/1110110101111011.png">

## Running
The project comes with a shell script that will:

1. Compile one or both of the C and Haskell programs with a given set of options and run them
2. If you ran both programs, compare their results and alert you if they are different (which would indicate that there's a bug somewhere)
3. Optionally produce an example of each pattern equivalence class in PNG format and write an HTML file that displays all of the PNGs it generated

It depends on Clang, GHC, Netpbm, Optipng, Python, and ZopfliPNG.

```
./run [-H] [-C] -x<size> [-i<size of png files>] [-c<color count>] [-s<symmetry type-{brick|none}>]

-H: Run Haskell
-C: Run C
-i: Create images and HTML
```

There is also a `runall` script that tries a variety of parameters. This is the script that produces the results published on GitLab. Increases in tile size beyond those that `runall` tries aren't feasible on any hardware I have access to. Increases in state count are possible but I don't think the patterns will be as interesting as they are with low state counts.

## Results
A pattern from each equivalence class is displayed. The equivalence classes are sorted by the number of distinct images that produce patterns belonging to them.

Listed from most to least interesting:
* [6x6 2-state brick](https://jacobwinters.com/tiled-patterns/results/6x6_2_brick/)
* [4x4 2-state](https://jacobwinters.com/tiled-patterns/results/4x4_2/)
* [4x4 2-state brick](https://jacobwinters.com/tiled-patterns/results/4x4_2_brick/)
* [4x4 3-state brick](https://jacobwinters.com/tiled-patterns/results/4x4_3_brick/)
* [3x3 2-state](https://jacobwinters.com/tiled-patterns/results/3x3_2/)
* [3x3 3-state](https://jacobwinters.com/tiled-patterns/results/3x3_3/)
* [2x2 2-state](https://jacobwinters.com/tiled-patterns/results/2x2_2/)
* [2x2 2-state brick](https://jacobwinters.com/tiled-patterns/results/2x2_2_brick/)
* [2x2 3-state](https://jacobwinters.com/tiled-patterns/results/2x2_3/)
* [2x2 3-state brick](https://jacobwinters.com/tiled-patterns/results/2x2_3_brick/)

There are also some runs that produce too many images to make a gallery, but you can download the raw data if you want to:
* [5x5 2-state](https://jacobwinters.com/tiled-patterns/results/5x5_2/results)
* [4x4 3-state](https://jacobwinters.com/tiled-patterns/results/4x4_3/results)
