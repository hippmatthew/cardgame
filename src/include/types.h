#ifndef gamble_types_h
#define gamble_types_h

typedef enum {
  ACE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  JOKER,
} card_e;

typedef enum {
  CLUBS = 15,
  DIAMONDS,
  HEARTS,
  SPADES
} suit_e;

typedef struct {
  card_e value;
  suit_e suit;
} card_t;

typedef struct {
  card_t cards[4];
  int count;
} hand_t;

typedef struct {
  card_t cards[54];
  int count;
} deck_t;

struct game_state {
  deck_t deck;
  int suitValues[4];
  int round;
  hand_t hand;
  int oppCardCount;
};

typedef struct {
  deck_t deck;
  int suitValues[4];
} packed_data_t;

typedef struct {
  card_t card;
  int result;
} results_t;

typedef enum {
  CLIENT,
  HOST
} player_e;

const char * card_char(card_e);
const char * card_str(card_e);
char suit_char(suit_e);
const char * suit_str(suit_e);

#endif