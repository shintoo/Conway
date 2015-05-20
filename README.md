#Conway's Game of Life

Implementation of the popular hit, "Conway's Game of Life"!


##Usage
From a text file: 
```
./Conway <generations> <speed 1-10> <text file>
```   
Randomly generated:
```
./Conway <generations> <speed 1-10> random <X,Y>
```
If generations is 0, it will run until killed externally.

In the source, there are two important definitions:
```C
#define COLORSPEED 14 /* The higher this is, the slower the color changes.
                       * Comment out to remove color.

#define LOG 20        /* Stops the game when the field reaches a static
                       * state, writes how many generations it took to reach
                       * in a logfile called log.txt. The amount of games
                       * played and logged is specified by this constant.
                       * Comment out to neithor log nor stop the initial game.
                       */
```
