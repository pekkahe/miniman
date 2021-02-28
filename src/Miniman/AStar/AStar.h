//////////////////////////////////////////////////////////////////////////
//
// A* pathfinding: http://www.policyalmanac.org/games/aStarTutorial.htm
//
// Calculates shortest path from path A to B using a formula F = G + H where:
// G = the movement cost to move from the starting point to a given point on the plane,
//     following the path generated to get there.
// H = the estimated movement cost to move from that given point on the plane to the final destination.
//
// The path with the lowest F score is chosen.
//
//////////////////////////////////////////////////////////////////////////

#ifndef ASTAR_H_
#define ASTAR_H_

#include "PathFinder.h"
#include "Path.h"
#include "PathIterator.h"
#include "ClientInterface.h"

#endif