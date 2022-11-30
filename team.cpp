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
    team_strength = teamStrength;
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
