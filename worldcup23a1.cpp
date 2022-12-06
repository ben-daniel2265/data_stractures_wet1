#include "worldcup23a1.h"

int compare_team_by_id(Team* team1, Team* team2){
	return team1->getId() - team2->getId(); 
}

int compare_player_by_id(Player* player1, Player* player2){
	return player1->getId() - player2->getId();
}

int compare_player_by_score(Player* player1, Player* player2){
	if(player1->getGoals() != player2->getGoals()){
		return player1->getGoals() - player2->getGoals();
	}
	else if(player1->getCardsRecived() != player2->getCardsRecived()){
		return player2->getCardsRecived() - player1->getCardsRecived();
	}
	else{
		return player1->getId() - player2->getId();
	}
}

world_cup_t::world_cup_t()
{
	this->team_tree = new AVLTree<Team>();
	this->players_by_id_tree = new AVLTree<Player>();
	this->players_by_score_tree = new AVLTree<Player>();
	this->active_teams = new AVLTree<Team>();

	this->player_count = 0;
	this->top_scorer = nullptr;
}

world_cup_t::~world_cup_t()
{
	this->players_by_id_tree->delete_values();
	this->team_tree->delete_values();

	this->players_by_id_tree->delete_hollow_tree();
	this->team_tree->delete_hollow_tree();
	this->players_by_score_tree->delete_hollow_tree();
	this->active_teams->delete_hollow_tree();

	delete players_by_id_tree;
	delete team_tree;
	delete players_by_score_tree;
	delete active_teams;
}

StatusType world_cup_t::add_team(int teamId, int points)
{
	if(teamId <= 0 || points < 0){
		return StatusType::INVALID_INPUT;
	}

	Team* team;

	try{
		team = new Team(teamId, points);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	if(this->team_tree->findValue(team, compare_team_by_id) != nullptr){
		delete team;
		return StatusType::FAILURE;
	}

	this->team_tree->insertValue(team, compare_team_by_id); 

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}


	Team* tempTeam;

	try{
		tempTeam = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}


	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;


	if(teamNode == nullptr){
		return StatusType::FAILURE;
	}
	else if(teamNode->value->getPlayerCount() == 0){
		Team* team = teamNode->value;
		this->team_tree->removeValue(team, compare_team_by_id);
		delete team;

		return StatusType::SUCCESS;
	}

	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	if(teamId <= 0 || playerId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0){
		return StatusType::INVALID_INPUT;
	}

	if(gamesPlayed == 0 && (cards > 0 || goals > 0)){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam;
	try{
		tempTeam = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;

	if(teamNode == nullptr) return StatusType::FAILURE;

	bool activeBefore = teamNode->value->getGoalieCount() > 0 && teamNode->value->getPlayerCount() >= 11;

	Player* tempPlayer;
	try{
		tempPlayer = new Player(playerId, 0,0,0,false);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	if(this->players_by_id_tree->findValue(tempPlayer, compare_player_by_id) != nullptr){
		delete tempPlayer;
		return StatusType::FAILURE;
	}
	delete tempPlayer;

	Player* p;
	try{
		p = new Player(playerId, goals, gamesPlayed - teamNode->value->getGamesPlayed(), cards, goalKeeper);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	p->setTeam(teamNode->value);

	if(teamNode->value->getPlayerTreeById()->root != nullptr){

		Player* tempDadInTeam = teamNode->value->getPlayerTreeByScore()->findDad(p, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(p, tempDadInTeam);
		Player* tempPlayerInTeam = nullptr;

	
		if(compareToDad < 0){
			tempPlayerInTeam = tempDadInTeam->getBelowPlayerInTeam();
		
			p->setAbovePlayerInTeam(tempDadInTeam);
			p->setBelowPlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setBelowPlayerInTeam(p);

			if(tempPlayerInTeam != nullptr) {
				tempPlayerInTeam->setAbovePlayerInTeam(p);}
		}
		else{
			tempPlayerInTeam = tempDadInTeam->getAbovePlayerInTeam();

			p->setBelowPlayerInTeam(tempDadInTeam);
			p->setAbovePlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setAbovePlayerInTeam(p);;
			if(tempPlayerInTeam != nullptr) tempPlayerInTeam->setBelowPlayerInTeam(p);

		}
	}

	if(this->players_by_id_tree->root != nullptr){
		Player* tempDad = this->players_by_score_tree->findDad(p, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(p, tempDad);

		Player* tempPlayer = nullptr;

		if(compareToDad < 0){
			tempPlayer = tempDad->getBelowPlayer();

			p->setAbovePlayer(tempDad);
			p->setBelowPlayer(tempPlayer);
			tempDad->setBelowPlayer(p);
			if(tempPlayer != nullptr) tempPlayer->setAbovePlayer(p);
		}
		else{
			tempPlayer = tempDad->getAbovePlayer();

			p->setBelowPlayer(tempDad);
			p->setAbovePlayer(tempPlayer);
			tempDad->setAbovePlayer(p);
			if(tempPlayer != nullptr) tempPlayer->setBelowPlayer(p);
		}
	}


	teamNode->value->getPlayerTreeById()->insertValue(p, compare_player_by_id);
	teamNode->value->getPlayerTreeByScore()->insertValue(p, compare_player_by_score);
	this->players_by_id_tree->insertValue(p, compare_player_by_id);
	this->players_by_score_tree->insertValue(p, compare_player_by_score);



	teamNode->value->setPlayerCount(teamNode->value->getPlayerCount() + 1);
	this->player_count++;

	teamNode->value->addStrength(goals - cards);

	if(goalKeeper){
		teamNode->value->setGoalieCount(teamNode->value->getGoalieCount() + 1);
	}

	if(teamNode->value->getTopScorer() != nullptr){
		if(compare_player_by_score(p, teamNode->value->getTopScorer()) > 0){
			teamNode->value->setTopScorer(p);
		}
	}
	else{
		teamNode->value->setTopScorer(p);
	}

	if(this->top_scorer != nullptr){
		if(compare_player_by_score(p, this->top_scorer) > 0){
			this->top_scorer = p;
		}
	}
	else{
		this->top_scorer = p;
	}

	bool activeAfter = teamNode->value->getGoalieCount() > 0 && teamNode->value->getPlayerCount() >= 11;

	if(!activeBefore && activeAfter){
		this->active_teams->insertValue(teamNode->value, compare_team_by_id);
	}


	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId <= 0) return StatusType::INVALID_INPUT;

	Player* tempP;
	try{
		tempP = new Player(playerId, 0,0,0,false);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Player>::Node* playerNode = this->players_by_id_tree->findValue(tempP, compare_player_by_id);
	delete tempP;


	if(playerNode == nullptr) return StatusType::FAILURE;

	Player* p = playerNode->value;
	Team* playerTeam = p->getTeam();


	playerTeam->getPlayerTreeById()->removeValue(p, compare_player_by_id);
	playerTeam->getPlayerTreeByScore()->removeValue(p, compare_player_by_score);
	this->players_by_id_tree->removeValue(p, compare_player_by_id);
	this->players_by_score_tree->removeValue(p, compare_player_by_score);

	if(compare_player_by_score(p, playerTeam->getTopScorer()) == 0){
		playerTeam->setTopScorer(p->getBelowPlayerInTeam());
	}

	if(compare_player_by_score(p, this->top_scorer) == 0){
		this->top_scorer = p->getBelowPlayer();
	}

	bool activeBefore = playerTeam->getGoalieCount() > 0 && playerTeam->getPlayerCount() >= 11;

	playerTeam->setPlayerCount(playerTeam->getPlayerCount() - 1);
	this->player_count--;

	if(p->isGoalie()){
		playerTeam->setGoalieCount(playerTeam->getGoalieCount() - 1);
	}

	bool activeAfter = playerTeam->getGoalieCount() > 0 && playerTeam->getPlayerCount() >= 11;

	if(activeBefore && !activeAfter){
		this->active_teams->removeValue(playerTeam, compare_team_by_id);
	}

	playerTeam->addStrength(p->getCardsRecived() - p->getGoals());

	Player* pAbove = p->getAbovePlayer();
	Player* pBelow = p->getBelowPlayer();
	Player* pAboveInTeam = p->getAbovePlayerInTeam();
	Player* pBelowInTeam = p->getBelowPlayerInTeam();

	if(pAbove != nullptr) pAbove->setBelowPlayer(pBelow);
	if(pBelow != nullptr) pBelow->setAbovePlayer(pAbove);
	if(pAboveInTeam != nullptr) pAboveInTeam->setBelowPlayerInTeam(pBelowInTeam);
	if(pBelowInTeam != nullptr) pBelowInTeam->setAbovePlayerInTeam(pAboveInTeam);

	delete p;

	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0){
		return StatusType::INVALID_INPUT;
	}

	Player* tempP;
	try{
		tempP = new Player(playerId, 0,0,0,false);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Player>::Node* playerNode = this->players_by_id_tree->findValue(tempP, compare_player_by_id);
	delete tempP;

	if(playerNode == nullptr) return StatusType::FAILURE;

	int sumGames = gamesPlayed + playerNode->value->getGamesPlayed() + playerNode->value->getTeam()->getGamesPlayed();
	int sumGoals = scoredGoals + playerNode->value->getGoals();
	int sumCards = cardsReceived + playerNode->value->getCardsRecived();
	bool isGoalie = playerNode->value->isGoalie();


	Player* updatedPlayer;
	try{
		updatedPlayer = new Player(playerId, sumGoals, sumGames, sumCards, isGoalie);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	updatedPlayer->setTeam(playerNode->value->getTeam());

	this->remove_player(playerId);

	bool activeBefore = updatedPlayer->getTeam()->getGoalieCount() > 0 && updatedPlayer->getTeam()->getPlayerCount() >= 11;

	if(updatedPlayer->getTeam()->getPlayerTreeById()->root != nullptr){

		Player* tempDadInTeam = updatedPlayer->getTeam()->getPlayerTreeByScore()->findDad(updatedPlayer, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(updatedPlayer, tempDadInTeam);

		Player* tempPlayerInTeam;
	

		if(compareToDad < 0){
			tempPlayerInTeam = tempDadInTeam->getBelowPlayerInTeam();
		
			updatedPlayer->setAbovePlayerInTeam(tempDadInTeam);
			updatedPlayer->setBelowPlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setBelowPlayerInTeam(updatedPlayer);
			if(tempPlayerInTeam != nullptr) tempPlayerInTeam->setAbovePlayerInTeam(updatedPlayer);
		}					
		else{
			tempPlayerInTeam = tempDadInTeam->getAbovePlayerInTeam();

			updatedPlayer->setBelowPlayerInTeam(tempDadInTeam);
			updatedPlayer->setAbovePlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setAbovePlayerInTeam(updatedPlayer);
			if(tempPlayerInTeam != nullptr) tempPlayerInTeam->setBelowPlayerInTeam(updatedPlayer);
		}
	}


	if(this->players_by_id_tree->root != nullptr){
		Player* tempDad = this->players_by_score_tree->findDad(updatedPlayer, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(updatedPlayer, tempDad);

		Player* tempPlayer;

		if(compareToDad < 0){
			tempPlayer = tempDad->getBelowPlayer();

			updatedPlayer->setAbovePlayer(tempDad);
			updatedPlayer->setBelowPlayer(tempPlayer);
			tempDad->setBelowPlayer(updatedPlayer);
			if(tempPlayer != nullptr) tempPlayer->setAbovePlayer(updatedPlayer);
		}
		else{
			tempPlayer = tempDad->getAbovePlayer();

			updatedPlayer->setBelowPlayer(tempDad);
			updatedPlayer->setAbovePlayer(tempPlayer);
			tempDad->setAbovePlayer(updatedPlayer);
			if(tempPlayer != nullptr) tempPlayer->setBelowPlayer(updatedPlayer);
		}
	}


	updatedPlayer->getTeam()->getPlayerTreeById()->insertValue(updatedPlayer, compare_player_by_id);
	updatedPlayer->getTeam()->getPlayerTreeByScore()->insertValue(updatedPlayer, compare_player_by_score);
	this->players_by_id_tree->insertValue(updatedPlayer, compare_player_by_id);
	this->players_by_score_tree->insertValue(updatedPlayer, compare_player_by_score);



	updatedPlayer->getTeam()->setPlayerCount(updatedPlayer->getTeam()->getPlayerCount() + 1);
	this->player_count++;

	updatedPlayer->getTeam()->addStrength(sumGoals - sumCards);

	if(isGoalie){
		updatedPlayer->getTeam()->setGoalieCount(updatedPlayer->getTeam()->getGoalieCount() + 1);
	}

	if(updatedPlayer->getTeam()->getTopScorer() != nullptr){
		if(compare_player_by_score(updatedPlayer, updatedPlayer->getTeam()->getTopScorer()) > 0){
			updatedPlayer->getTeam()->setTopScorer(updatedPlayer);
		}
	}
	else{
		updatedPlayer->getTeam()->setTopScorer(updatedPlayer);
	}

	if(this->top_scorer != nullptr){
		if(compare_player_by_score(updatedPlayer, this->top_scorer) > 0){
			this->top_scorer = updatedPlayer;
		}
	}
	else{
		this->top_scorer = updatedPlayer;
	}

	bool activeAfter = updatedPlayer->getTeam()->getGoalieCount() > 0 && updatedPlayer->getTeam()->getPlayerCount() >= 11;

	if(!activeBefore && activeAfter){
		this->active_teams->insertValue(updatedPlayer->getTeam(), compare_team_by_id);
	}

	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}

	Team* temp1;
	Team* temp2;
	try{
		temp1 = new Team(teamId1, 0);
		temp2 = new Team(teamId2, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	AVLTree<Team>::Node* teamNode1 = this->team_tree->findValue(temp1, compare_team_by_id);
	AVLTree<Team>::Node* teamNode2 = this->team_tree->findValue(temp2, compare_team_by_id);

	delete temp1;
	delete temp2;

	if(teamNode1 == nullptr || teamNode2 == nullptr){
		return StatusType::FAILURE;
	}

	bool isActive1 = teamNode1->value->getGoalieCount() > 0 && teamNode1->value->getPlayerCount() >= 11;
	bool isActive2 = teamNode2->value->getGoalieCount() > 0 && teamNode2->value->getPlayerCount() >= 11;

	if(!isActive1 || !isActive2){
		return StatusType::FAILURE;
	}

	int matchResult = teamNode1->value->getTeamStrength() - teamNode2->value->getTeamStrength();

	teamNode1->value->setGamesPlayed(teamNode1->value->getGamesPlayed() + 1);
	teamNode2->value->setGamesPlayed(teamNode2->value->getGamesPlayed() + 1);

	if(matchResult == 0){
		teamNode1->value->setPoints(teamNode1->value->getPoints() + 1);
		teamNode2->value->setPoints(teamNode2->value->getPoints() + 1);
	}
	else if(matchResult > 0){
		teamNode1->value->setPoints(teamNode1->value->getPoints() + 3);
	}
	else{
		teamNode2->value->setPoints(teamNode2->value->getPoints() + 3);
	}

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Player* temp;
	try{
		temp = new Player(playerId, 0, 0, 0, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Player>::Node* playerNode = this->players_by_id_tree->findValue(temp, compare_player_by_id); 
	delete temp;

	if(playerNode == nullptr){
		return StatusType::FAILURE;
	}

	return playerNode->value->getGamesPlayed() + playerNode->value->getTeam()->getGamesPlayed();
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* temp;
	try{
		temp = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id); 
	delete temp;

	if(teamNode == nullptr){
		return StatusType::FAILURE;
	}

	return teamNode->value->getPoints();
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* temp1;
	Team* temp2;
	try{
		temp1 = new Team(teamId1, 0);
		temp2 = new Team(teamId2, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	AVLTree<Team>::Node* teamNode1 = this->team_tree->findValue(temp1, compare_team_by_id);
	AVLTree<Team>::Node* teamNode2 = this->team_tree->findValue(temp2, compare_team_by_id);

	delete temp1;
	delete temp2;

	if(teamNode1 == nullptr || teamNode2 == nullptr){
		return StatusType::FAILURE;
	}

	Team* tempTeam;
	try{
		tempTeam = new Team(newTeamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Team>::Node* tempNode = this->team_tree->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;

	if(tempNode != nullptr && !((teamId1 == newTeamId) || (teamId2 == newTeamId))){
		return StatusType::FAILURE;
	}

	bool teamActive1 = teamNode1->value->getGoalieCount() > 0 && teamNode1->value->getPlayerCount() >= 11;
	bool teamActive2 = teamNode2->value->getGoalieCount() > 0 && teamNode2->value->getPlayerCount() >= 11;

	if(teamActive1){
		this->active_teams->removeValue(teamNode1->value, compare_team_by_id);
	}
	if(teamActive2){
		this->active_teams->removeValue(teamNode2->value, compare_team_by_id);
	}

	int countPlayers1 = teamNode1->value->getPlayerCount();
	int countPlayers2 = teamNode2->value->getPlayerCount();
	int sumStrength = teamNode1->value->getTeamStrength() + teamNode2->value->getTeamStrength();
	int sumPoints = teamNode1->value->getPoints() + teamNode2->value->getPoints();
	int sumPlayers = countPlayers1 + countPlayers2;
	int sumGoalies = teamNode1->value->getGoalieCount() + teamNode2->value->getGoalieCount();

	int gamesPlayed1 = teamNode1->value->getGamesPlayed();
	int gamesPlayed2 = teamNode2->value->getGamesPlayed();

	Team* newTeam;
	try{
		newTeam = new Team(newTeamId, sumPoints);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	newTeam->setTeamStrength(sumStrength);
	newTeam->setPlayerCount(sumPlayers);
	newTeam->setGoalieCount(sumGoalies);

	if(sumPlayers == 0){
		this->remove_team(teamId1);
		this->remove_team(teamId2);
		this->team_tree->insertValue(newTeam, compare_team_by_id);

		return StatusType::SUCCESS;
	}


	Player** playersArrayById1;
	Player** playersArrayByScore1;
	Player** playersArrayById2;
	Player** playersArrayByScore2;
	Player** newPlayersArrayById;
	Player** newPlayersArrayByScore;
	
	try{
		playersArrayById1 = new Player*[countPlayers1];
		playersArrayByScore1 = new Player*[countPlayers1];
		playersArrayById2 = new Player*[countPlayers2];
		playersArrayByScore2 = new Player*[countPlayers2];
		newPlayersArrayById = new Player*[sumPlayers];
		newPlayersArrayByScore = new Player*[sumPlayers];
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	teamNode1->value->getPlayerTreeById()->intoArray(playersArrayById1);
	teamNode1->value->getPlayerTreeByScore()->intoArray(playersArrayByScore1);
	teamNode2->value->getPlayerTreeById()->intoArray(playersArrayById2);
	teamNode2->value->getPlayerTreeByScore()->intoArray(playersArrayByScore2);

	teamNode1->value->setPlayerCount(0);
	teamNode2->value->setPlayerCount(0);

	teamNode1 = nullptr;
	teamNode2 = nullptr;

	this->remove_team(teamId1);
	this->remove_team(teamId2);

	for(int i = 0; i < countPlayers1; i++){
		playersArrayById1[i]->add_games_played(gamesPlayed1);
	}

	for(int i = 0; i < countPlayers2; i++){
		playersArrayById2[i]->add_games_played(gamesPlayed2);
	}

	int index1 = 0;
	int index2 = 0;
	int compareResult;

	while(index1 < countPlayers1 && index2 < countPlayers2){
		compareResult = compare_player_by_id(playersArrayById1[index1], playersArrayById2[index2]);

		if(compareResult > 0){
			newPlayersArrayById[index1 + index2] = playersArrayById2[index2];
			index2++;
		}
		else{
			newPlayersArrayById[index1 + index2] = playersArrayById1[index1];
			index1++;
		}
	}

	if(index1 == countPlayers1){
		while(index2 < countPlayers2){
			newPlayersArrayById[index1 + index2] = playersArrayById2[index2];
			index2++;
		}
	}
	else{
		while(index1 < countPlayers1){
			newPlayersArrayById[index1 + index2] = playersArrayById1[index1];
			index1++;
		}
	}

	index1 = 0;
	index2 = 0;

	while(index1 < countPlayers1 && index2 < countPlayers2){
		compareResult = compare_player_by_score(playersArrayByScore1[index1], playersArrayByScore2[index2]);

		if(compareResult > 0){
			newPlayersArrayByScore[index1 + index2] = playersArrayByScore2[index2];
			index2++;
		}
		else{
			newPlayersArrayByScore[index1 + index2] = playersArrayByScore1[index1];
			index1++;
		}
	}

	if(index1 == countPlayers1){
		while(index2 < countPlayers2){
			newPlayersArrayByScore[index1 + index2] = playersArrayByScore2[index2];
			index2++;
		}
	}
	else{
		while(index1 < countPlayers1){
			newPlayersArrayByScore[index1 + index2] = playersArrayByScore1[index1];
			index1++;
		}
	}

	delete[] playersArrayById1;
	delete[] playersArrayByScore1;
	delete[] playersArrayById2;
	delete[] playersArrayByScore2;

	newPlayersArrayByScore[0]->setBelowPlayerInTeam(nullptr);
	newPlayersArrayByScore[0]->setAbovePlayerInTeam(newPlayersArrayByScore[1]);
	newPlayersArrayByScore[0]->setTeam(newTeam);
	newPlayersArrayByScore[sumPlayers-1]->setBelowPlayerInTeam(newPlayersArrayByScore[sumPlayers-2]);
	newPlayersArrayByScore[sumPlayers-1]->setAbovePlayerInTeam(nullptr);
	newPlayersArrayByScore[sumPlayers-1]->setTeam(newTeam);

	for(int i = 1; i < sumPlayers - 1; i++){
		newPlayersArrayByScore[i]->setBelowPlayerInTeam(newPlayersArrayByScore[i-1]);
		newPlayersArrayByScore[i]->setAbovePlayerInTeam(newPlayersArrayByScore[i+1]);
		newPlayersArrayByScore[i]->setTeam(newTeam);
	}

	newTeam->getPlayerTreeById()->arrayToAVLTree(newPlayersArrayById, sumPlayers);
	newTeam->getPlayerTreeByScore()->arrayToAVLTree(newPlayersArrayByScore, sumPlayers);

	this->team_tree->insertValue(newTeam, compare_team_by_id);

	if(newTeam->getGoalieCount() > 0 && newTeam->getPlayerCount() >= 11){
		this->active_teams->insertValue(newTeam, compare_team_by_id);
	}

	delete[] newPlayersArrayById;
	delete[] newPlayersArrayByScore;

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if(teamId == 0){
		return StatusType::INVALID_INPUT;
	}

	if(teamId < 0){
		if(this->top_scorer == nullptr) return StatusType::FAILURE;

		return this->top_scorer->getId();
	}

	Team* temp;
	try{
		temp = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id);
	delete temp;

	if(teamNode == nullptr) return StatusType::FAILURE;
	if(teamNode->value->getTopScorer() == nullptr) return StatusType::FAILURE;

	return teamNode->value->getTopScorer()->getId();
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if(teamId == 0){
		return StatusType::INVALID_INPUT;
	}

	if(teamId < 0){
		return this->player_count;
	}

	Team* temp;
	try{
		temp = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id);
	delete temp;

	if(teamNode == nullptr) return StatusType::FAILURE;

	return teamNode->value->getPlayerCount();
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if(teamId == 0 || output == nullptr) return StatusType::INVALID_INPUT;

	if(teamId < 0){
		if(this->get_all_players_count(-1).ans() == 0) return StatusType::FAILURE;

		Player** tempArray;
		try{
			tempArray = new Player*[this->get_all_players_count(-1).ans()];
		}
		catch(std::exception e){
			return StatusType::ALLOCATION_ERROR;
		}
		this->players_by_score_tree->intoArray(tempArray);

		for(int i = 0; i < this->get_all_players_count(-1).ans(); i++){
			output[i] = tempArray[i]->getId();
		}

		delete[] tempArray;
		return StatusType::SUCCESS;
	}

	Team* temp;
	try{
		temp = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id);
	delete temp;

	if(teamNode == nullptr) return StatusType::FAILURE;

	if(teamNode->value->getPlayerCount() == 0) return StatusType::FAILURE;

	Player** tempArray;
		try{
			tempArray = new Player*[teamNode->value->getPlayerCount()];
		}
		catch(std::exception e){
			return StatusType::ALLOCATION_ERROR;
		}
	teamNode->value->getPlayerTreeByScore()->intoArray(tempArray);

	for(int i = 0; i < teamNode->value->getPlayerCount(); i++){
		output[i] = tempArray[i]->getId();
	}
	delete[] tempArray;

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	if(teamId <= 0 || playerId <= 0){
		return StatusType::INVALID_INPUT;
	}


	Team* tempTeam;
	try{
		tempTeam = new Team(teamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;

	if(teamNode == nullptr) return StatusType::FAILURE;

	Player* tempP;
	try{
		tempP = new Player(playerId, 0,0,0,0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	AVLTree<Player>::Node* playerNode = teamNode->value->getPlayerTreeById()->findValue(tempP, compare_player_by_id);
	delete tempP;

	if(playerNode == nullptr) return StatusType::FAILURE;

	if(playerNode->value->getAbovePlayer() == nullptr && playerNode->value->getBelowPlayer() == nullptr){
		return StatusType::FAILURE;
	}
	else if(playerNode->value->getAbovePlayer() == nullptr){
		return playerNode->value->getBelowPlayer()->getId();
	}
	else if(playerNode->value->getBelowPlayer() == nullptr){
		return playerNode->value->getAbovePlayer()->getId();
	}
	else{
		int idAbove = playerNode->value->getAbovePlayer()->getId();
		int idBelow = playerNode->value->getBelowPlayer()->getId();
	
		int distAbove = abs(playerNode->value->getGoals() - playerNode->value->getAbovePlayer()->getGoals());
		int distBelow = abs(playerNode->value->getGoals() - playerNode->value->getBelowPlayer()->getGoals());

		if(distAbove == distBelow){
			distAbove = abs(playerNode->value->getCardsRecived() - playerNode->value->getAbovePlayer()->getCardsRecived());
			distBelow = abs(playerNode->value->getCardsRecived() - playerNode->value->getBelowPlayer()->getCardsRecived());

			if(distAbove == distBelow){
				distAbove = abs(playerNode->value->getId() - playerNode->value->getAbovePlayer()->getId());
				distBelow = abs(playerNode->value->getId() - playerNode->value->getBelowPlayer()->getId());

				if(distAbove == distBelow) return idAbove > idBelow ? idAbove : idBelow;

				return distAbove > distBelow ? idBelow : idAbove;
			}

			return distAbove > distBelow ? idBelow : idAbove;
		}

		return distAbove > distBelow ? idBelow : idAbove;
	}

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{

	//cout << "knockout: " << minTeamId << "-" << maxTeamId << endl;
	if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId){
		return StatusType::INVALID_INPUT;
	}

	Team* temp1;
	Team* temp2;
	try{
		temp1  = new Team(minTeamId, 0);
		temp2 = new Team(maxTeamId, 0);
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}
	int numTeams = this->active_teams->countInRange(temp1, temp2, compare_team_by_id);

	if(numTeams == 0){
		delete temp1;
		delete temp2;
		return StatusType::FAILURE;
	}

	Team** teams;
	try{
		teams = new Team*[numTeams];
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	this->active_teams->rangedIntoArray(teams, temp1, temp2, compare_team_by_id);

	delete temp1;
	delete temp2;

	int* teamsId;
	int* teamsStrength;
	try{
		teamsId = new int[numTeams];
		teamsStrength = new int [numTeams];
	}
	catch(std::exception e){
		return StatusType::ALLOCATION_ERROR;
	}

	for(int i = 0; i < numTeams; i++){
		teamsId[i] = teams[i]->getId();
		teamsStrength[i] = teams[i]->getTeamStrength();
	//	cout << "team: " << teamsId[i] << " strength: " << teamsStrength[i] << endl;
	}

	delete[] teams;

	int jumpSize = 2;
	int index = 0;
	int matchResult = 0;

	while(jumpSize / 2 < numTeams){
		while(jumpSize*index + jumpSize / 2 < numTeams){
			matchResult = teamsStrength[index*jumpSize] - teamsStrength[index*jumpSize + jumpSize / 2];
			teamsStrength[index*jumpSize] += teamsStrength[index*jumpSize + jumpSize / 2] + 3;

			if(matchResult <= 0){
				teamsId[index*jumpSize] = teamsId[index*jumpSize + jumpSize / 2];
			}
			index++;
		}
		index = 0;
		jumpSize *= 2;
	}

	int finalId = teamsId[0];

	delete[] teamsId;
	delete[] teamsStrength;

	return finalId;
}
