/**
 *  \file driver.cc
 *  \brief HW 3: Minimum Spanning Tree using Prim's Algorithm
 *
 *  This program gets the name of the file from the command and
 *  calls the minimum spanning tree function to create the 
 *  minimum spanning tree for the input file.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.c"
#include "MST.hh"

/* ============================================================
 */

int main (int argc, char* argv[])
{
  int N = -1;

  if (argc == 2) {
  } else {
    fprintf (stderr, "usage: %s <n>\n", argv[0]);
    fprintf (stderr, "where <n> is the length of the list to sort.\n");
    return -1;
  }
  stopwatch_init ();
  struct stopwatch_t* timer = stopwatch_create ();
  assert (timer);
  stopwatch_start (timer);
  mst(string(argv[1]));
  long double t_std = stopwatch_stop (timer);
  printf ("Running Time: %Lg seconds\n", t_std);
  return 0;
}

/* eof */
