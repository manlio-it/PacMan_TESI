#pragma once

#include "CoreMinimal.h"
#include "MapBlock.h"
#include <algorithm>
#include <random>
#include "Utility.h"

class PACMAN_PROJECT_API DirectionGraph
{

	struct Nodo {
		int peso;
		bool start;
		bool target;
		Nodo* archi[4];
	};

public:

	Nodo* Matrix[36][28];

	Nodo* CurrentNode = nullptr;

	std::list<Nodo*> GraphMap;

	DirectionGraph(AMapBlock* (&GameMapBlocks)[36][28]);

	Utility::MovDirection ConservativeDirection(Utility::MovDirection PreviusMove);
	Utility::MovDirection BalancedDirection(Utility::MovDirection PreviusMove);
	DirectionGraph::Nodo* NextNode(Utility::MovDirection Move, Nodo* CurrentNodo);
};
