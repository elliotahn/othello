Elliot Ahn - kobe_is_the_goat

I worked by myself.

My initial approach was to create my own class. The class was a tree where each node a board state along with the move made by the last player. Nodes were connected by moves, and choosing a move was simply a matter of changing the head node and deleting the other nodes. Unfortunately, this complicated pointer method was difficult as I was gettiing many segfaults and it was very difficult (even with the help of the TAs) to debug.

I then changed to a more recursive approach. I initially frowned upon this approach (favoring the tree method) because recursion would require a lot of redundant calculations (like a recursive Fibbonacci function). However, I added a memoization structure in the form of an unordered_map that would keep me from re-calculating available moves for a given board state. The key to doing this was being able to express a board state as a unique string and using the string as a key for quick lookup.

The scoring was simply done by weights, being aggressive for the corners and equally aggressive from keeping the opponents from capturing the corners. I was able to defeat BetterPlayer consistently, making this AI competitive for the CS2 tournament. The AI would regularly fall behind early and then capture the corners at the end to quickly turn the game around. This was explained by the fact that I would calculate the minimax tree to depth 4 for most of the game, and then use depth 6 at the end of the game.