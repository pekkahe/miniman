#ifndef BEHAVIOURINTERFACE_H_
#define BEHAVIOURINTERFACE_H_

class AI;
class BehaviourInterface
{
public:
	virtual ~BehaviourInterface() {}

	virtual void init(AI* ai) = 0;
	
	virtual void act() = 0;
	
	virtual void reset() = 0;
};

#endif