#ifndef TEAM_cpp
#define TEAM_cpp

#include "team.h"

int Team::getId() const {
    return id;
}

int Team::getPoints() const {
    return points;
}

int Team::getGamesPlayed() const {
    return games_played;
}

int Team::getTeamStrength() const {
    return team_strength;
}

int Team::getPlayerCount() const {
    return player_count;
}

int Team::getGoalieCount() const {
    return goalie_count;
}

AVLTree<Player> *Team::getPlayerTreeById() const {
    return player_tree_by_id;
}

AVLTree<Player> *Team::getPlayerTreeByScore() const {
    return player_tree_by_score;
}

Player *Team::getTopScorer() const {
    return top_scorer;
}

void Team::setId(int id) {
    Team::id = id;
}

void Team::setPoints(int points) {
    Team::points = points;
}

void Team::setGamesPlayed(int gamesPlayed) {
    games_played = gamesPlayed;
}

void Team::setTeamStrength(int teamStrength) {
    team_strength = teamStrength + points;
}

void Team::setPlayerCount(int playerCount) {
    player_count = playerCount;
}

void Team::setGoalieCount(int goalieCount) {
    goalie_count = goalieCount;
}

void Team::setPlayerTreeById(AVLTree<Player> *playerTreeById) {
    player_tree_by_id = playerTreeById;
}

void Team::setPlayerTreeByScore(AVLTree<Player> *playerTreeByScore) {
    player_tree_by_score = playerTreeByScore;
}

void Team::setTopScorer(Player *topScorer) {
    top_scorer = topScorer;
}

Team::Team(int id, int points) : id(id), points(points) {
    games_played=0;
    team_strength=0;
    player_count=0;
    goalie_count=0;
    player_tree_by_id=new AVLTree<Player>();
    player_tree_by_score=new AVLTree<Player>();
    top_scorer=nullptr;

}

Team::~Team()
{
    player_tree_by_id->delete_hollow_tree();
    player_tree_by_score->delete_hollow_tree();
}

void printPlayer(Player* p){
    p->printPlayer();
}

void Team::printTeam(){
    cout << "(Id: " << this->id << ", Player count: " << this->player_count << ", Strength: " <<  this->team_strength <<  ")" << endl;

    cout << endl;

    cout << "Players by id:" << endl;
    this->player_tree_by_id->doOnTreeInOrder(printPlayer);

    cout << endl;

    cout << "Players by score:" << endl;
    this->player_tree_by_score->doOnTreeInOrder(printPlayer);

    cout << endl;
}




#endif //TEAM_cpp
