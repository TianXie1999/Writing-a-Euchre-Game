#include<iostream>
#include"Pack.h"
#include<string>

Pack::Pack()
{
    int cards_per_suit = 6;
    int start_of_rank = 7;
    
    
    for(int i = 0; i < 4; i++)
        for (int j = start_of_rank; j < cards_per_suit + start_of_rank; j++) {
         cards[cards_per_suit * i + j-start_of_rank] = Card(RANK_NAMES_BY_WEIGHT[j],SUIT_NAMES_BY_WEIGHT[i]);
         }
    next = 0;
}

Pack::Pack(std::istream& pack_input)
{
    std::string Rank = "";
    std::string of = "";
    std::string Suit = "";
    for (int i = 0; i < PACK_SIZE; i++) {
        pack_input >> Rank >> of >> Suit;
        cards[i] = Card(Rank, Suit);
    }
    next = 0;
}

Card Pack::deal_one()
{   
    Card Next = cards[next];
    next++;
    return Next;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle()
{
    std::array<Card, PACK_SIZE> cardsCopy;
    int half = PACK_SIZE / 2;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < half; j++) {
            cardsCopy[2 * j + 1] = cards[j];
        }

        for (int k = half; k < PACK_SIZE; k++) {
            int index = (k - half) * 2;
            cardsCopy[index] = cards[k];
        }
        for (int m = 0; m < PACK_SIZE; m++) {
            cards[m] = cardsCopy[m];
        }
    }

    
    reset();
}

bool Pack::empty() const
{
    if (next == 24)return true;
    return false;
}

