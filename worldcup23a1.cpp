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

	Team* tempTeam = new Team(teamId, 0);
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

	Team* tempTeam = new Team(teamId, 0);
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;

	if(teamNode == nullptr) return StatusType::FAILURE;

	bool activeBefore = teamNode->value->getGoalieCount() > 0 && teamNode->value->getPlayerCount() >= 11;

	Player* tempPlayer = new Player(playerId, 0,0,0,false);
	if(this->players_by_id_tree->findValue(tempPlayer, compare_player_by_id) != nullptr){
		delete tempPlayer;
		return StatusType::FAILURE;
	}
	delete tempPlayer;


	Player* p = new Player(playerId, goals, gamesPlayed - teamNode->value->getGamesPlayed(), cards, goalKeeper);
	p->setTeam(teamNode->value);

	if(teamNode->value->getPlayerTreeById()->root != nullptr){
		Player* tempDadInTeam = teamNode->value->getPlayerTreeByScore()->findDad(p, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(p, tempDadInTeam);

		Player* tempPlayerInTeam;

		if(compareToDad < 0){
			tempPlayerInTeam = tempDadInTeam->getBelowPlayerInTeam();

			p->setAbovePlayerInTeam(tempDadInTeam);
			p->setBelowPlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setBelowPlayerInTeam(p);
			if(tempPlayerInTeam != nullptr) tempPlayerInTeam->setAbovePlayerInTeam(p);
		}
		else{
			tempPlayerInTeam = tempDadInTeam->getAbovePlayerInTeam();

			p->setBelowPlayerInTeam(tempDadInTeam);
			p->setAbovePlayerInTeam(tempPlayerInTeam);
			tempDadInTeam->setAbovePlayerInTeam(p);
			if(tempPlayerInTeam != nullptr) tempPlayerInTeam->setBelowPlayerInTeam(p);
		}
	}

	if(this->players_by_id_tree->root != nullptr){

		Player* tempDad = this->players_by_score_tree->findDad(p, compare_player_by_score)->value;
		int compareToDad = compare_player_by_score(p, tempDad);

		Player* tempPlayer;

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

	teamNode->value->setTeamStrength(teamNode->value->getTeamStrength() + goals - cards);

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

	teamNode->value->printTeam();

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId <= 0) return StatusType::INVALID_INPUT;

	Player* tempP = new Player(playerId, 0,0,0,0);
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

	playerTeam->setTeamStrength(playerTeam->getTeamStrength() - p->getGoals() + p->getCardsRecived());

	Player* pAbove = p->getAbovePlayer();
	Player* pBelow = p->getBelowPlayer();
	Player* pAboveInTeam = p->getAbovePlayerInTeam();
	Player* pBelowInTeam = p->getBelowPlayerInTeam();

	if(pAbove != nullptr) pAbove->setBelowPlayer(pBelow);
	if(pBelow != nullptr) pBelow->setAbovePlayer(pAbove);
	if(pAboveInTeam != nullptr) pAboveInTeam->setBelowPlayerInTeam(pBelowInTeam);
	if(pBelowInTeam != nullptr) pBelowInTeam->setAbovePlayerInTeam(pAboveInTeam);

	delete p;

	playerTeam->printTeam();

	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}

	Team* temp1 = new Team(teamId1, 0);
	Team* temp2 = new Team(teamId2, 0);

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

	Player* temp = new Player(playerId, 0, 0, 0, 0);
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

	Team* temp = new Team(teamId, 0);
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id); 
	delete temp;

	if(teamNode == nullptr){
		return StatusType::FAILURE;
	}

	return teamNode->value->getPoints();
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
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

	Team* temp = new Team(teamId, 0);
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

	Team* temp = new Team(teamId, 0);
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

		Player** tempArray = new Player*[this->get_all_players_count(-1).ans()];
		this->players_by_score_tree->intoArray(tempArray);

		for(int i = 0; i < this->get_all_players_count(-1).ans(); i++){
			output[i] = tempArray[i]->getId();
		}

		delete[] tempArray;
		return StatusType::SUCCESS;
	}

	Team* temp = new Team(teamId, 0);
	AVLTree<Team>::Node* teamNode = this->team_tree->findValue(temp, compare_team_by_id);
	delete temp;

	if(teamNode == nullptr) return StatusType::FAILURE;

	if(teamNode->value->getPlayerCount() == 0) return StatusType::FAILURE;

	Player** tempArray = new Player*[teamNode->value->getPlayerCount()];
	teamNode->value->getPlayerTreeByScore()->intoArray(tempArray);

	for(int i = 0; i < teamNode->value->getPlayerCount(); i++){
		output[i] = tempArray[i]->getId();
	}
	delete[] tempArray;

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}




int main(){
    world_cup_t t = world_cup_t();

	t.add_team(1, 3);

	t.add_player(1, 1, 1,2,1,0);
	t.add_player(2, 1, 5,7,3,0);
	t.add_player(3, 1, 5,1,0,0);


	t.add_team(2, 1);

	t.add_player(4, 2, 2,2,0,0);
	t.add_player(5, 2, 6,1,1,0);
	t.add_player(6, 2, 1,0,0,0);

	t.play_match(1, 2);

	t.add_player(7, 1, 1,0,0,0);

	int output[3];

	t.get_all_players(2, output);

	for(int i = 0; i < 3; i++){
		cout << output[i] << endl;
	}

    return 0;
}