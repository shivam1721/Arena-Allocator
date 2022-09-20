# Arena-Allocator

In this project, we study concepts of memory management by constructring a custom allocator which allows the user to allocate chunks of memory 
within a preexisting arena initialized by the user. For better understanding on how the Operating System decides which free block to allocate, 
we implement four allocation techniques First fit, Next fit, Best fit, and Worst fit. To do this, we use linked lists and enumerated types to keep track of holes in memory.
