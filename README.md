Use W and S to navigate the main menu. E is to select
If hosting, make sure port 8008 is forwarded to your device's local ip in your router if you want WAN support
If you choose 'Join', input the ip (internal is LAN, external if WAN) of the host you want to connect to

Upon connecting, a deck will be generated and shuffled and a suit bias will also be generated.
A suit bias is just an array of values corresponding to the suits.

At the top of the board the round number, deck count, and suit bias is displayed. Spades = S, Hearts = H, Diamonds = D, and Clubs = C.
Your opponent's hand is displayed under that so you will always know how many cards they have.
The center of the screen is where the round is played. This is empty until both cards are chosen.
Your hand is displayed at the bottom. The card you currently have selected is displayed higher than the rest.
To navigate, use A and D. To select use E.
Once selected, try not to type anything (especially not E) until the next round since I didnt clear the buffers and I dont want to go back and change it lol

RULES:
1. Select a card from your hand
2. The card's value is calculated as such: VALUE = FACE VALUE + SUIT VALUE (suit value is obtained from the suit bias described earlier)
3. You win the round if your card's value is higher than your oppponent's
5. Losing a round means you draw a card
6. If both card values are equal, it is a stalemate and nobody draws.
7. If a joker is played, the opposing player draws a maximum hand
8. If both jokers are in play, each player discards their remaining hand and redraws a maximum hand
9. If the deck count is too low, jokers will not take effect.
10. FACE VALUES are as such: ACES = 1, FACE VALUE = FACE VALUE, JACKS = 11, QUEENS = 12, KINGS = 13
