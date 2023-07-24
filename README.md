# Car-Park
In a 2-dimesional car park as seen from above, vehicles are allowed to park either vertically or horizontally on a grid :
<img width="65" alt="image" src="https://github.com/Leon-Chen1999/Car-Park/assets/122807406/67404115-9e96-42d3-aa83-1889ef50c4e0">

Here there are two vehicles, both of length 3. One is vertical (aligned north/south) coloured red, and one is horizontal (aligned west/east) coloured green. Empty cells of the gridded car park are shown in gray, and immovable boundaries (bollards) are shown in orange. Vehicles can only move forwards or backwards one square at a time (vertical vehicles move up and down, horizontal vehicles move left and right). These moves cannot be into, or across, another vehicle, nor into a bollard.
Our aim is to get all the vehicles safely out of the car park. During a turn, one vehicle is allowed to move fowards or backwards one square. Once the front of a vehicle touches an empty square on the edge of the car park, it is deemed to have exited the car park, and removed.
To ‘solve’ the car park above, we would move vehicle B one move left (so that it exits) :
