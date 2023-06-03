#include <iostream>
#include <vector>
#include <optional>
#include <random>
#include <map>
#include <string>

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
        std::cout << "1 or 11? ";
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
    std::cout << "Constructing deck...\n";

    for (int suit = 0; suit < (int)Suit::num_suits; ++suit)
        for (int rank = 0; rank < (int)Rank::num_ranks; ++rank)
            cards.push_back(Card{ (Suit)suit, (Rank)rank });

    std::shuffle(cards.begin(), cards.end(), Random::mt);

    for (auto card : cards)
        std::cout << card << ' ';
    std::cout << '\n';
}

std::optional<Card> Deck::draw() {
    if (cards.empty()) {
        std::cout << "deck is empty\n";
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
    std::cout << name << " receives " << card << '\n';
    cards.push_back(card);
    score += get_value(card);
    std::cout << "his score is now " << score << '\n';
}

class Blackjack {
public:
    Blackjack();
    void run();
    void deal_card(const std::string& player);
    bool is_over() const;
    std::optional<Player> get_player(std::string name);

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

void Blackjack::run() {
    while (!is_over()) {
        // game logic
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
    return true;
}

std::optional<Player> Blackjack::get_player(std::string name) {
    auto player = players.find(name);
    if (player != players.end())
        return player->second;
    return {};
}

int main() {
    Blackjack game;
    game.deal_card("dealer");
    auto dealer = game.get_player("dealer");
    if (dealer.has_value()) {
        for (auto card : dealer.value().get_cards())
            std::cout << card << '\n';
    }

    return 0;
}