#include <thread>
#include <iostream>
#include "PathFinder.h"
#include "Path.h"
#include "Graph.h"
#include "Worker.h"
#include "ClientInterface.h"
#include "../Level.h"

using namespace astar;

astar::PathFinder& astar::pathFinder()
{
	static PathFinder finder;
	return finder;
}

PathFinder::PathFinder() :
	graph_(nullptr),
	gameClosed_(false)
{ }

void PathFinder::initialize(const Level& level)
{
	if (graph_ != nullptr) {
		delete graph_;
	}

	graph_ = new Graph(level);
	graph_->initialize();
}

void PathFinder::exclude(sf::Vector2u position, ClientInterface* client)
{
	mutex_.lock();
	excludeMap_[client] = position;
	mutex_.unlock();
}

void PathFinder::include(sf::Vector2u position, ClientInterface* client)
{
	mutex_.lock();
	includeMap_[client] = position;
	mutex_.unlock();
}

void PathFinder::findPath(sf::Vector2u from, sf::Vector2u to, ClientInterface* client)
{
	// Start new path finding thread for client
	std::thread thread(&PathFinder::startWorker, this, from, to, client);

	// Store thread id for client, i.e. register client
	mutex_.lock();
	threads_[client] = thread.get_id();
	mutex_.unlock();

	// Detach thread so it can keep on living out of this scope,
	// and will be automatically destroyed once ended
	thread.detach();
}

void PathFinder::initGraphForClient(Graph* graph, ClientInterface* client)
{
	mutex_.lock();

	// Remove exclusive position from map
	if (excludeMap_.count(client) > 0) {
		graph->remove(excludeMap_[client]);
		excludeMap_.erase(client); // Unregister
	}

	// Add inclusive position to map
	if (includeMap_.count(client) > 0) {
		graph->add(includeMap_[client]);
		includeMap_.erase(client); // Unregister
	}

	mutex_.unlock();
}

void PathFinder::startWorker(sf::Vector2u from, sf::Vector2u to, ClientInterface* client)
{
	// Uncomment below to simulate path finding delay
	//std::this_thread::sleep_for(std::chrono::seconds(2));

	// Create a thread-scope copy of graph
	Graph graph(*graph_);
	
	initGraphForClient(&graph, client);

	Worker worker;
	Path* path = worker.findPath(from, to, &graph);

	// Early out ongoing threads if game quit
	if (gameClosed_) {
		return;
	}

	// Forward path if client hasn't unregistered or sent another request in the meanwhile
	mutex_.lock();

	if (threads_.count(client) > 0 && threads_[client] == std::this_thread::get_id()) {
		client->pathFound(path);
		threads_.erase(client); // Unregister
	}

	mutex_.unlock();
}

void PathFinder::unregister(ClientInterface* client)
{
	mutex_.lock();

	threads_.erase(client);
	includeMap_.erase(client);
	excludeMap_.erase(client);

	mutex_.unlock();
}

void PathFinder::gameClosed()
{
	gameClosed_ = true;
}

PathFinder::~PathFinder()
{
	if (graph_ != nullptr) {
		delete graph_;
	}
}
