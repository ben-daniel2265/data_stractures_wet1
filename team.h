#ifndef TEAM
#define TEAM

#include "AVLTree.h"
#include "player.h"

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

    int get_id();
    int get_points();
    int getgames_played();
    int get_team_strength();
    int get_player_count();
    int get_goalie_count();

    AVLTree<Player>& get_player_tree_by_id();
    AVLTree<Player>& get_player_tree_by_score();

    Player& get_top_scorer();
};


#endif //TEAM