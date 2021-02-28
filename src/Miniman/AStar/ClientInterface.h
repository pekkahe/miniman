#ifndef IPATHSEARCHER_H_
#define IPATHSEARCHER_H_

#include "AStarInternal.h"

namespace astar 
{
	/// <summary>
	/// Defines a client interface for the AStar path finder. Used to notify
	/// the client when the path finding is completed.
	/// </summary>
	class ClientInterface
	{
	public:
		virtual void pathFound(Path*) = 0;
	};
}

#endif