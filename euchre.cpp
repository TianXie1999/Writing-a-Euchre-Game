#include"Player.h"
#include"Pack.h"
#include<iostream>
#include<fstream>
#include<algorithm>
#include<string>
#include <cstdlib>
using namespace std;

class Game {
public:
    //default constructor
    Game() :trump(""){
        for (int i = 0; i < 2; i++) {
            score[i] = 0;  
        }
    }

    //nondefault
    Game(string name[4], string type[4], istream &input):trump("") {
        //initialize players
        for (int i = 0; i < 4; i++) {
            player[i] = Player_factory(name[i], type[i]);
        }

        //initialize scores
        for (int i = 0; i < 2; i++) {
            score[i] = 0;
        }

        pack = Pack(input);
        
    }

    //function for shuffling
    void shuffle(string kind) {
        if (kind == "shuffle") { 
            pack.shuffle(); 
        }
        return;
    }


    //function for dealing
    Card dealing(int hand) {
        
        cout << "Hand " << hand << endl;
        //index of the first batch
        int index = hand % 4;
        cout << player[index]->get_name() << " deals" << endl;
        //first round of deal
        for (int i = index + 1; i < index + 5; i++) {
            
            if ((i - index) % 2 == 1) {
             player[i % 4]->add_card(pack.deal_one());
             player[i % 4]->add_card(pack.deal_one());
             player[i % 4]->add_card(pack.deal_one());
            }
            else {
             player[i % 4]->add_card(pack.deal_one());
             player[i % 4]->add_card(pack.deal_one());
            }
        }

        //second round of deal
        for (int i = index + 1; i < index + 5; i++) {
            if ((i - index) % 2 == 1) {
                player[i % 4]->add_card(pack.deal_one());
                player[i % 4]->add_card(pack.deal_one());
                
            }
            else {
                player[i % 4]->add_card(pack.deal_one());
                player[i % 4]->add_card(pack.deal_one());
                player[i % 4]->add_card(pack.deal_one());
            }
        }
        Card upCard = pack.deal_one();

        cout << upCard << " turned up" << endl;
        return upCard;
    }

    Card deal_one() {
        return pack.deal_one();
    }

    //function for making trump
    int trump_make(int &hand, int &round, Card &upCard){
        int maker = 5;
        bool make = false;
        
        for (int i = hand + 1; i < hand + 5; i++) {            
            int is_dealer = 0;
            if (i % 4 == hand)is_dealer = 1;
            
            make = player[i%4]->make_trump(upCard, is_dealer, round, trump);

            if (make) {
                cout << player[i % 4]->get_name() << " orders up " << trump << endl;
                if (round == 1)player[hand % 4]->add_and_discard(upCard);
                cout << endl;
                maker = i%4;
                break;
            }

            else cout << player[i%4]->get_name() << " passes" << endl;
            
        }

        return maker;
    }

    //function for one trick
    int trick(int lead_index,int trick[]) {
        array<Card, 4> trickCards;
        trickCards[lead_index] = player[lead_index]->lead_card(trump);

        cout << trickCards[lead_index] << " led by " << player[lead_index]->get_name() << endl;

        int max = lead_index;

        for (int i = lead_index + 1; i < lead_index + 4; i++) {

            trickCards[i % 4] = player[i % 4]->play_card(trickCards[lead_index], trump);

            cout << trickCards[i%4] << " played by " << player[i%4]->get_name() << endl;

            if (Card_less(trickCards[max],trickCards[i % 4], trickCards[lead_index],trump)) {
               max = i%4;
            }
        }

        cout << player[max]->get_name() << " takes the trick" << endl << endl;
        trick[max]++;
        return max;
    }

    //function for printing result
    void result(int trick[], int maker) {

        if (trick[0] + trick[2] > trick[1] + trick[3]) {
            cout << player[0]->get_name() << " and " << player[2]->get_name() << " win the hand" << endl;
            if (maker == 0 || maker == 2) {
                if (trick[0] + trick[2] == 5) {
                    cout << "Marched!" << endl;
                    score[0] += 2;
                }
                else score[0]++;
            }

            else {
                cout << "euchred!" << endl;
                score[0] += 2;
            }
        }
        
        else { 
            cout << player[1]->get_name() << " and " << player[3]->get_name() << " win the hand" << endl; 

            if (maker == 1 || maker == 3) {
                if (trick[1] + trick[3] == 5) {
                    cout << "march!" << endl;
                    score[1] += 2;
                }
                else score[1]++;
            }

            else {
                cout << "euchred!" << endl;
                score[1] += 2;
            }
        }

        cout << player[0]->get_name() << " and " << player[2]->get_name() << " have " << score[0] << " points" << endl;

        cout << player[1]->get_name() << " and " << player[3]->get_name() << " have " << score[1] << " points" << endl << endl;
    }

    void Delete() {
        for (int i = 0; i < 4; ++i) {
            delete player[i];
        }
    }

    int maxScore() {
        if (score[0] > score[1])return score[0];
        else return score[1];
    }

    void finalResult() {
        if (score[0] > score[1])cout << player[0]->get_name() << " and " << player[2]->get_name() << " win!" << endl;

        else cout << player[1]->get_name() << " and " << player[3]->get_name() << " win!" << endl;
    }

    void newHand() {
        pack.reset();
        trump = "";
    }

private:
    Pack pack;
    Player *player[4];
    string trump;
    int score[2];
};


int main(int argc, char* argv[]) {

    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }

    int hand = 0;
    ifstream ins;
    string filename = argv[1];
    int point = atoi(argv[3]);
    string shuffle = argv[2];
    string name[4] = {argv[4],argv[6],argv[8],argv[10]};
    string type[4] = {argv[5],argv[7],argv[9],argv[11] };

  
    for (int i = 0; i < 4; i ++) {
        
        if (type[i] != "Human" && type[i] != "Simple") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                << "NAME4 TYPE4" << endl;
            
            return 1;
        }
    }

    ins.open(filename);
    
    
    if (point < 1 || point > 100) {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                << "NAME4 TYPE4" << endl;
           
            return 1;
    }

    if (shuffle != "shuffle" && shuffle != "noshuffle") {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        cout << argv[i] << " ";
    }

    cout << endl;
    Game newGame = Game(name, type, ins);

    while (newGame.maxScore() < point) {
        int round = 1;
        int lead = (hand + 1) % 4;
        int trick[4] = { 0,0,0,0 };
        newGame.shuffle(shuffle);
        Card upCard = newGame.dealing(hand);
        int maker = newGame.trump_make(hand, round,upCard);

        if (maker == 5) {
            round++;
            maker = newGame.trump_make(hand, round, upCard);
        }
        
        for (int i = 0; i < 5; i++) {
            lead = newGame.trick(lead, trick);
        }

        newGame.result(trick, maker);
        newGame.newHand();
        hand++;
    }

    newGame.finalResult();
    newGame.Delete();
    return 0;
}