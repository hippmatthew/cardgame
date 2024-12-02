#include "include/game.h"
#include "include/graphics.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int suitValue(struct game_state * state, suit_e suit) {
  switch (suit) {
    case CLUBS:
      return state->suitValues[0];
    case DIAMONDS:
      return state->suitValues[1];
    case HEARTS:
      return state->suitValues[2];
    case SPADES:
      return state->suitValues[3];
  }
}

packed_data_t packData(struct game_state * state) {
  packed_data_t data = {0};

  for (int i = 0; i < 54; ++i) {
    if (i < 4) data.suitValues[i] = htonl(state->suitValues[i]);

    data.deck.cards[i].suit = htonl(state->deck.cards[i].suit);
    data.deck.cards[i].value = htonl(state->deck.cards[i].value);
  }

  data.deck.count = htonl(state->deck.count);

  return data;
}

void unpackData(struct game_state * state, packed_data_t * data) {
  for (int i = 0; i < 54; ++ i) {
    if (i < 4) state->suitValues[i] = ntohl(data->suitValues[i]);

    state->deck.cards[i].suit = ntohl(data->deck.cards[i].suit);
    state->deck.cards[i].value = ntohl(data->deck.cards[i].value);
  }

  state->deck.count = ntohl(data->deck.count);
}

card_t packCard(card_t * card) {
  card_t packedCard;

  packedCard.suit = htonl(card->suit);
  packedCard.value = htonl(card->value);

  return packedCard;
}

void unpackCard(card_t * card) {
  card->suit = ntohl(card->suit);
  card->value = ntohl(card->value);
}

card_t chooseCard(struct game_state * state, int sel) {
  card_t card = state->hand.cards[sel];

  --state->hand.count;
  for (int i = sel; i < state->hand.count; ++i)
    state->hand.cards[i] = state->hand.cards[i + 1];

  return card;
}

void generateSuitBias(struct game_state * state) {
  for (int i = 0; i < 4; ++i)
    state->suitValues[i] = i - 1;

  // another knuth shuffle (see newDeck())
  for (int i = 3; i > 0; --i) {
    int index = rand() % (i + 1);
    int temp = state->suitValues[index];

    state->suitValues[index] = state->suitValues[i];
    state->suitValues[i] = temp;
  }
}

deck_t newDeck() {
  deck_t deck = {0};
  deck.count = 54;

  deck.cards[52].value = deck.cards[53].value = JOKER;
  deck.cards[52].suit = deck.cards[53].suit = SPADES;

  for (int i = SPADES; i >= CLUBS; --i) {
    for (int j = ACE; j <= KING; ++j) {
      card_t card = {0};
      card.suit = i;
      card.value = j;

      deck.cards[13 * abs(i - 18) + j - 1] = card;
    }
  }

  srand(time(0));

  // Knuth shuffle:
  // 1. start at highest index
  // 2. swap with random index that hasn't been sorted yet
  // 3. repeat until you reach the last element
  for (int i = 53; i > 0; --i) {
    int index = rand() % (i + 1);
    card_t temp = deck.cards[index];

    deck.cards[index] = deck.cards[i];
    deck.cards[i] = temp;
  }

  return deck;
}

card_t selectCard(struct game_state * state) {
  int sel = 0;
  char input = 0;

  while (input != 3) {
    drawHand(sel, state->hand);
    input = getchar();

    switch(input) {
      case 'a':
        sel = sel == 0 ? 0 : sel - 1;
        break;
      case 'd':
        sel = sel == state->hand.count - 1 ? state->hand.count - 1 : sel + 1;
        break;
      case 'e':
        input = 3;
    }
  }

  return chooseCard(state, sel);
}

int calculateRound(struct game_state * state, card_t hostCard, card_t clientCard) {
  int hostVal = hostCard.value + suitValue(state, hostCard.suit);
  int clientVal = clientCard.value + suitValue(state, clientCard.suit);

  int result = hostVal > clientVal ? 1 : 2;

  if (hostCard.value == JOKER && clientCard.value != JOKER) result = 3;
  else if (clientCard.value == JOKER && hostCard.value != JOKER) result = 4;
  else if (hostCard.value == JOKER && clientCard.value == JOKER) result = 5;

  return result;
}

void win(struct game_state * state) {
  --state->deck.count;
  state->oppCardCount = state->oppCardCount == 4 ? 4 : state->oppCardCount + 1;

  drawOppHand(state->oppCardCount);

  (void)printf("\033[18;40HYou win the round!");
  (void)fflush(stdout);
}

void lose(struct game_state * state) {
  if (state->hand.count == 4) return;

  state->hand.cards[state->hand.count++] = state->deck.cards[--state->deck.count];

  drawHand(4, state->hand);

  (void)printf("\033[18;40HYou lost the round...");
  (void)fflush(stdout);
}

void dealJoker(struct game_state * state) {
  if (state->deck.count - 4 + state->oppCardCount < 0) {
    (void)printf("\033[18;40HLow deck count nullified the joker!");
    (void)fflush(stdout);
    return;
  }

  state->deck.count -= 4 - state->oppCardCount;
  state->oppCardCount = 4;

  drawOppHand(state->oppCardCount);

  (void)printf("\033[18;40HYou jokered the opponent!");
  (void)fflush(stdout);
}

void getJoker(struct game_state * state) {
  if (state->deck.count - 4 + state->hand.count < 0) {
    (void)printf("\033[18;40HLow deck count nullified the joker!");
    (void)fflush(stdout);
    return;
  }

  for (int i = state->hand.count; i < 4; ++i)
    state->hand.cards[i] = state->deck.cards[--state->deck.count];
  state->hand.count = 4;

  drawHand(4, state->hand);

  (void)printf("\033[18;40HYou got jokered...");
  (void)fflush(stdout);
}

void newHand(struct game_state * state, player_e player) {
  for (int i = 0; i < 4; ++i)
    state->hand.cards[i] = state->deck.cards[state->deck.count - 2 * i - 1 - player];
  state->hand.count = 4;
}

void doubleJoker(struct game_state * state, player_e player) {
  if (state->deck.count - 8 < 0) {
    (void)printf("\033[18;40HLow deck count nullified the double joker!");
    (void)fflush(stdout);
    return;
  }

  newHand(state, player);
  state->deck.count -= 8;
  state->oppCardCount = 4;

  drawHand(4, state->hand);
  drawOppHand(state->oppCardCount);
}

int endRound(struct game_state * state) {
  if (state->hand.count == 0) {
    (void)printf("\033[19;40HYou win!");
    (void)fflush(stdout);
    return 1;
  }

  if (state->oppCardCount == 0) {
    (void)printf("\033[19;40HYou lose...");
    (void)fflush(stdout);
    return 1;
  }

  ++state->round;
  return 0;
}

void playHost() {
  clearScreen();

  (void)printf("creating socket...\n");

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("failed to create socket");
    return;
  }

  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8008);

  (void)printf("binding client address info...\n");

  if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
    perror("failed to bind info");
    return;
  }

  (void)printf("listening on port 8008...\n");

  if (listen(fd, 0) == -1) {
    perror("failed to listen on port 8008");
    return;
  }

  (void)printf("waiting for client...\n");

  struct sockaddr_in clientAddress = {0};
  socklen_t addressSize = sizeof(clientAddress);

  int cfd = accept(fd, (struct sockaddr *)&clientAddress, &addressSize);
  if (cfd == -1) {
    perror("failed to connect to client");
    close(fd);
    return;
  }

  (void)printf("beginning game...\n");
  sleep(1);

  struct game_state state = {0};
  state.oppCardCount = 4;
  state.round = 1;

  state.deck = newDeck();
  generateSuitBias(&state);

  char buf[sizeof(packed_data_t)] = {0};
  packed_data_t * data = (packed_data_t *)buf;
  *data = packData(&state);

  (void)write(cfd, data, sizeof(packed_data_t));

  newHand(&state, HOST);
  state.deck.count -= 8;

  while (state.deck.count != 0) {
    clearScreen();
    drawBoard(&state);

    card_t hostCard = selectCard(&state);

    char buf1[sizeof(card_t)] = {0};
    card_t * clientCard = (card_t *)buf1;

    (void)read(cfd, clientCard, sizeof(card_t));
    unpackCard(clientCard);

    --state.oppCardCount;
    drawBoard(&state);
    drawHand(4, state.hand);
    drawChosen(hostCard, *clientCard);

    int result = calculateRound(&state, hostCard, *clientCard);

    char buf2[sizeof(results_t)] = {0};
    results_t * results = (results_t *)buf2;
    results->card = packCard(&hostCard);
    results->result = htonl(result);

    (void)write(cfd, results, sizeof(results_t));

    switch (result) {
      case 0:
        (void)printf("\033[18;40HStalemate round.");
        break;
      case 1:
        win(&state);
        break;
      case 2:
        lose(&state);
        break;
      case 3:
        dealJoker(&state);
        break;
      case 4:
        getJoker(&state);
        break;
      case 5:
        doubleJoker(&state, HOST);
    }

    if (endRound(&state)) break;

    sleep(3);
  }

  close(cfd);
  close(fd);
}

void playClient(const char * ip) {
  clearScreen();

  (void)printf("creating socket...\n");

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("failed to create socket");
    return;
  }

  (void)printf("connecting to host...\n");

  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip);
  address.sin_port = htons(8008);

  if (connect(fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
    perror("failed to connect to host");
    close(fd);
    return;
  }

  (void)printf("beginning game...\n");
  sleep(1);

  char buf[sizeof(packed_data_t)] = {0};
  packed_data_t * data = (packed_data_t *)buf;
  (void)read(fd, data, sizeof(packed_data_t));

  struct game_state state = {0};
  state.oppCardCount = 4;
  state.round = 1;

  unpackData(&state, data);

  newHand(&state, CLIENT);
  state.deck.count -= 8;

  while (state.deck.count != 0) {
    clearScreen();
    drawBoard(&state);

    card_t clientCard = selectCard(&state);

    char buf1[sizeof(card_t)] = {0};
    card_t * sentCard = (card_t *)buf1;
    *sentCard = packCard(&clientCard);

    (void)write(fd, sentCard, sizeof(card_t));

    char buf2[sizeof(results_t)] = {0};
    results_t * results = (results_t *)buf2;
    (void)read(fd, results, sizeof(results_t));

    card_t hostCard = results->card;
    unpackCard(&hostCard);
    int result = ntohl(results->result);

    --state.oppCardCount;

    drawBoard(&state);
    drawHand(4, state.hand);
    drawChosen(clientCard, hostCard);

    switch (result) {
      case 0:
        (void)printf("\033[18;40HStalemate round.");
        break;
      case 1:
        lose(&state);
        break;
      case 2:
        win(&state);
        break;
      case 3:
        getJoker(&state);
        break;
      case 4:
        dealJoker(&state);
        break;
      case 5:
        doubleJoker(&state, CLIENT);
    }

    if (endRound(&state)) break;

    sleep(3);
  }

  close(fd);
}