#include"Player.h"
#include<algorithm>
#include<iostream>
#include<array>
#include<vector>
using namespace std;

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << p.get_name();
    return os;
}


class SimplePlayer : public Player {
public:
    //default constructor
    SimplePlayer():simple_size(0){}

    //non default with only name
    SimplePlayer(string name_in) :simple_name(name_in),simple_size(0) {}


    //non default
    SimplePlayer(const string& simple_name_in, const int simple_size_in, const array<Card, MAX_HAND_SIZE> simple_cards_in) :simple_name(simple_name_in), simple_size(simple_size_in) {
        for (int i = 0; i < simple_size; i++) {
            simple_cards[i] = Card(simple_cards_in[i].get_rank(), simple_cards_in[i].get_suit());
        }
    }

  

    const string& get_name()const override {
        return simple_name;
    }

    void add_card(const Card& c) override {
        simple_cards.push_back(c);
        simple_size++;
    }

    bool make_trump(const Card& upcard, bool is_dealer,
        int round, std::string& order_up_suit)const override {
        
        int faceCard = 0;

        //When this is the first round
        if (round == 1) {
            //Calculate face card
            for (int i = 0; i < simple_size; i++) {
                if (simple_cards[i].is_trump(upcard.get_suit()) && simple_cards[i].is_face())faceCard++;
            }
            
            if (faceCard >= 2) {
                order_up_suit = upcard.get_suit();
                return true;
        }
        }

        //when this is the second round
        if (round == 2) {
            //when the player is dealer
            if (is_dealer) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }

            //Calculate the face card
            for (int i = 0; i < simple_size; i++) {
                if (simple_cards[i].is_trump(Suit_next(upcard.get_suit())) && simple_cards[i].is_face())faceCard++;
            }
          
            if (faceCard >= 1) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
        }

        return false;
    }

    void add_and_discard(const Card& upcard) override {
        //Find the lowest card in hand
        Card lowest = Card(upcard.get_rank(), upcard.get_suit());
        int lowIndex = simple_size;
        for (int i = 0; i < simple_size; i++) {
            if (Card_less(simple_cards[i], lowest,upcard.get_suit())) {
                lowest = Card(simple_cards[i].get_rank(), simple_cards[i].get_suit());
                lowIndex = i;
            }
        }
       
        //if lowest is not upcard
        if (lowIndex != simple_size) {
            for (int i = lowIndex; i < simple_size - 1; i++) {
                simple_cards[i] = simple_cards[i + 1];
            }
            simple_cards[simple_size - 1] = Card(upcard.get_rank(), upcard.get_suit());
        }
        
    }

    void Sort_trump(string trump) {
        for (int i = 1; i < simple_size; i++) {
            for (int j = 0; j < simple_size - i; j++) {
                if (Card_less(simple_cards[j + 1], simple_cards[j], trump)) {
                    Card temp = Card(simple_cards[j].get_rank(), simple_cards[j].get_suit());
                    simple_cards[j] = Card(simple_cards[j + 1].get_rank(), simple_cards[j + 1].get_suit());
                    simple_cards[j + 1] = Card(temp.get_rank(), temp.get_suit());
                }
            }
        }
    }

    void Sort_lead(string trump, const Card& Led_card) {
        
        for (int i = 1; i < simple_size; i++) {
            for (int j = 0; j < simple_size - i; j++) {
                if (Card_less(simple_cards.at(j+1), simple_cards.at(j),Led_card,trump)) {
                    Card temp = Card(simple_cards[j].get_rank(), simple_cards[j].get_suit());
                    simple_cards[j] = Card(simple_cards[j + 1].get_rank(), simple_cards[j + 1].get_suit());
                    simple_cards[j + 1] = Card(temp.get_rank(), temp.get_suit());
                }
            }
        }
    }

    Card lead_card(const std::string& trump)override {
        // sort
        Sort_trump(trump);
        int index = 5;
        for (int i = 0; i < simple_size; i++) {
            if (!simple_cards[i].is_trump(trump))index = i;
        }
        
        if (index != 5) {
            Card lead = Card(simple_cards[index].get_rank(), simple_cards[index].get_suit());

            for (int i = index; i < simple_size - 1; i++) {
                simple_cards[i] = simple_cards[i + 1];
            }

            simple_cards.pop_back();
            simple_size--;
            return lead;
        }

        Card lead = Card(simple_cards[simple_size - 1].get_rank(), simple_cards[simple_size - 1].get_suit());

        simple_cards.pop_back();
        simple_size--;
        return lead;
    }

    Card play_card(const Card& led_card, const std::string& trump)override {

        //sort card      
        Sort_lead(trump,led_card);
        string suit = led_card.get_suit(trump);
       

        int index = 0;

        
        for (int i = 0; i < simple_size; i++) {
            if (simple_cards[i].get_suit(trump) == suit) {
                index = i;
            }
        }
        
        
        //if index = 0, this is the smallest one with no lead suit. If index != 0, it will be the largest with lead suit.
        
        Card play = Card(simple_cards[index].get_rank(), simple_cards[index].get_suit());

        for (int i = index; i < simple_size - 1; i++) {
            simple_cards[i] = simple_cards[i + 1];
        }

        simple_cards.pop_back();
        simple_size--;
        return play;
     
    }

 
private:
    string simple_name;
    int simple_size;
    vector<Card> simple_cards;
};


class HumanPlayer : public Player {
public:
    //default constructor
    HumanPlayer() :human_size(0) {}

    //non default with only name
    HumanPlayer(string name_in) :human_name(name_in), human_size(0) {}


    //non default
   HumanPlayer(const string& human_name_in, const int human_size_in, const vector<Card> human_cards_in) :human_name(human_name_in), human_size(human_size_in) {
        for (int i = 0; i < human_size; i++) {
            human_cards[i] = Card(human_cards_in[i].get_rank(), human_cards_in[i].get_suit());
        }
    }

    const std::string& get_name() const override {
        return human_name;
    }

    void add_card(const Card& c)override {
        human_cards.push_back(c);
        human_size++;
        sort(human_cards.begin(),human_cards.end());
    }

    void printCard() const{
        
        for (int i = 0; i < human_size; i++) {
            cout << "Human player " << human_name << "'s hand: " << "[" << i << "] " << human_cards[i] << endl;
        }
    }

    bool make_trump(const Card& upcard, bool is_dealer,
        int round, std::string& order_up_suit) const override {
        printCard();

        cout << "Human player " << human_name << ", please enter a suit, or \"pass\":";
        
        string command;
        cin >> command;
        cout << endl;
        if (command == "pass") 
        return false; 

        else {
            order_up_suit = command;

            return true;
        }
    }

    void add_and_discard(const Card& upcard)override {
        printCard();
        cout << "Discard upcard: [-1]" << endl;
        cout << "Human player " << human_name << ", please select a card to discard:";

        int index;

        cin >> index;
        cout << endl;
        if (index != -1) {
            for (int i = index; i < human_size - 1; i++) {
                human_cards[i] = human_cards[i + 1];
            }
            human_cards[human_size - 1] = Card(upcard.get_rank(), upcard.get_suit());
        }
        sort(human_cards.begin(),human_cards.end());
    }

    Card lead_card(const std::string& trump)override {
        printCard();
        cout << "Human player " << human_name << ", please select a card:";
        int index;
        cin >> index;
        cout << endl;
        Card lead = Card(human_cards[index].get_rank(), human_cards[index].get_suit());

        for (int i = index; i < human_size - 1; i++) {
            human_cards[i] = human_cards[i + 1];
        }
        human_cards.pop_back();
        human_size--;
        return lead;
}
    Card play_card(const Card& led_card, const std::string& trump)override {
        printCard();
        cout << "Human player " << human_name << ", please select a card:";
        int index;
        cin >> index;
        cout << endl;
        Card play = Card(human_cards[index].get_rank(), human_cards[index].get_suit());

        for (int i = index; i < human_size - 1; i++) {
            human_cards[i] = human_cards[i + 1];
        }
        human_cards.pop_back();
        human_size--;
        return play;
    }
    
private:
    string human_name;
    int human_size;
    vector <Card> human_cards;
};


Player* Player_factory(const std::string& name, const std::string& strategy) {
    if (strategy == "Simple") {
        // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }

    else {
        // The "new" keyword dynamically allocates an object.
        return new HumanPlayer(name);
    }

}

