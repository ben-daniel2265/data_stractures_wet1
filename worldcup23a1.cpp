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
		return player1->getCardsRecived() - player2->getCardsRecived();
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

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
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
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
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

	t.add_team(1, 0);
	t.add_player(1, 1, 0,0,0,0);
	
	StatusType s = t.add_player(1, 1, 0,0,0,0);

    if(s == StatusType::SUCCESS){
		cout << "success";
	}

    return 0;
}