#include <iostream>
#include <random>
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
	/*std::vector<float> neuron_activations = {float(dealerShowing)};
	for (int i = 0; i < playerHand.size(); i++) { neuron_activations.push_back(playerHand.at(i)); }
	while (neuron_activations.size() < 9) { neuron_activations.push_back(0); }
	network->set_inputs(neuron_activations);*/

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

	//just a proof of concept function, rip it out when making you're own
	float cost = 0;
	for (int i = 0; i < 1000; i++) {
		cost -= blackjack_game(network);
	}
	return cost;
}

int main() {
	NeuralNetwork* base = new NeuralNetwork({ 3, 7, 9, 7, 2 });

	NeuralNetwork* trained = base->random_evolve_train(score_func, 100, 200, 3);

	trained->save_to_file("test.dat");
	trained->load_from_file("test.dat");

	std::cout << trained->get_structure() << "\n" << trained->get_cost();
	return 0;
}