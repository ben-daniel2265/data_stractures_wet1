#ifndef TEAM_h
#define TEAM_h

#include "AVLTree.h"
#include "player.h"

class Player;

class Team {
    private:
    int id;
    int points;
    int games_played;
    int team_strength;
    int player_count;
    int goalie_count;

    AVLTree<Player> * player_tree_by_id;
    AVLTree<Player> * player_tree_by_score;

    Player * top_scorer;

    public:

    Team(int id, int points);
    int getId() const;
    int getPoints() const;
    int getGamesPlayed() const;
    int getTeamStrength() const;
    int getPlayerCount() const;
    int getGoalieCount() const;
    AVLTree<Player> *getPlayerTreeById() const;
    AVLTree<Player> *getPlayerTreeByScore() const;
    Player *getTopScorer() const;

    void setId(int id);
    void setPoints(int points);
    void setGamesPlayed(int gamesPlayed);
    void setTeamStrength(int teamStrength);
    void setPlayerCount(int playerCount);
    void setGoalieCount(int goalieCount);
    void setPlayerTreeById(AVLTree<Player> *playerTreeById);
    void setPlayerTreeByScore(AVLTree<Player> *playerTreeByScore);
    void setTopScorer(Player *topScorer);

    void printTeam();
};


#endif //TEAM_h