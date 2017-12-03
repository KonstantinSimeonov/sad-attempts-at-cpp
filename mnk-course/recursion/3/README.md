# Description

Board with 21 bits:

```
 xxx 
xxxxx
xxxxx
xxxxx
 xxx 
```

- Every tile has a value of either 1 or 0.
- 3 operations are defined:
	- flip the bits crosswise:
	```
	 100 
	11100
	01000
	00000
	 000 
	```
	- flip the bits in a `3x3` square:
	```
	 000 
	11100
	11100
	11100
	 000 
	```
	- flip all bits on the field

Find the minimal amount of flips that are required to transform an empty board into a given board.

## Input
- read from "game.inp" file
- on the first line in the file is the number **K** - the amount of boards the task should be solved for
	- 1 < **K** < 500
- on the next **K** lines there will be exactly **21** bits (0 or 1), separated by a single whitespace

# Output
- The output should be written to a file named "game21.out"
- For every input field from "game.inp", output the answer on a new line. If the given field configuration cannot be achieved using the provided flips, output **-1**

### Sample test
```
2
0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0
0 1 1 0 0 1 0 0 1 0 0 0 0 1 1 0 0 0 1 1 1
```

### Sample output
```
-1
3
```