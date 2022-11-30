#ifndef PLAYER_cpp
#define PLAYER_cpp

#include "player.h"




Player::Player(int id, int goals, int gamesPlayed, int cardsRecived, bool isGoalie) : id(id), goals(goals),
    games_played(gamesPlayed), cards_recived(cardsRecived), is_goalie(isGoalie){

}

void Player::setId(int id) {
    Player::id = id;
}

void Player::setGoals(int goals) {
    Player::goals = goals;
}

void Player::setGamesPlayed(int gamesPlayed) {
    games_played = gamesPlayed;
}

void Player::setCardsRecived(int cardsRecived) {
    cards_recived = cardsRecived;
}

void Player::setIsGoalie(bool isGoalie) {
    is_goalie = isGoalie;
}

void Player::setTeam(Team *team) {
    Player::team = team;
}

void Player::setAbovePlayer(Player *abovePlayer) {
    above_player = abovePlayer;
}

void Player::setBelowPlayer(Player *belowPlayer) {
    below_player = belowPlayer;
}

void Player::setAbovePlayerInTeam(Player *abovePlayerInTeam) {
    above_player_in_team = abovePlayerInTeam;
}

void Player::setBelowPlayerInTeam(Player *belowPlayerInTeam) {
    below_player_in_team = belowPlayerInTeam;
}

int Player::getId() const {
    return id;
}

int Player::getGoals() const {
    return goals;
}

int Player::getGamesPlayed() const {
    return games_played;
}

int Player::getCardsRecived() const {
    return cards_recived;
}

bool Player::isGoalie() const {
    return is_goalie;
}

Team *Player::getTeam() const {
    return team;
}

Player *Player::getAbovePlayer() const {
    return above_player;
}

Player *Player::getBelowPlayer() const {
    return below_player;
}

Player *Player::getAbovePlayerInTeam() const {
    return above_player_in_team;
}

Player *Player::getBelowPlayerInTeam() const {
    return below_player_in_team;
}


void Player::add_goals(int goals) {
    this->goals+=goals;
}

void Player::add_cards_recived(int cards) {
    this->cards_recived+=cards;
}

void Player::add_games_played(int games) {
    this->games_played+=games;
}

#endif // PLAYER_cpp
