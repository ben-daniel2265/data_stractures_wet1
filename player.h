#ifndef PLAYER_h
#define PLAYER_h

#include "team.h"

class Team;

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
    Player(int id, int goals, int gamesPlayed, int cardsRecived, bool isGoalie);

    void setId(int id);
    void setGoals(int goals);
    void setGamesPlayed(int gamesPlayed);
    void setCardsRecived(int cardsRecived);
    void setIsGoalie(bool isGoalie);
    void setTeam(Team *team);
    void setAbovePlayer(Player *abovePlayer);
    void setBelowPlayer(Player *belowPlayer);
    void setAbovePlayerInTeam(Player *abovePlayerInTeam);
    void setBelowPlayerInTeam(Player *belowPlayerInTeam);


    int getId() const;
    int getGoals() const;
    int getGamesPlayed() const;
    int getCardsRecived() const;
    bool isGoalie() const;
    Team* getTeam() const;
    Player *getAbovePlayer() const;
    Player *getBelowPlayer() const;
    Player *getAbovePlayerInTeam() const;
    Player *getBelowPlayerInTeam() const;



    void add_goals(int goals);
    void add_games_played(int games);
    void add_cards_recived(int cards);

    void printPlayer();
};

#endif //PLAYER_h