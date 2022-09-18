/*
    Name: Shivam Patel
    ID: 1001707265
*/

// The MIT License (MIT)
// 
// Copyright (c) 2022 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "mavalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


enum TYPE
{
    FREE = 0,
    USED
};
// Node for the linked list
struct Node {
  size_t size;
  enum TYPE type;
  void * arena;
  struct Node * next;   
  struct Node * prev;    
};

struct Node *alloc_list;
struct Node *previous_node;

void * arena;

enum ALGORITHM allocation_algorithm = FIRST_FIT;

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  arena = malloc( ALIGN4( size ) );
  
  allocation_algorithm = algorithm;

  alloc_list = ( struct Node * )malloc( sizeof( struct Node ));

  alloc_list -> arena = arena;
  alloc_list -> size  = ALIGN4(size);
  alloc_list -> type  = FREE;
  alloc_list -> next  = NULL;
  alloc_list -> prev  = NULL;

  previous_node  = alloc_list;

  return 0;
}

void mavalloc_destroy( )
{
  free( arena );  
  // iterate over the linked list and free the nodes
  return;
}

void * mavalloc_alloc( size_t size )
{
  struct Node * node;

  if( allocation_algorithm == FIRST_FIT )
  { 
    node = alloc_list;
  }
  // next fit starts from where the previous one ended
  else if( allocation_algorithm == NEXT_FIT )
  { 
    node = previous_node;
  }
  else if( allocation_algorithm == WORST_FIT )
  { 
    node = alloc_list;
  }
  else if( allocation_algorithm == BEST_FIT )
  { 
    node = alloc_list;
  }
  else
  {
    printf("ERROR: Unknown allocation algorithm!\n");
    exit(0);
  }

  size_t aligned_size = ALIGN4( size );
  // First Fit
  if( allocation_algorithm == FIRST_FIT)
  {
    while( node )
    {
      if( node -> size >= aligned_size  && node -> type == FREE )
      {
        int leftover_size = 0;
  
        node -> type  = USED;
        leftover_size = node -> size - aligned_size;
        node -> size =  aligned_size;
  
        if( leftover_size > 0 )
        {
          struct Node * previous_next = node -> next;
          struct Node * leftover_node = ( struct Node * ) malloc ( sizeof( struct Node ));
  
          leftover_node -> arena = node -> arena + size;
          leftover_node -> type  = FREE;
          leftover_node -> size  = leftover_size;
          leftover_node -> next  = previous_next;
  
          node -> next = leftover_node;
        }
        previous_node = node;
        return ( void * ) node -> arena;
      }
      node = node -> next;
    }
  }
  // implement Next Fit
  else if( allocation_algorithm == NEXT_FIT )
  {
    // if the previous node is null it would start from beginning
    if(previous_node == NULL)
    {
        previous_node = alloc_list;
    }
    struct Node * previous_next = node -> next;
    while( node )
    {
      if( node -> size >= aligned_size  && node -> type == FREE )
      {
        int leftover_size = 0;
  
        node -> type  = USED;
        leftover_size = node -> size - aligned_size;
        node -> size =  aligned_size;
  
        if( leftover_size > 0 )
        {
          struct Node * previous_next = node -> next;
          struct Node * leftover_node = ( struct Node * ) malloc ( sizeof( struct Node ));
  
          leftover_node -> arena = node -> arena + size;
          leftover_node -> type  = FREE;
          leftover_node -> size  = leftover_size;
          leftover_node -> next  = previous_next;
  
          node -> next = leftover_node;
        }
        return ( void * ) node -> arena;
      }
      node = node -> next;
      if(node == previous_node)
        break;
      if(node == NULL)
        node = arena;    
    }
  }
  // Implement Best Fit
  else if(allocation_algorithm == BEST_FIT)
  {
      size_t maSize = INT_MAX;
      struct Node *temp1 = NULL;
      size_t tempSize;
      while ( node )
      {
          if(node -> size > aligned_size  && node -> type == FREE)
          {
              tempSize = node -> size - aligned_size;
              if(tempSize < maSize)
              {
                  maSize = node -> size - size;
                  temp1 = node;
              }
          }
          node = node -> next;
      }
      node = temp1;       
  }
  // Implement Worst Fit
  else if(allocation_algorithm == WORST_FIT)
  {
      size_t mSize = INT_MIN;
      struct Node *temp2 = NULL;
      size_t tempSz;
      while ( node )
      {
          tempSz = aligned_size - node -> size;
          if(node -> size >= aligned_size  && node -> type == FREE && tempSz > mSize)
          {
              mSize = node -> size;
              temp2 = node;
          }
          node = node -> next;
      }
      node = temp2;    
  }
  return NULL;
}

void mavalloc_free( void * ptr )
{
    struct Node * node = alloc_list;
    while( node )
    {
        if(node -> arena == ptr)
        {
            if(node -> type == FREE)
            {
                printf("Warning: Double free detection\n");
            }
            node -> type = FREE;
            break;
        }
        node = node -> next;
    }
  return;
}

int mavalloc_size( )
{
  int number_of_nodes = 0;
  struct Node * ptr = alloc_list;

  while( ptr )
  {
    number_of_nodes ++;
    ptr = ptr -> next; 
  }

  return number_of_nodes;
}
