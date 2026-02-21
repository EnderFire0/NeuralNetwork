#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include "NeuralNetwork.h"

std::random_device dev;
std::mt19937 randEngine(dev());

struct Deck {
private:
	std::vector<int> draw;
	std::vector<int> discard;

public:
	Deck() {
		this->draw = {};
		this->draw = discard;
		for (int i = 0; i < 4; i++) {
			for (int o = 1; o <= 13; o++) {
				this->draw.push_back(o);
			}
		}
	}

	int draw_card() {
		int card = this->draw.front();
		this->discard.push_back(card);
		this->draw.erase(this->draw.begin());
		return card;
	}

	void shuffle() {
		for (int i : this->discard) {
			this->draw.push_back(i);
		}
		this->discard.clear();
		std::shuffle(this->draw.begin(), this->draw.end(), dev);
	}
};
Deck* deck = new Deck();

int count_aces(std::vector<int> hand) {
	int aces = 0;
	for (int i : hand) {
		if (i == 1) { aces++; }
	}
	return aces;
}

float score_blackjack_hand(std::vector<int> hand) {
	int aces = 0;
	float score = 0;
	for (int i : hand) {
		switch (i) {
		case 1:
			aces++;
			score += 1;
			break;
		case 11:
			score += 10;
			break;
		case 12:
			score += 10;
			break;
		case 13:
			score += 10;
			break;
		default:
			score += i;
		}
	}
	if ((aces > 0) && (score < 12)) {
		score += 10;
	}

	return score;
}

bool hit_or_stand(float playerScore, int aces, int dealerShowing, NeuralNetwork* network) {
	/*std::vector<float> neuron_values = {float(dealerShowing)};
	for (int i = 0; i < playerHand.size(); i++) { neuron_values.push_back(playerHand.at(i)); }
	while (neuron_values.size() < 9) { neuron_values.push_back(0); }
	network->set_inputs(neuron_values);*/

	network->set_inputs(std::vector<float> {playerScore, float(aces), float(dealerShowing)});
	network->update_network();
	std::vector<float> output = network->read_output();

	return output[0] > output[1];
}

//proof of concept scoring function
float blackjack_game(NeuralNetwork* network) {
	deck->shuffle();

	std::vector<int> playerHand, dealerHand = {};
	
	for (int i = 0; i < 2; i++) { playerHand.push_back(deck->draw_card()); }
	for (int i = 0; i < 2; i++) { dealerHand.push_back(deck->draw_card()); }

	float playerScore = score_blackjack_hand(playerHand);

	if (playerScore == 21) {
		if (score_blackjack_hand(dealerHand) == 21) { return 0; }
		return 1;
	}
	if (score_blackjack_hand(dealerHand) == 21) { return -1; }

	while (hit_or_stand(playerScore, count_aces(playerHand), dealerHand[0], network)) {
		playerHand.push_back(deck->draw_card());
		playerScore = score_blackjack_hand(playerHand);
		if (playerScore > 21) {
			return -1;
		}
	}

	while (score_blackjack_hand(dealerHand) < 17) { dealerHand.push_back(deck->draw_card()); }

	if (score_blackjack_hand(dealerHand) > 21) { return 1; }

	if (score_blackjack_hand(dealerHand) > playerScore) { return -1; }
	else if (score_blackjack_hand(dealerHand) < playerScore) { return 1; }
	else { return 0; }

}


float score_func(NeuralNetwork* network) {
	//scoring function goes here

	//just a proof of concept function, feel free to rip it out when making you're own
	return blackjack_game(network);
}

int main() {
	NeuralNetwork* base = new NeuralNetwork({ 3, 7, 9, 7, 2 });
	NeuralNetwork* best = nullptr;

	for (int generation = 0; generation < 100; generation++) {
		std::cout << "Generation: " << generation << "\n";

		for (int member = 0; member < 200; member++) {
			NeuralNetwork* test_member = base->duplicate();
			test_member->random_tune_network(5/std::sqrt(generation + 1));
			for (int trial = 0; trial < 10000; trial++) {
				test_member->modify_score(score_func(test_member));
			}
			if (best == nullptr) {
				best = test_member;
				continue;
			}
			if (best->get_score() < test_member->get_score()) {
				delete(best);
				best = test_member;
			}
			else {
				delete(test_member);
			}
		}
		if (best != nullptr) {
			std::cout << "Best score: " << best->get_score() << "\n";
			delete(base);
			base = best;
			base->set_score(0);
			best = nullptr;
		}
	}

	base->print_structure();

	return 0;
}