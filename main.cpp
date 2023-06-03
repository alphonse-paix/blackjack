#include <iostream>
#include <vector>
#include <optional>
#include <random>
#include <map>
#include <string>
#include <iomanip>

#define VERBOSE

namespace Random {
    std::random_device rd;
    std::seed_seq ss{ rd(), rd(), rd() };
    std::mt19937 mt{ ss };
}

enum class Suit {
    clover,
    diamond,
    heart,
    spade,
    num_suits,
};

enum class Rank {
    ace,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    num_ranks,
};

struct Card {
    Suit suit;
    Rank rank;
};

int get_value(Card& card) {
    using enum Rank;
    switch (card.rank) {
    case two: case three: case four: case five:
    case six: case seven: case eight: case nine:
        return (int)card.rank + 1;
    case ten: case jack: case queen: case king:
        return 10;
    case ace: {
        std::cout << "Ace! 1 or 11? ";
        int i;
        std::cin >> i;
        return i;
    }
    default:
        std::cout << "get_score(card): ???";
        return -1;
    }
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    using enum Rank;
    using enum Suit;

    switch (card.rank) {
    case two:
    case three:
    case four:
    case five:
    case six:
    case seven:
    case eight:
    case nine:
        std::cout << (int)card.rank + 1;
        break;
    case ten:
        std::cout << 'T';
        break;
    case ace:
        std::cout << 'A';
        break;
    case jack:
        std::cout << 'J';
        break;
    case queen:
        std::cout << 'Q';
        break;
    case king:
        std::cout << 'K';
        break;
    default:
        std::cout << "????";
        break;
    }

    switch (card.suit) {
    case clover:
        std::cout << 'C';
        break;
    case diamond:
        std::cout << 'D';
        break;
    case heart:
        std::cout << 'H';
        break;
    case spade:
        std::cout << 'S';
        break;
    default:
        std::cout << "????";
        break;
    }

    return os;
}

class Deck {
public:
    Deck();
    std::optional<Card> draw();
    std::size_t size() const { return cards.size(); }

private:
    std::vector<Card> cards;
};

Deck::Deck() {
    for (int suit = 0; suit < (int)Suit::num_suits; ++suit)
        for (int rank = 0; rank < (int)Rank::num_ranks; ++rank)
            cards.push_back(Card{ (Suit)suit, (Rank)rank });

    std::shuffle(cards.begin(), cards.end(), Random::mt);

#ifdef VERBOSE
    std::cout << "Deck: ";
    for (auto card : cards)
        std::cout << card << ' ';
    std::cout << ".\n";
#endif
}

std::optional<Card> Deck::draw() {
    if (cards.empty()) {
        std::cout << "The deck is empty.\n";
        return {};
    }
    auto last = cards.back();
    cards.pop_back();
    return last;
}

class Player {
public:
    Player(std::string name);
    int get_score() const { return score; }
    void receives(Card& card);
    std::string get_name() const { return name; }
    const std::vector<Card>& get_cards() const { return cards; }

private:
    std::string name;
    std::vector<Card> cards;
    int score;
};

Player::Player(std::string name) : name{ name }, score{ 0 } {}

void Player::receives(Card& card) {
#ifdef VERBOSE
    std::cout << name << " got " << card << ".\n";
#endif
    cards.push_back(card);
    score += get_value(card);
#ifdef VERBOSE
    std::cout << "His score is now " << score << ".\n";
#endif
}

class Blackjack {
public:
    Blackjack();
    void run();
    void deal_card(const std::string& player);
    bool is_over() const;
    std::optional<Player> get_player(std::string name);
    int player_score() const;
    int dealer_score() const;
    void print_score() const;

private:
    Deck deck;
    std::map<std::string, Player> players;
};

Blackjack::Blackjack() {
    Player dealer{ "dealer" };
    players.insert(std::make_pair(dealer.get_name(), dealer));
    Player player{ "player" };
    players.insert(std::make_pair(player.get_name(), player));
}

void ignore_line() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Blackjack::run() {
    bool player_stand = false;

    deal_card("dealer");
    deal_card("player");
    deal_card("player");

    if (player_score() == 21) {
        std::cout << "Blackjack!\n";
        return;
    }

    while (!is_over()) {
        print_score();

        if (!player_stand) {
            std::cout << "Draw (y/n): ";
            char answer;
            std::cin >> answer;
            ignore_line();
            if (answer == 'y') {
                deal_card("player");
                if (player_score() > 21) {
                    std::cout << "Player busted!\n";
                    break;
                }
            }
            else {
                player_stand = true;
            }
        }
        if (dealer_score() < player_score() || dealer_score() < 17) {
            deal_card("dealer");
            if (dealer_score() > 21) {
                std::cout << "Dealer busted!\n";
                break;
            }
        }
        else {
            if (player_stand)
                break;
        }
    }
}

void Blackjack::deal_card(const std::string& name) {
    auto card = deck.draw();
    if (card.has_value()) {
        auto player = players.find(name);
        if (player != players.end())
            player->second.receives(card.value());
        else
            std::cout << name << " is not a player\n";
    }
}

bool Blackjack::is_over() const {
    return false;
}

std::optional<Player> Blackjack::get_player(std::string name) {
    auto player = players.find(name);
    if (player != players.end())
        return player->second;
    return {};
}

int Blackjack::player_score() const {
    return players.find("player")->second.get_score();
}

int Blackjack::dealer_score() const {
    return players.find("dealer")->second.get_score();
}

void Blackjack::print_score() const {
    std::cout << "Dealer " << std::setw(2) << dealer_score()
        << ' ' << std::setw(2) << player_score() << " Player\n";
}

int main() {
    Blackjack game;
    //game.deal_card("dealer");
    //auto dealer = game.get_player("dealer");
    //if (dealer.has_value()) {
    //    for (auto card : dealer.value().get_cards())
    //        std::cout << card << '\n';
    //}
    game.run();

    return 0;
}