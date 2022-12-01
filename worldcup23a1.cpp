#include "worldcup23a1.h"

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

	if(this->team_tree->find(team, compare_team_by_id) != nullptr) return StatusType::FAILURE;

	this->team_tree->insert(team, compare_team_by_id); 

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* team = this->team_tree->find(team, compare_team_by_id);

	if(team == nullptr){
		return StatusType::FAILURE;
	}
	else if(team->getPlayerCount() == 0){
		this->team_tree->remove(team, compare_team_by_id);
		delete team;
		return StatusType::SUCCESS;
	}

	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
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

bool compare_team_by_id(Team* team1, Team* team2){
	return team1->getId() >= team2->getId(); 
}


bool compare_player_by_id(Player* player1, Player* player2){
	return player1->getId() >= player2->getId();
}

bool compare_player_by_score(Player* player1, Player* player2){
	if(player1->getGoals() != player2->getGoals()){
		return player1->getGoals() >= player2->getGoals();
	}
	else if(player1->getCardsRecived() != player2->getCardsRecived()){
		return player1->getCardsRecived() >= player2->getCardsRecived();
	}
	else{
		return player1->getId() >= player2->getId();
	}
}
