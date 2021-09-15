// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
using namespace std;
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;



int getRankWeight(string rank) {
    int Rank = 0;
    for (int i = 0; i < NUM_RANKS; i++) {
        string temp = RANK_NAMES_BY_WEIGHT[i];
        if (rank == temp) {
            Rank = i;
            break;
        }
    }
    return Rank;
}

int getSuitWeight(string suit1) {
    int Suit = 0;
    for (int i = 0; i < NUM_SUITS; i++) {
        if (suit1 == SUIT_NAMES_BY_WEIGHT[i]) {
            Suit = i;
            break;
        }
    }
    return Suit;
}

bool trumpLess(const Card& a, const Card& b, const std::string& trump) {
    //if a is right bower
    if (a.is_right_bower(trump))return false;
    //if b is right bower
    if (b.is_right_bower(trump))return true;
    //if a is left bower
    if (a.is_left_bower(trump))return false;
    //if b is left bower
    if (b.is_left_bower(trump))return true;
    //if not, compare normally
    return operator<(a, b);
}

Card::Card():rank(RANK_TWO),suit(SUIT_SPADES){}


Card::Card(const std::string& rank_in, const std::string& suit_in):rank(rank_in),suit(suit_in){}

std::string Card::get_rank() const
{
    return rank;
}

std::string Card::get_suit() const
{
    return suit;
}

std::string Card::get_suit(const std::string& trump) const
{
    if (is_left_bower(trump))return trump;
    else return suit;
}

bool Card::is_face() const
{
    if (rank == RANK_JACK) return true;
    if (rank == RANK_QUEEN) return true;
    if (rank == RANK_KING) return true;
    if (rank == RANK_ACE) return true;
    return false;

}

bool Card::is_right_bower(const std::string& trump) const
{
    if (suit == trump && rank == RANK_JACK)return true;
    return false;
}

bool Card::is_left_bower(const std::string& trump) const
{
    if (suit == Suit_next(trump) && rank == RANK_JACK) {
        return true;
    }

    return false;
}

bool Card::is_trump(const std::string& trump) const
{
    if (suit == trump)return true;
    if (is_left_bower(trump))return true;
    return false;
}

bool operator<(const Card& lhs, const Card& rhs)
{
    int rank1 = getRankWeight(lhs.get_rank());
    
    int rank2 = getRankWeight(rhs.get_rank());
    
    int suit1 = getSuitWeight(lhs.get_suit());
    int suit2 = getSuitWeight(rhs.get_suit());
    if (rank1 < rank2)return true;
    else if (rank1 == rank2 && suit1 < suit2)return true;
    return false;
}

bool operator>(const Card& lhs, const Card& rhs)
{
    return operator<(rhs, lhs);
}

bool operator==(const Card& lhs, const Card& rhs)
{
    if (lhs.get_rank() == rhs.get_rank()) {
        if (lhs.get_suit() == rhs.get_suit())return true;
    }
    return false;
}

bool operator!=(const Card& lhs, const Card& rhs)
{
    return !(lhs==rhs);
}

std::string Suit_next(const std::string& suit)
{
    if (suit == Card::SUIT_SPADES)return Card::SUIT_CLUBS;
    if (suit == Card::SUIT_HEARTS)return Card::SUIT_DIAMONDS;
    if (suit == Card::SUIT_CLUBS)return Card::SUIT_SPADES;
    return Card::SUIT_HEARTS;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card& a, const Card& b, const std::string& trump)
{   //If a is trump but b is not
    if (a.is_trump(trump) && !b.is_trump(trump)) {
        return false;
    }
    //if a is not trump but b is trump
    if (!a.is_trump(trump) && b.is_trump(trump))  { return true; }
    //if a and b are both trumps
    if (a.is_trump(trump) && b.is_trump(trump)) {
        return trumpLess(a, b, trump);
    }
    
    //neither is trump
    return operator<(a, b);

  
}

bool Card_less(const Card& a, const Card& b, const Card& led_card, const std::string& trump)
{   
    if (led_card.is_left_bower(trump))return Card_less(a, b, trump);

    //both are trumps
    if (a.is_trump(trump) && b.is_trump(trump)) {
        return trumpLess(a, b, trump);
    }
    // a is trump
    if (a.is_trump(trump) && !b.is_trump(trump)) {
        return false;
    }
    // b is trump
    if (!a.is_trump(trump) && b.is_trump(trump)) { return true; }

    //a and b are both led cards
    if (a.get_suit() == led_card.get_suit() && b.get_suit() == led_card.get_suit()) {
        return (a<b);
    }

    //only b is led card
    if (a.get_suit() != led_card.get_suit() && b.get_suit() == led_card.get_suit()) {
        return true;
    }

    //only a is led card
    if (a.get_suit() == led_card.get_suit() && b.get_suit() != led_card.get_suit()) {
        return false;
    }

    //neither is led card
    return (a < b);

}



