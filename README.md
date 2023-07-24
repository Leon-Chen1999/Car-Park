# Car-Park
  In a 2-dimesional car park as seen from above, vehicles are allowed to park either vertically or horizontally on a grid :  
  <img width="65" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/67404115-9e96-42d3-aa83-1889ef50c4e0">  

  Here there are two vehicles, both of length 3. One is vertical (aligned north/south) coloured red, and one is horizontal (aligned west/east) coloured green. Empty cells of the gridded car park are shown in gray, and immovable boundaries (bollards) are shown in orange. Vehicles can only move forwards or backwards one square at a time (vertical vehicles move up and down, horizontal vehicles move left and right). These moves cannot be into, or across, another vehicle, nor into a bollard.
  Our aim is to get all the vehicles safely out of the car park. During a turn, one vehicle is allowed to move fowards or backwards one square. Once the front of a vehicle touches an empty square on the edge of the car park, it is deemed to have exited the car park, and removed.
To ‘solve’ the car park above, we would move vehicle B one move left (so that it exits) :  
  <img width="65" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/2dad8988-fa7d-4252-8d69-78e9f7d939bb">  
  and then vehicle A up one :  
  <img width="67" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/88ed4b6f-1aec-4544-a37f-77da9ff76f09">  
  and then up one more move so that it too exits :  
  <img width="62" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/63656f4c-3bd4-44cc-a5d7-d40aebe57338">  
  All vehicles have now exited the car park taking a total of three turns.
  Write a program that reads in a car park file (specified on the command line), and shows the ‘turns’ to solve it. The file for the car park above looks like 
  6x6
#.####  
.BBB.#  
#A...#  
#A...#  
#A...#  
######  

The first line has two numbers; the height of the car park (number of rows) and then the width (number of columns).
In the remainder of the file, vehicles are shown as a capital letter, gaps as a full-stop and bollards as a hash symbol. Each cars may only lie in the grid vertically or horizontally, and must be of at least length 2. Each vehicle must have a unique uppercase letter, the first of which must be an ‘A’, the next one be ‘B’ and so on.
We wil use a brute-force algorithm for searching over all moves for a solution :
1. You will use an array (list) of structures, each one containg the data for one car park. Note that you may choose to store the state of each car park, not as a 2D array, but
as something that is easier to manipulate, e.g. an array of vehicles, including their position, orientation and whether they’ve exited or not. Each approach has pros and cons.
2. Put the initial car park into the front of this list, f=0.
3. Consider the car park at the front of the list (index f).
4. For this (parent) car park, find the resulting (child) car parks which can be created from
all the possible vehicle moves. For each of these child car parks:
• If this car park is unique (i.e. it has not been seen before in the list), add it to the
end of the list.
• If it has been seen before (a duplicate) ignore it.
• If it is the ‘final’ car park, stop and print the solution.
5. Add one to f. If there are more car parks in the list, go to step 3.
To help with printing out the correct moves, when a solution has been found, each structure in the list will need to contain (amongst other things) a car park and a record of its parent car park, i.e. the car park that it was created from. Since you’re using an array, this could simply be which index of the array was the parent.
The program reads the name of the car park definition file from the command line. If it finds a successful solution, it prints out the number of car parks that would be printed in the solution and nothing else, or else exactly the phrase ‘No Solution?” if none can be found (as might be the case if it is impossible, or you simply run out of memory) :
$ ./carpark ../Git/Data/CarPark/6x6_2c_3t.prk
3 moves
$ .car/park ../Git/Data/CarPark/11x9_10c_26t.prk 26 moves

If the ‘show’ flag is used, your program will print out the solution in the correct order :

$ ./carpark -show ../Git/Data/CarPark/6x6_2c_3t.prk
#.####  
.BBB.#  
#A...#   
#A...#   
#A...#   
######  
  
#.####  
.....#  
#A...#   
#A...#  
#A...#   
######  
  
#.####   
.A...#   
#A...#  
#A...#   
#....#  
######  
  
#.####   
.....#  
#....#  
#....#  
#....#   
######  
  
3 moves  

  <img width="331" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/35ac32c9-7ee7-43a5-b8e0-cc6cd3f4582b">  

Your program :
• Must use the algorithm detailed above (which is similar to a queue and therefore a
breadth-first search). Do not use the other algorithms possible (e.g. best-first, guided, recursive etc.); the quality of your coding is being assessed against others taking the same approach.
• Must not use dynamic arrays or linked lists. Since car parks cannot be any larger than 20 × 20, you can create car parks of this size, and only use a sub-part of them if the car park required is smaller. The list of car parks can be a fixed (large) size.
• Should check for invalid car park definition files, and report in a graceful way if there is a problem, aborting with exit(EXIT_FAILURE) if so.
• Should not print anything else out to screen after successfully completing the search, except that which is shown above. Automated checking will be used during marking, and therefore the ouput must be precise.
• Should call the function test() to perform any assertion testing etc.

