# IZP - second project
Here is my previously private repo of my solution of the first project in **Introduction to programming systems (IZP)** class, which is basically an introductory C course and introduction to programming as a whole.  I coded it in november of 2022, when I took the class

The structure of the program/code was all laid out. There was no room at all for any invention, even though I managed to re-implement something that was already done in the code's "skeleton" but I didn't notice it. It's the "Swap" on line 581 in `cluster.c`. My implementation was simpler - swap needless cluster with the last one in the array and decrement `cluster_count`. It made the clustering less stable, though, meaning it outputted the clusters in different order than the reference solution. It could disadvantage my program in school's testing, but I couldn't be bothered - it did what it was supposed to do - it outputed the same clusters, order of which has no meaning (or does it? 🤔 I don't really know).

It leaks memory. It was too late for me to fix it. Not because of the deadline but because I had to focus on other classes and would not benefit in any way from the hours put into finding the leak.

Also, the responsible teacher had told us thah we can define our own functions. But I, of course, was being petty and refused to write anything elsewhere than under `// TODO` comments, like I was told.

I am now publishing this repo so that I have something to show on my GitHub profile.
