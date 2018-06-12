# Tower defense Game

A simple generic tower defense game written C. Using GLFW, OpenGL4.2 (GLEW), stb_image.h libraries.

Developer, User Documentation and initial Specification is also avaliable in Hungarian (see PDFs).

## Game rules:

Enemies start from a start point and move towards the goal. The player's tasked with stopping these enemies by shooting them with turrets they place.

### Enemies:

March from **spawner** to **goal** cell until they reach it or die. The player gets money for killing them. If they reach the goal they despawn the player loses a life.

### Money:

Can be used to:
- build towers
- upgrade towers (damage, firerate, range)

### Targeting methods for towers:

Can be altered by the player between set presets of: least/most HP, closest/furthest, slowest/fastest.

### Starting the game:

Loads the map and puts the player in the **interwave** state.

### Waves:

Upon pressing the `Start wave` button a wave starts (which cannot be cancelled from now on). It spawns a set number of enemies and then waits for the player killing them or them reaching the goal. After which it puts the player back in the interwave state (presenting the start wave button again).

## Map file

The cells of the map and the spawner's properties are read from the `sample.map` file.

### Initial values

```c
money [x:int]
hp [x:int]
```

### Describing cells:

Camera will be initially set to the cell set as a `cell_camera`:
```c
cell_camera [x:int] [y:int]
```

#### Cells block:

```c
cells [rows:int] [cols:int]
cell [x:int] [y:int] [type:string] 
...
endcells
```

#### Cell types:

```c
"unbuildable" // no tower can be built on it, no enemy can step on it
"buildable" // tower can be built on it, no enemy can step on it (NOTE: all cell is implicitly marked as buildable)
"path" // no tower can be built on it, enemies can step on it
"spawner" // no tower can be built on it, enemies appear from here (there may be only be one spawner)
"goal" // no tower can be built on it, enemies march to here (there may be only be one goal)
```

### Describing spawners:

#### Spawner block
```c
spawner
hp_func [type:string] {parameters} 
hp_deviation_func [type:string] {parameters} 
speed_func [type:string] {parameters} 
speed_deviation_func [type:string] {parameters} 
money_func [type:string] {parameters} 
num_func [type:string] [{parameter} 
endspawner
```

Parameters means fix number of double values. See also next section for function types and their parameters.

#### Function types and parameters associated with them

```c
constant     – 1 parameter p[0]
linear       – 2 parameter p[0]+p[1]*x
quadratic    – 2 parameter p[0]+p[1]*x^2
power        – 3 parameter p[0]+p[1]*x^p[2]
exponential  – 3 parameter p[0]+p[1]*p[2]^x
logarithmic  – 3 parameter p[0]+p[1]*log_base_p[2](p[2]+x)
```