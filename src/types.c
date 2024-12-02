#include "include/types.h"

char suit_char(suit_e suit) {
  switch (suit) {
    case CLUBS:
      return 'C';
    case DIAMONDS:
      return 'D';
    case HEARTS:
      return 'H';
    case SPADES:
      return 'S';
  }
}

const char * card_char(card_e card) {
  switch (card) {
    case ACE:
      return "A";
    case TWO:
      return "2";
    case THREE:
      return "3";
    case FOUR:
      return "4";
    case FIVE:
      return "5";
    case SIX:
      return "6";
    case SEVEN:
      return "7";
    case EIGHT:
      return "8";
    case NINE:
      return "9";
    case TEN:
      return "10";
    case JACK:
      return "J";
    case QUEEN:
      return "Q";
    case KING:
      return "K";
    case JOKER:
      return "J";
  }
}

const char * suit_str(suit_e suit) {
  switch (suit) {
    case CLUBS:
      return "CLUBS";
    case DIAMONDS:
      return "DIAMONDS";
    case HEARTS:
      return "HEARTS";
    case SPADES:
      return "SPADES";
  }
}

const char * card_str(card_e card) {
  switch (card) {
    case ACE:
      return "ACE";
    case TWO:
      return "TWO";
    case THREE:
      return "THREE";
    case FOUR:
      return "FOUR";
    case FIVE:
      return "FIVE";
    case SIX:
      return "SIX";
    case SEVEN:
      return "SEVEN";
    case EIGHT:
      return "EIGHT";
    case NINE:
      return "NINE";
    case TEN:
      return "TEN";
    case JACK:
      return "JACK";
    case QUEEN:
      return "QUEEN";
    case KING:
      return "KING";
    case JOKER:
      return "JOKER";
  }
}