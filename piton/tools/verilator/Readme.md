### EXA Metro-mp 

- POC : 
    - Instantiate one metro chipset , n metro mpi tiles 

    - Hybrid MPI+openmp model 
    - Include Vmetro_tile , Vmetro_chipset objects and instantiates the 
    - eg 
        - Only build 
            - Vmetro_tile
            - Vmetro_chipset 
        - Include in main() with rank in parallel regions 
    - Files:
        - exa_metro_mpi.cpp
        - exa_metro_chipset.cpp
        - exa_metro_tile.cpp

- Generic Instantiation of metro chip , metro tile and their communication using only openmp 

- shared memory model with neighbour tiles 
    - Chipset with tile0(west)
    - tile(x,y) with neighbours 
    - each tile has its neighbours that will be communicating 
    - E W N S 

- One Chipset 
    - N tiles 
    - Tile to tile communication using openmp!!
