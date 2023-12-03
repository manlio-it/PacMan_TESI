#include "DirectionGraph.h"


DirectionGraph::DirectionGraph(AMapBlock* (&GameMapBlocks)[36][28])
{

	//Operations for derive graph

	for (int i = 0; i < 36; i++) {
		for (int k = 0; k < 28; k++) {
			
			if (GameMapBlocks[i][k]->isWalkable && !(GameMapBlocks[i][k]->isSlowToApply)) {
				Matrix[i][k] = new Nodo();
				
				if (GameMapBlocks[i][k]->PacMan) {
					Matrix[i][k]->start = true;
				}
				Matrix[i][k]->peso = GameMapBlocks[i][k]->valore_rischio;
				Matrix[i][k]->target = GameMapBlocks[i][k]->Food;
			}
			else {
				Matrix[i][k] = nullptr;
			}
		}
	}

	for (int i = 0; i < 36; i++) {
		for (int k = 0; k < 28; k++) {

			if (Matrix[i][k] != nullptr && Matrix[i - 1][k] != nullptr && i > 0) {
				Matrix[i][k]->archi[0] = Matrix[i - 1][k];
			}
			if (Matrix[i][k] != nullptr && Matrix[i][k - 1] != nullptr && k > 0) {
				Matrix[i][k]->archi[1] = Matrix[i][k - 1];
			}
			if (Matrix[i][k] != nullptr && Matrix[i + 1][k] != nullptr && i < 35) {
				Matrix[i][k]->archi[2] = Matrix[i + 1][k];
			}
			if (Matrix[i][k] != nullptr && Matrix[i][k + 1] != nullptr && k < 27) {
				Matrix[i][k]->archi[3] = Matrix[i][k + 1];
			}
		}
	}

	for (int i = 0; i < 36; i++) {
		for (int k = 0; k < 28; k++) {
			if (Matrix[i][k] != nullptr) {
				GraphMap.push_back(Matrix[i][k]);
			}
		}
	}

	for (auto node : GraphMap) {
		if (node->start) {
			CurrentNode = node;
			break;
		}
	}
}


Utility::MovDirection DirectionGraph::ConservativeDirection(Utility::MovDirection PreviusMove) {

	//Conservative algorithm

	Utility::MovDirection MovDirection = Utility::IDLE;
	Utility::MovDirection temp_var[4] = { Utility::LEFT, Utility::RIGHT, Utility::UP, Utility::DOWN };

	std::random_device rd;
	std::mt19937 r(rd());
	std::shuffle(std::begin(temp_var), std::end(temp_var), r);

	int valore_peso_temp = 100000;
	int valore_peso_temp1 = 100000;
	int temp_peso;
	Nodo* temp1;
	Nodo* temp2;
	Nodo* temp3;
	Nodo* temp4;
	Nodo* temp5;
	int esecuzioni = 0;

	
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			for (int c = 0; c < 4; c++) {
				for (int d = 0; d < 4; d++) {
					for (int e = 0; e < 4; e++) {
						for (int f = 0; f < 4; f++) {
							for (int g = 0; g < 4; g++) {
								for (int h = 0; h < 4; h++) {
									for (int i = 0; i < 4; i++) {
										for (int j = 0; j < 4; j++) {
											for (int m = 0; m < 4; m++) {
												for (int n = 0; n < 4; n++) {

													if ((temp_var[a] == Utility::LEFT && temp_var[b] == Utility::RIGHT) || (temp_var[a] == Utility::RIGHT && temp_var[b] == Utility::LEFT))
														continue;

													if ((temp_var[a] == Utility::UP && temp_var[b] == Utility::DOWN) || (temp_var[a] == Utility::DOWN && temp_var[b] == Utility::UP))
														continue;

													if ((temp_var[a] == Utility::LEFT && temp_var[c] == Utility::RIGHT) || (temp_var[a] == Utility::RIGHT && temp_var[c] == Utility::LEFT))
														continue;

													if ((temp_var[a] == Utility::UP && temp_var[c] == Utility::DOWN) || (temp_var[a] == Utility::DOWN && temp_var[c] == Utility::UP))
														continue;
													//
													if ((temp_var[b] == Utility::LEFT && temp_var[c] == Utility::RIGHT) || (temp_var[b] == Utility::RIGHT && temp_var[c] == Utility::LEFT))
														continue;

													if ((temp_var[b] == Utility::UP && temp_var[c] == Utility::DOWN) || (temp_var[b] == Utility::DOWN && temp_var[c] == Utility::UP))
														continue;

													if ((temp_var[b] == Utility::LEFT && temp_var[d] == Utility::RIGHT) || (temp_var[b] == Utility::RIGHT && temp_var[d] == Utility::LEFT))
														continue;

													if ((temp_var[b] == Utility::UP && temp_var[d] == Utility::DOWN) || (temp_var[b] == Utility::DOWN && temp_var[d] == Utility::UP))
														continue;
													//
													if ((temp_var[c] == Utility::LEFT && temp_var[d] == Utility::RIGHT) || (temp_var[c] == Utility::RIGHT && temp_var[d] == Utility::LEFT))
														continue;

													if ((temp_var[c] == Utility::UP && temp_var[d] == Utility::DOWN) || (temp_var[c] == Utility::DOWN && temp_var[d] == Utility::UP))
														continue;

													if ((temp_var[c] == Utility::LEFT && temp_var[e] == Utility::RIGHT) || (temp_var[c] == Utility::RIGHT && temp_var[e] == Utility::LEFT))
														continue;

													if ((temp_var[c] == Utility::UP && temp_var[e] == Utility::DOWN) || (temp_var[c] == Utility::DOWN && temp_var[e] == Utility::UP))
														continue;
													//
													if ((temp_var[d] == Utility::LEFT && temp_var[e] == Utility::RIGHT) || (temp_var[d] == Utility::RIGHT && temp_var[e] == Utility::LEFT))
														continue;

													if ((temp_var[d] == Utility::UP && temp_var[e] == Utility::DOWN) || (temp_var[d] == Utility::DOWN && temp_var[e] == Utility::UP))
														continue;

													if ((temp_var[d] == Utility::LEFT && temp_var[f] == Utility::RIGHT) || (temp_var[d] == Utility::RIGHT && temp_var[f] == Utility::LEFT))
														continue;

													if ((temp_var[d] == Utility::UP && temp_var[f] == Utility::DOWN) || (temp_var[d] == Utility::DOWN && temp_var[f] == Utility::UP))
														continue;
													//
													if ((temp_var[e] == Utility::LEFT && temp_var[f] == Utility::RIGHT) || (temp_var[e] == Utility::RIGHT && temp_var[f] == Utility::LEFT))
														continue;

													if ((temp_var[e] == Utility::UP && temp_var[f] == Utility::DOWN) || (temp_var[e] == Utility::DOWN && temp_var[f] == Utility::UP))
														continue;

													if ((temp_var[e] == Utility::LEFT && temp_var[g] == Utility::RIGHT) || (temp_var[e] == Utility::RIGHT && temp_var[g] == Utility::LEFT))
														continue;

													if ((temp_var[e] == Utility::UP && temp_var[g] == Utility::DOWN) || (temp_var[e] == Utility::DOWN && temp_var[g] == Utility::UP))
														continue;
													//
													if ((temp_var[f] == Utility::LEFT && temp_var[g] == Utility::RIGHT) || (temp_var[f] == Utility::RIGHT && temp_var[g] == Utility::LEFT))
														continue;

													if ((temp_var[f] == Utility::UP && temp_var[g] == Utility::DOWN) || (temp_var[f] == Utility::DOWN && temp_var[g] == Utility::UP))
														continue;

													if ((temp_var[f] == Utility::LEFT && temp_var[h] == Utility::RIGHT) || (temp_var[f] == Utility::RIGHT && temp_var[h] == Utility::LEFT))
														continue;

													if ((temp_var[f] == Utility::UP && temp_var[h] == Utility::DOWN) || (temp_var[f] == Utility::DOWN && temp_var[h] == Utility::UP))
														continue;
													//
													if ((temp_var[g] == Utility::LEFT && temp_var[h] == Utility::RIGHT) || (temp_var[g] == Utility::RIGHT && temp_var[h] == Utility::LEFT))
														continue;

													if ((temp_var[g] == Utility::UP && temp_var[h] == Utility::DOWN) || (temp_var[g] == Utility::DOWN && temp_var[h] == Utility::UP))
														continue;

													if ((temp_var[g] == Utility::LEFT && temp_var[i] == Utility::RIGHT) || (temp_var[g] == Utility::RIGHT && temp_var[i] == Utility::LEFT))
														continue;

													if ((temp_var[g] == Utility::UP && temp_var[i] == Utility::DOWN) || (temp_var[g] == Utility::DOWN && temp_var[i] == Utility::UP))
														continue;
													//
													if ((temp_var[h] == Utility::LEFT && temp_var[i] == Utility::RIGHT) || (temp_var[h] == Utility::RIGHT && temp_var[i] == Utility::LEFT))
														continue;

													if ((temp_var[h] == Utility::UP && temp_var[i] == Utility::DOWN) || (temp_var[h] == Utility::DOWN && temp_var[i] == Utility::UP))
														continue;

													if ((temp_var[h] == Utility::LEFT && temp_var[j] == Utility::RIGHT) || (temp_var[h] == Utility::RIGHT && temp_var[j] == Utility::LEFT))
														continue;

													if ((temp_var[h] == Utility::UP && temp_var[j] == Utility::DOWN) || (temp_var[h] == Utility::DOWN && temp_var[j] == Utility::UP))
														continue;
													//
													if ((temp_var[i] == Utility::LEFT && temp_var[j] == Utility::RIGHT) || (temp_var[i] == Utility::RIGHT && temp_var[j] == Utility::LEFT))
														continue;

													if ((temp_var[i] == Utility::UP && temp_var[j] == Utility::DOWN) || (temp_var[i] == Utility::DOWN && temp_var[j] == Utility::UP))
														continue;

													if ((temp_var[i] == Utility::LEFT && temp_var[m] == Utility::RIGHT) || (temp_var[i] == Utility::RIGHT && temp_var[m] == Utility::LEFT))
														continue;

													if ((temp_var[i] == Utility::UP && temp_var[m] == Utility::DOWN) || (temp_var[i] == Utility::DOWN && temp_var[m] == Utility::UP))
														continue;
													//
													if ((temp_var[j] == Utility::LEFT && temp_var[m] == Utility::RIGHT) || (temp_var[j] == Utility::RIGHT && temp_var[m] == Utility::LEFT))
														continue;

													if ((temp_var[j] == Utility::UP && temp_var[m] == Utility::DOWN) || (temp_var[j] == Utility::DOWN && temp_var[m] == Utility::UP))
														continue;

													if ((temp_var[j] == Utility::LEFT && temp_var[n] == Utility::RIGHT) || (temp_var[j] == Utility::RIGHT && temp_var[n] == Utility::LEFT))
														continue;

													if ((temp_var[j] == Utility::UP && temp_var[n] == Utility::DOWN) || (temp_var[j] == Utility::DOWN && temp_var[n] == Utility::UP))
														continue;
													//
													if ((temp_var[m] == Utility::LEFT && temp_var[n] == Utility::RIGHT) || (temp_var[m] == Utility::RIGHT && temp_var[n] == Utility::LEFT))
														continue;

													if ((temp_var[m] == Utility::UP && temp_var[n] == Utility::DOWN) || (temp_var[m] == Utility::DOWN && temp_var[n] == Utility::UP))
														continue;

													esecuzioni++;

													if (NextNode(temp_var[a], CurrentNode) != nullptr && NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)) != nullptr) {
														temp1 = NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode));
														if (NextNode(temp_var[c], temp1) != nullptr && NextNode(temp_var[d], NextNode(temp_var[c], temp1)) != nullptr) {
															temp2 = NextNode(temp_var[d], NextNode(temp_var[c], temp1));
															if (NextNode(temp_var[e], temp2) != nullptr && NextNode(temp_var[f], NextNode(temp_var[e], temp2)) != nullptr) {
																temp3 = NextNode(temp_var[f], NextNode(temp_var[e], temp2));
																if (NextNode(temp_var[g], temp3) != nullptr && NextNode(temp_var[h], NextNode(temp_var[g], temp3)) != nullptr) {
																	temp4 = NextNode(temp_var[h], NextNode(temp_var[g], temp3));
																	if (NextNode(temp_var[i], temp4) != nullptr && NextNode(temp_var[j], NextNode(temp_var[i], temp4)) != nullptr) {
																		temp5 = NextNode(temp_var[j], NextNode(temp_var[i], temp4));
																		if (NextNode(temp_var[m], temp5) != nullptr && NextNode(temp_var[n], NextNode(temp_var[m], temp5)) != nullptr) {
																			temp_peso = NextNode(temp_var[a], CurrentNode)->peso + NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[c], temp1)->peso + NextNode(temp_var[d], NextNode(temp_var[c], temp1))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[e], temp2)->peso + NextNode(temp_var[f], NextNode(temp_var[e], temp2))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[g], temp3)->peso + NextNode(temp_var[h], NextNode(temp_var[g], temp3))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[i], temp4)->peso + NextNode(temp_var[j], NextNode(temp_var[i], temp4))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[m], temp5)->peso + NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso;

																			bool check = false;

																			if (NextNode(temp_var[a], CurrentNode)->peso < 6500 && NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))->peso < 6500 && NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)))->peso < 6500 && NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))))->peso < 6500 &&
																				NextNode(temp_var[e], NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)))))->peso < 6500 && NextNode(temp_var[f], NextNode(temp_var[e], NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))))))->peso < 6500) {
																				if (temp_var[a] == PreviusMove) {
																					check = true;
																				}
																				else if (NextNode(PreviusMove, CurrentNode) == nullptr) {
																					check = true;
																				}
																				else if (NextNode(temp_var[a], CurrentNode)->peso < NextNode(PreviusMove, CurrentNode)->peso) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::LEFT && PreviusMove != Utility::RIGHT) && (NextNode(Utility::UP, CurrentNode) != nullptr || NextNode(Utility::DOWN, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::RIGHT && PreviusMove != Utility::LEFT) && (NextNode(Utility::UP, CurrentNode) != nullptr || NextNode(Utility::DOWN, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::UP && PreviusMove != Utility::DOWN) && (NextNode(Utility::LEFT, CurrentNode) != nullptr || NextNode(Utility::RIGHT, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::DOWN && PreviusMove != Utility::UP) && (NextNode(Utility::LEFT, CurrentNode) != nullptr || NextNode(Utility::RIGHT, CurrentNode) != nullptr)) {
																					check = true;
																				}
																			}

																			if (NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso <= valore_peso_temp && check) {
																				if (temp_peso <= valore_peso_temp1) {
																					valore_peso_temp1 = temp_peso;
																					valore_peso_temp = NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso;
																					MovDirection = temp_var[a];
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	/*
	UE_LOG(LogTemp, Warning, TEXT("****************************"));
	if (NextNode(Utility::UP, CurrentNode) != nullptr) {
		int val1 = NextNode(Utility::UP, CurrentNode)->peso;  //RIGHT
		UE_LOG(LogTemp, Warning, TEXT("Valore UP: %d"), val1);
	}
	if (NextNode(Utility::DOWN, CurrentNode) != nullptr) {
		int val2 = NextNode(Utility::DOWN, CurrentNode)->peso;  //RIGHT
		UE_LOG(LogTemp, Warning, TEXT("Valore DOWN: %d"), val2);
	}
	if (NextNode(Utility::LEFT, CurrentNode) != nullptr) {
		int val3 = NextNode(Utility::LEFT, CurrentNode)->peso; //LEFT
		UE_LOG(LogTemp, Warning, TEXT("Valore LEFT: %d"), val3);
	}
	if (NextNode(Utility::RIGHT, CurrentNode) != nullptr) {
		int val4 = NextNode(Utility::RIGHT, CurrentNode)->peso;  //RIGHT
		UE_LOG(LogTemp, Warning, TEXT("Valore RIGHT: %d"), val4);
	}
	if (MovDirection == Utility::UP)
		UE_LOG(LogTemp, Warning, TEXT("Scelta: UP"));
	if (MovDirection == Utility::DOWN)
		UE_LOG(LogTemp, Warning, TEXT("Scelta: DOWN"));
	if (MovDirection == Utility::LEFT)
		UE_LOG(LogTemp, Warning, TEXT("Scelta: LEFT"));
	if (MovDirection == Utility::RIGHT)
		UE_LOG(LogTemp, Warning, TEXT("Scelta: RIGHT"));

	*/

	// UE_LOG(LogTemp, Warning, TEXT("Tempi: %d"), esecuzioni);

	return MovDirection;
}


Utility::MovDirection DirectionGraph::BalancedDirection(Utility::MovDirection PreviusMove) {

	//Balanced algorithm

	Utility::MovDirection MovDirection = Utility::IDLE;
	Utility::MovDirection temp_var[4] = { Utility::LEFT, Utility::RIGHT, Utility::UP, Utility::DOWN };

	std::random_device rd;
	std::mt19937 r(rd());
	std::shuffle(std::begin(temp_var), std::end(temp_var), r);

	int valore_peso_temp = 100000;
	int valore_peso_temp1 = 100000;
	int temp_peso;

	Nodo* temp1;
	Nodo* temp2;
	Nodo* temp3;
	Nodo* temp4;
	Nodo* temp5;
	int esecuzioni = 0;

	for (auto node : GraphMap) {
		if (node->target) {
			node->peso = node->peso - 40;
		}
	}

	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			for (int c = 0; c < 4; c++) {
				for (int d = 0; d < 4; d++) {
					for (int e = 0; e < 4; e++) {
						for (int f = 0; f < 4; f++) {
							for (int g = 0; g < 4; g++) {
								for (int h = 0; h < 4; h++) {
									for (int i = 0; i < 4; i++) {
										for (int j = 0; j < 4; j++) {
											for (int m = 0; m < 4; m++) {
												for (int n = 0; n < 4; n++) {

													if ((temp_var[a] == Utility::LEFT && temp_var[b] == Utility::RIGHT) || (temp_var[a] == Utility::RIGHT && temp_var[b] == Utility::LEFT))
														continue;

													if ((temp_var[a] == Utility::UP && temp_var[b] == Utility::DOWN) || (temp_var[a] == Utility::DOWN && temp_var[b] == Utility::UP))
														continue;

													if ((temp_var[a] == Utility::LEFT && temp_var[c] == Utility::RIGHT) || (temp_var[a] == Utility::RIGHT && temp_var[c] == Utility::LEFT))
														continue;

													if ((temp_var[a] == Utility::UP && temp_var[c] == Utility::DOWN) || (temp_var[a] == Utility::DOWN && temp_var[c] == Utility::UP))
														continue;
													//
													if ((temp_var[b] == Utility::LEFT && temp_var[c] == Utility::RIGHT) || (temp_var[b] == Utility::RIGHT && temp_var[c] == Utility::LEFT))
														continue;

													if ((temp_var[b] == Utility::UP && temp_var[c] == Utility::DOWN) || (temp_var[b] == Utility::DOWN && temp_var[c] == Utility::UP))
														continue;

													if ((temp_var[b] == Utility::LEFT && temp_var[d] == Utility::RIGHT) || (temp_var[b] == Utility::RIGHT && temp_var[d] == Utility::LEFT))
														continue;

													if ((temp_var[b] == Utility::UP && temp_var[d] == Utility::DOWN) || (temp_var[b] == Utility::DOWN && temp_var[d] == Utility::UP))
														continue;
													//
													if ((temp_var[c] == Utility::LEFT && temp_var[d] == Utility::RIGHT) || (temp_var[c] == Utility::RIGHT && temp_var[d] == Utility::LEFT))
														continue;

													if ((temp_var[c] == Utility::UP && temp_var[d] == Utility::DOWN) || (temp_var[c] == Utility::DOWN && temp_var[d] == Utility::UP))
														continue;

													if ((temp_var[c] == Utility::LEFT && temp_var[e] == Utility::RIGHT) || (temp_var[c] == Utility::RIGHT && temp_var[e] == Utility::LEFT))
														continue;

													if ((temp_var[c] == Utility::UP && temp_var[e] == Utility::DOWN) || (temp_var[c] == Utility::DOWN && temp_var[e] == Utility::UP))
														continue;
													//
													if ((temp_var[d] == Utility::LEFT && temp_var[e] == Utility::RIGHT) || (temp_var[d] == Utility::RIGHT && temp_var[e] == Utility::LEFT))
														continue;

													if ((temp_var[d] == Utility::UP && temp_var[e] == Utility::DOWN) || (temp_var[d] == Utility::DOWN && temp_var[e] == Utility::UP))
														continue;

													if ((temp_var[d] == Utility::LEFT && temp_var[f] == Utility::RIGHT) || (temp_var[d] == Utility::RIGHT && temp_var[f] == Utility::LEFT))
														continue;

													if ((temp_var[d] == Utility::UP && temp_var[f] == Utility::DOWN) || (temp_var[d] == Utility::DOWN && temp_var[f] == Utility::UP))
														continue;
													//
													if ((temp_var[e] == Utility::LEFT && temp_var[f] == Utility::RIGHT) || (temp_var[e] == Utility::RIGHT && temp_var[f] == Utility::LEFT))
														continue;

													if ((temp_var[e] == Utility::UP && temp_var[f] == Utility::DOWN) || (temp_var[e] == Utility::DOWN && temp_var[f] == Utility::UP))
														continue;

													if ((temp_var[e] == Utility::LEFT && temp_var[g] == Utility::RIGHT) || (temp_var[e] == Utility::RIGHT && temp_var[g] == Utility::LEFT))
														continue;

													if ((temp_var[e] == Utility::UP && temp_var[g] == Utility::DOWN) || (temp_var[e] == Utility::DOWN && temp_var[g] == Utility::UP))
														continue;
													//
													if ((temp_var[f] == Utility::LEFT && temp_var[g] == Utility::RIGHT) || (temp_var[f] == Utility::RIGHT && temp_var[g] == Utility::LEFT))
														continue;

													if ((temp_var[f] == Utility::UP && temp_var[g] == Utility::DOWN) || (temp_var[f] == Utility::DOWN && temp_var[g] == Utility::UP))
														continue;

													if ((temp_var[f] == Utility::LEFT && temp_var[h] == Utility::RIGHT) || (temp_var[f] == Utility::RIGHT && temp_var[h] == Utility::LEFT))
														continue;

													if ((temp_var[f] == Utility::UP && temp_var[h] == Utility::DOWN) || (temp_var[f] == Utility::DOWN && temp_var[h] == Utility::UP))
														continue;
													//
													if ((temp_var[g] == Utility::LEFT && temp_var[h] == Utility::RIGHT) || (temp_var[g] == Utility::RIGHT && temp_var[h] == Utility::LEFT))
														continue;

													if ((temp_var[g] == Utility::UP && temp_var[h] == Utility::DOWN) || (temp_var[g] == Utility::DOWN && temp_var[h] == Utility::UP))
														continue;

													if ((temp_var[g] == Utility::LEFT && temp_var[i] == Utility::RIGHT) || (temp_var[g] == Utility::RIGHT && temp_var[i] == Utility::LEFT))
														continue;

													if ((temp_var[g] == Utility::UP && temp_var[i] == Utility::DOWN) || (temp_var[g] == Utility::DOWN && temp_var[i] == Utility::UP))
														continue;
													//
													if ((temp_var[h] == Utility::LEFT && temp_var[i] == Utility::RIGHT) || (temp_var[h] == Utility::RIGHT && temp_var[i] == Utility::LEFT))
														continue;

													if ((temp_var[h] == Utility::UP && temp_var[i] == Utility::DOWN) || (temp_var[h] == Utility::DOWN && temp_var[i] == Utility::UP))
														continue;

													if ((temp_var[h] == Utility::LEFT && temp_var[j] == Utility::RIGHT) || (temp_var[h] == Utility::RIGHT && temp_var[j] == Utility::LEFT))
														continue;

													if ((temp_var[h] == Utility::UP && temp_var[j] == Utility::DOWN) || (temp_var[h] == Utility::DOWN && temp_var[j] == Utility::UP))
														continue;
													//
													if ((temp_var[i] == Utility::LEFT && temp_var[j] == Utility::RIGHT) || (temp_var[i] == Utility::RIGHT && temp_var[j] == Utility::LEFT))
														continue;

													if ((temp_var[i] == Utility::UP && temp_var[j] == Utility::DOWN) || (temp_var[i] == Utility::DOWN && temp_var[j] == Utility::UP))
														continue;

													if ((temp_var[i] == Utility::LEFT && temp_var[m] == Utility::RIGHT) || (temp_var[i] == Utility::RIGHT && temp_var[m] == Utility::LEFT))
														continue;

													if ((temp_var[i] == Utility::UP && temp_var[m] == Utility::DOWN) || (temp_var[i] == Utility::DOWN && temp_var[m] == Utility::UP))
														continue;
													//
													if ((temp_var[j] == Utility::LEFT && temp_var[m] == Utility::RIGHT) || (temp_var[j] == Utility::RIGHT && temp_var[m] == Utility::LEFT))
														continue;

													if ((temp_var[j] == Utility::UP && temp_var[m] == Utility::DOWN) || (temp_var[j] == Utility::DOWN && temp_var[m] == Utility::UP))
														continue;

													if ((temp_var[j] == Utility::LEFT && temp_var[n] == Utility::RIGHT) || (temp_var[j] == Utility::RIGHT && temp_var[n] == Utility::LEFT))
														continue;

													if ((temp_var[j] == Utility::UP && temp_var[n] == Utility::DOWN) || (temp_var[j] == Utility::DOWN && temp_var[n] == Utility::UP))
														continue;
													//
													if ((temp_var[m] == Utility::LEFT && temp_var[n] == Utility::RIGHT) || (temp_var[m] == Utility::RIGHT && temp_var[n] == Utility::LEFT))
														continue;

													if ((temp_var[m] == Utility::UP && temp_var[n] == Utility::DOWN) || (temp_var[m] == Utility::DOWN && temp_var[n] == Utility::UP))
														continue;

													esecuzioni++;

													if (NextNode(temp_var[a], CurrentNode) != nullptr && NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)) != nullptr) {
														temp1 = NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode));
														if (NextNode(temp_var[c], temp1) != nullptr && NextNode(temp_var[d], NextNode(temp_var[c], temp1)) != nullptr) {
															temp2 = NextNode(temp_var[d], NextNode(temp_var[c], temp1));
															if (NextNode(temp_var[e], temp2) != nullptr && NextNode(temp_var[f], NextNode(temp_var[e], temp2)) != nullptr) {
																temp3 = NextNode(temp_var[f], NextNode(temp_var[e], temp2));
																if (NextNode(temp_var[g], temp3) != nullptr && NextNode(temp_var[h], NextNode(temp_var[g], temp3)) != nullptr) {
																	temp4 = NextNode(temp_var[h], NextNode(temp_var[g], temp3));
																	if (NextNode(temp_var[i], temp4) != nullptr && NextNode(temp_var[j], NextNode(temp_var[i], temp4)) != nullptr) {
																		temp5 = NextNode(temp_var[j], NextNode(temp_var[i], temp4));
																		if (NextNode(temp_var[m], temp5) != nullptr && NextNode(temp_var[n], NextNode(temp_var[m], temp5)) != nullptr) {
																			temp_peso = NextNode(temp_var[a], CurrentNode)->peso + NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[c], temp1)->peso + NextNode(temp_var[d], NextNode(temp_var[c], temp1))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[e], temp2)->peso + NextNode(temp_var[f], NextNode(temp_var[e], temp2))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[g], temp3)->peso + NextNode(temp_var[h], NextNode(temp_var[g], temp3))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[i], temp4)->peso + NextNode(temp_var[j], NextNode(temp_var[i], temp4))->peso;
																			temp_peso = temp_peso + NextNode(temp_var[m], temp5)->peso + NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso;

																			bool check = false;

																			if (NextNode(temp_var[a], CurrentNode)->peso < 6500 && NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))->peso < 6500 && NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)))->peso < 6500 && NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))))->peso < 6500 &&
																				NextNode(temp_var[e], NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode)))))->peso < 6500 && NextNode(temp_var[f], NextNode(temp_var[e], NextNode(temp_var[d], NextNode(temp_var[c], NextNode(temp_var[b], NextNode(temp_var[a], CurrentNode))))))->peso < 6500) {
																				if (temp_var[a] == PreviusMove) {
																					check = true;
																				}
																				else if (NextNode(PreviusMove, CurrentNode) == nullptr) {
																					check = true;
																				}
																				else if (NextNode(temp_var[a], CurrentNode)->peso < NextNode(PreviusMove, CurrentNode)->peso) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::LEFT && PreviusMove != Utility::RIGHT) && (NextNode(Utility::UP, CurrentNode) != nullptr || NextNode(Utility::DOWN, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::RIGHT && PreviusMove != Utility::LEFT) && (NextNode(Utility::UP, CurrentNode) != nullptr || NextNode(Utility::DOWN, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::UP && PreviusMove != Utility::DOWN) && (NextNode(Utility::LEFT, CurrentNode) != nullptr || NextNode(Utility::RIGHT, CurrentNode) != nullptr)) {
																					check = true;
																				}
																				else if ((temp_var[a] == Utility::DOWN && PreviusMove != Utility::UP) && (NextNode(Utility::LEFT, CurrentNode) != nullptr || NextNode(Utility::RIGHT, CurrentNode) != nullptr)) {
																					check = true;
																				}
																			}

																			if (NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso <= valore_peso_temp && check) {
																				if (temp_peso <= valore_peso_temp1) {
																					valore_peso_temp1 = temp_peso;
																					valore_peso_temp = NextNode(temp_var[n], NextNode(temp_var[m], temp5))->peso;
																					MovDirection = temp_var[a];
																				}
																			}
																		}
																	}
																}
															}
														}
													}

												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// UE_LOG(LogTemp, Warning, TEXT("Tempi: %d"), esecuzioni);

	return MovDirection;
}


DirectionGraph::Nodo* DirectionGraph::NextNode(Utility::MovDirection Move, Nodo* CurrentNodo) {

	//Next Nodo in one Direction
	switch (Move) {
	case Utility::UP:
		if (CurrentNodo->archi[0] != nullptr) {
			return CurrentNodo->archi[0];
		}
		return nullptr;
	case Utility::LEFT:
		if (CurrentNodo->archi[1] != nullptr) {
			return CurrentNodo->archi[1];
		}
		return nullptr;
	case Utility::DOWN:
		if (CurrentNodo->archi[2] != nullptr) {
			return CurrentNodo->archi[2];
		}
		return nullptr;
	case Utility::RIGHT:
		if (CurrentNodo->archi[3] != nullptr) {
			return CurrentNodo->archi[3];
		}
		return nullptr;
	default:
		return nullptr;
	}
}