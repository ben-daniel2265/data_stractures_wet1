#ifndef PLAYER
#define PLAYER

#include "Team.h"

class Player {
    private:
    int id;
    int goals;
    int games_played;
    int cards_recived;
    bool is_goalie;

    Team * team;                        // the team the player belongs to
    Player * above_player;              // the player with the next best score from all players
    Player * below_player;              // the player with the next worst score from all players    
    Player * above_player_in_team;      // the player with the next best score from the team 
    Player * below_player_in_team;      // the player with the next worst score from the team

    public:
    Player(int id, int goals, int games_played, int cards_recived, bool is_golie);

    int get_id() const;
    int get_goals() const;
    int get_games_played() const;
    int get_cards_recived() const;
    bool is_goalie() const;

    Team& get_team() const;
    Player& get_above_player() const;
    Player& get_below_player() const;
    Player& get_above_player_in_team() const;
    Player& get_below_player_in_team() const;

    void add_goals(int goals);
    void add_games_played(int games);
    void add_cards_recived(int cards);

    void set_team(Team * team);
    void set_above_player(Player * player);
    void set_below_player(Player * player);
    void set_above_player_in_team(Player * player);
    void set_below_player_in_team(Player * player);
};


#endif //PLAYER