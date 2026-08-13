#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include "NeuralNetwork.h"

std::random_device dev;
std::mt19937 randEngine(dev());
std::uniform_real_distribution<> dis2(-1.0, 1.0);

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

//farkel scoring function pulled from other project of mine
float farkel_score(std::vector<char> rolls = {}) {
	unsigned int dice = rolls.size();
	float score = 0;
	int bonusRolls = 0;

	std::sort(rolls.begin(), rolls.end());

	switch (dice) {
	case 6:
		//6oak
		if (rolls.front() == rolls.back()) {
			score += 3000;
			bonusRolls++;
		}
		//2 Triples
		else if ((rolls.front() == rolls.at(2)) && (rolls.at(3) == rolls.back())) {
			score += 2500;
			bonusRolls++;
		}
		//5oak, dice 1-5
		else if ((rolls.front() == rolls.at(4))) {
			score += 2000;
			if (rolls.back() == 5) {
				score += 50;
				bonusRolls++;
			}
			break;
		}
		//5oak, dice 2-6
		else if ((rolls.at(1) == rolls.back())) {
			score += 2000;
			switch (rolls.front()) {
			case 1:
				score += 100;
				bonusRolls++;
			case 5:
				score += 50;
				bonusRolls++;
			default:
				break;
			}
			break;
		}
		//3 pair
		else if ((rolls.front() == rolls.at(1)) && (rolls.at(2) == rolls.at(3)) && (rolls.at(4) == rolls.back())) {
			score += 1500;
			bonusRolls++;
		}
		//straight
		else if ((rolls.front() == 1) && (rolls.at(1) == 2) && (rolls.at(2) == 3) && (rolls.at(3) == 4) && (rolls.at(4) == 5) && (rolls.back() == 6)) {
			score += 1500;
			bonusRolls++;
		}
		//4oak, dice 1-4
		else if (rolls.front() == rolls.at(3)) {
			score += 1000;
			//only need to check for a 5 because a 1 can't be after the fourth die w/o other scoring
			//also cant award bonus roll w/o other scoring
			if (rolls.at(4) == 5) {
				score += 50;
			}
			if (rolls.back() == 5) {
				score += 50;
			}
		}
		//4oak, dice 2-5
		else if (rolls.at(1) == rolls.at(4)) {
			score += 1000;
			//only check for 1 as it can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				score += 100;
				if (rolls.back() == 5) {
					score += 50;
					bonusRolls++;
				}
			}
			else if (rolls.back() == 5) {
				score += 50;
			}
		}
		//4oak, dice 3-6
		else if (rolls.at(2) == rolls.back()) {
			score += 1000;
			switch (rolls.front()) {
			case 1:
				score += 100;
				//second die can't be 1 w/o other scoring
				if (rolls.at(1) == 5) {
					score += 50;
					bonusRolls++;
					break;
				}
			case 5:
				score += 50;
				break;
				//second die can't be five w/o other scoring
			default:
				if (rolls.at(1) == 5) {
					score += 50;
				}
			}
		}
		//3oak, dice 1-3
		else if (rolls.front() == rolls.at(2)) {
			score += rolls.front() == 1 ? 300 : rolls.front() * 100;
			//only need to check for 5s because a 1 can't be after the third die w/o other scoring
			//also cant award bonus roll w/o other scoring
			score += rolls.at(3) == 5 ? 50.0 : 0;
			score += rolls.at(4) == 5 ? 50.0 : 0;
			score += rolls.back() == 5 ? 50.0 : 0;
		}
		//3oak, dice 2-4
		else if (rolls.at(1) == rolls.at(3)) {
			score += rolls.at(1) == 1 ? 300 : rolls.at(1) * 100;
			//only check for 1 as it can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				score += 100.0;
				if (rolls.at(4) == 5) {
					score += 50.0;
					if (rolls.back() == 5) {
						score += 50.0;
						bonusRolls++;
					}
				}
				else if (rolls.back() == 5) {
					score += 50.0;
				}
			}
			else {
				score += rolls.at(4) == 5 ? 50.0 : 0;
				score += rolls.back() == 5 ? 50.0 : 0;
			}
		}
		//3oak, dice 3-5
		else if (rolls.at(2) == rolls.at(4)) {
			score += rolls.at(2) == 1 ? 300 : rolls.at(2) * 100;
			//first can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				score += 100;
				//only check if == 1, as if == 5, another scoring already happened
				if (rolls.at(1) == 1) {
					score += 100;
					if (rolls.back() == 1) {
						score += 100;
						bonusRolls++;
					}
					else if (rolls.back() == 5) {
						score += 50;
						bonusRolls++;
					}
				}
			}
			if (rolls.back() == 5) {
				score += 50;
			}
		}
		//3oak, dice 4-6
		else if (rolls.at(3) == rolls.back()) {
			score += rolls.back() == 1 ? 300 : rolls.back() * 100;
			//first can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				score += 100;
				if (rolls.at(1) == 1) {
					score += 100;
					if (rolls.at(2) == 5) {
						score += 50;
						bonusRolls++;
					}
				}
				else if (rolls.at(1) == 5) {
					score += 50;
					if (rolls.at(2) == 5) {
						score += 50;
						bonusRolls++;
					}
				}
			}
			else {
				if (rolls.at(1) == 5) {
					score += 50;
				}
				if (rolls.at(2) == 5) {
					score += 50;
				}
			}
		}
		//1s and 5s only
		else {
			for (unsigned int i : rolls) {
				score += i == 1 ? 100 : i == 5 ? 50 : 0;
			}
		}
		break;
	case 5:
		//5oak
		if (rolls.front() == rolls.back()) {
			score += 2000;
			bonusRolls++;
		}
		//4oak, dice 1-4
		else if ((rolls.front() == rolls.at(3))) {
			score += 1000;
			if (rolls.back() == 5) {
				score += 50;
				bonusRolls++;
			}
		}
		//4oak, dice 2-5
		else if ((rolls.at(1) == rolls.back())) {
			score += 1000;
			switch (rolls.front()) {
			case 1:
				score += 100;
				bonusRolls++;
			case 5:
				score += 50;
				bonusRolls++;
			}
		}
		//3oak, dice 1-3
		else if (rolls.front() == rolls.at(2)) {
			score += rolls.front() == 1 ? 300 : rolls.front() * 100;
			//only need to check for a 5 because a 1 can't be after the third die w/o other scoring
			if (rolls.at(3) == 5) {
				score += 50;
				if (rolls.back() == 5) {
					score += 50;
					bonusRolls++;
				}
			}
			else if (rolls.back() == 5) {
				score += 50;
			}
		}
		//3oak, dice 2-4
		else if (rolls.at(1) == rolls.at(3)) {
			score += rolls.at(1) == 1 ? 300 : rolls.at(1) * 100;
			//only check for 1 as it can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				score += 100;
				if (rolls.back() == 5) {
					score += 50;
					bonusRolls++;
				}
			}
			else if (rolls.back() == 5) {
				score += 50;
			}
		}
		//3oak, dice 3-5
		else if (rolls.at(2) == rolls.back()) {
			score += rolls.at(2) == 1 ? 300 : rolls.at(2) * 100;
			switch (rolls.front()) {
			case 1:
				score += 100;
				if (rolls.at(1) == 1) {
					score += 100;
					bonusRolls++;
				}
				else if (rolls.at(1) == 5) {
					score += 50;
					bonusRolls++;
				}
				break;
			case 5:
				score += 50;
				if (rolls.at(1) == 5) {
					score += 50;
					bonusRolls++;
				}
				break;
			default:
				if (rolls.at(1) == 5) {
					score += 50;
				}
			}
		}
		//1s and 5s only
		else {
			for (unsigned int i : rolls) {
				score += i == 1 ? 100 : i == 5 ? 50 : 0;
			}
		}
		break;
	case 4:
		//4oak
		if (rolls.front() == rolls.back()) {
			score += 1000;
			bonusRolls++;
		}
		//3oak, dice 1-3
		else if ((rolls.front() == rolls.at(2))) {
			score += rolls.front() == 1 ? 300 : rolls.front() * 100;
			if (rolls.back() == 5) {
				score += 50;
				bonusRolls++;
			}
		}
		//3oak, dice 2-4
		else if ((rolls.at(1) == rolls.back())) {
			score += rolls.back() == 1 ? 300 : rolls.back() * 100;
			switch (rolls.front()) {
			case 1:
				score += 100;
				bonusRolls++;
			case 5:
				score += 50;
				bonusRolls++;
			}
		}
		//1s and 5s only
		else {
			unsigned char* counter_1_5 = new(unsigned char);
			*counter_1_5 = 0;
			for (unsigned int i : rolls) {
				if (i == 1) {
					score += 100;
					(*counter_1_5)++;
				}
				else if (i == 5) {
					score += 50;
					(*counter_1_5)++;
				}
			}
			if (*counter_1_5 == 4) {
				bonusRolls++;
			}
			delete(counter_1_5);
		}
		break;
	case 3:
		//3oak
		if (rolls.front() == rolls.back()) {
			score += rolls.front() == 1 ? 300 : rolls.front() * 100;
			bonusRolls++;
		}
		//1s and 5s only
		else {
			unsigned char* counter_1_5 = new(unsigned char);
			*counter_1_5 = 0;
			for (unsigned int i : rolls) {
				if (i == 1) {
					score += 100;
					(*counter_1_5)++;
				}
				else if (i == 5) {
					score += 50;
					(*counter_1_5)++;
				}
			}
			if (*counter_1_5 == 3) {
				bonusRolls++;
			}
			delete(counter_1_5);
		}
		break;
	default:
		//1s and 5s only
		unsigned char* counter_1_5 = new(unsigned char);
		*counter_1_5 = 0;
		for (unsigned int i : rolls) {
			if (i == 1) {
				score += 100;
				(*counter_1_5)++;
			}
			else if (i == 5) {
				score += 50;
				(*counter_1_5)++;
			}
		}
		if (*counter_1_5 == dice) {
			bonusRolls++;
		}
		delete(counter_1_5);
	}

	return score;
}

//modified version of farkel_score for boolean usefulness
uint8_t farkel_can_score(std::vector<float> rolls = {}) {
	char dice = rolls.size();
	uint8_t canScore = 0b000000;
	
	//We're assuming the dice rolls come into the function sorted
	//std::sort(rolls.begin(), rolls.end());

	switch (dice) {
	case 6:
		//6oak, 2 triples, 3 pair, or straight
		if (
			(rolls.front() == rolls.back()) || 
			((rolls.front() == rolls.at(2)) && (rolls.at(3) == rolls.back())) ||
			((rolls.front() == rolls.at(1)) && (rolls.at(2) == rolls.at(3)) && (rolls.at(4) == rolls.back())) ||
			((rolls.front() == 1) && (rolls.at(1) == 2) && (rolls.at(2) == 3) && (rolls.at(3) == 4) && (rolls.at(4) == 5) && (rolls.back() == 6))
			) {
			canScore = 0b111111;
		}
		//5oak, dice 1-5
		else if ((rolls.front() == rolls.at(4))) {
			canScore = 0b111110;
			if (rolls.back() == 5) {
				canScore |= 0b000001;
			}
		}
		//5oak, dice 2-6
		else if ((rolls.at(1) == rolls.back())) {
			canScore = 0b011111;
			if ((rolls.front() == 5) || (rolls.front() == 1)) {
				canScore |= 0b100000;
			}
		}
		//4oak, dice 1-4
		else if (rolls.front() == rolls.at(3)) {
			canScore = 0b111100;
			//only need to check for a 5 because a 1 can't be after the fourth die w/o other scoring
			//also can't cause bonus roll w/o other scoring
			canScore |= rolls.at(4) == 5 ? 0b010000 : 0;
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//4oak, dice 2-5
		else if (rolls.at(1) == rolls.at(4)) {
			canScore = 0b011110;
			//only check for 1 as it can't be 5 w/o other scoring
			canScore |= rolls.front() == 1 ? 0b100000 : 0;
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//4oak, dice 3-6
		else if (rolls.at(2) == rolls.back()) {
			canScore = 0b001111;
			canScore |= rolls.front() == 1 ? 0b100000 : 0;
			canScore |= rolls.at(1) == 5 ? 0b010000 : 0;
		}
		//3oak, dice 1-3
		else if (rolls.front() == rolls.at(2)) {
			canScore = 0b111000;
			//only need to check for 5s because a 1 can't be after the third die w/o other scoring
			canScore |= rolls.at(3) == 5 ? 0b000100 : 0;
			canScore |= rolls.at(4) == 5 ? 0b000010 : 0;
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//3oak, dice 2-4
		else if (rolls.at(1) == rolls.at(3)) {
			canScore = 0b011100;
			//only check for 1 as it can't be 5 w/o other scoring
			canScore |= rolls.front() == 1 ? 0b100000 : 0;
			canScore |= rolls.at(4) == 5 ? 0b000010 : 0;
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//3oak, dice 3-5
		else if (rolls.at(2) == rolls.at(4)) {
			canScore = 0b001110;
			//first can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				canScore |= 0b100000;
				//only check if == 1, as if == 5, another scoring already happened
				canScore |= rolls.at(1) == 1 ? 0b010000 : 0;
			}
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//3oak, dice 4-6
		else if (rolls.at(3) == rolls.back()) {
			canScore = 0b000111;
			//first can't be 5 w/o other scoring
			if (rolls.front() == 1) {
				canScore |= 0b100000;
				canScore |= rolls.at(1) == 1 ? 0b010000 : 0;
			}
			canScore |= rolls.at(1) == 5 ? 0b010000 : 0;
			canScore |= rolls.at(2) == 5 ? 0b001000 : 0;
		}
		//1s and 5s only
		else {
			for (char i = 0; i < 6; i++) {//dice is guaranteed == 6, and need to shift dice - (i + 1), or just 5 - i
				canScore |= rolls.at(i) == 1 ? 0b1 << (5-i) : rolls.at(i) == 5 ? 0b1 << (5-i) : 0;
			}
		}
		break;
	case 5:
		//5oak
		if (rolls.front() == rolls.back()) {
			canScore = 0b11111;
		}
		//4oak, dice 1-4
		else if ((rolls.front() == rolls.at(3))) {
			canScore = 0b11110;
			canScore |= rolls.back() == 5 ? 0b000001 : 0;
		}
		//4oak, dice 2-5
		else if ((rolls.at(1) == rolls.back())) {
			canScore = 0b01111;
			canScore |= rolls.front() == 1 || rolls.front() == 5 ? 0b10000 : 0;
		}
		//3oak, dice 1-3
		else if (rolls.front() == rolls.at(2)) {
			canScore = 0b11100;
			//only need to check for a 5 because a 1 can't be after the third die w/o other scoring
			canScore |= rolls.at(3) == 5 ? 0b00010 : 0;
			canScore |= rolls.back() == 5 ? 0b00001 : 0;
		}
		//3oak, dice 2-4
		else if (rolls.at(1) == rolls.at(3)) {
			canScore = 0b01110;
			//only check for 1 as it can't be 5 w/o other scoring
			canScore |= rolls.front() == 1 ? 0b10000 : 0;
			canScore |= rolls.back() == 5 ? 0b00001 : 0;
		}
		//3oak, dice 3-5
		else if (rolls.at(2) == rolls.back()) {
			canScore = 0b00111;
			canScore |= rolls.front() == 1 || rolls.front() == 5 ? 0b10000 : 0;
			canScore |= rolls.at(1) == 1 || rolls.at(1) == 5 ? 0b10000 : 0;
		}
		//1s and 5s only
		else {
			for (char i = 0; i < 5; i++) {//dice is guaranteed == 5, and need to shift dice - (i + 1), or just 4 - i
				canScore |= rolls.at(i) == 1 || rolls.at(i) == 5 ? 0b1 << (4 - i) : 0;
			}
		}
		break;
	case 4:
		//4oak
		if (rolls.front() == rolls.back()) {
			canScore = 0b1111;
		}
		//3oak, dice 1-3
		else if ((rolls.front() == rolls.at(2))) {
			canScore = 0b1110;
			canScore |= rolls.back() == 5 ? 0b0001 : 0;
		}
		//3oak, dice 2-4
		else if ((rolls.at(1) == rolls.back())) {
			canScore = 0b0111;
			canScore |= rolls.front() == 1 ? 0b1000 : rolls.front() == 5 ? 0b1000 : 0;
		}
		//1s and 5s only
		else {
			for (char i = 0; i < 4; i++) {//dice is guaranteed == 4, and need to shift dice - (i + 1), or just 3 - i
				canScore |= rolls.at(i) == 1 ? 0b1 << (3 - i) : rolls.at(i) == 5 ? 0b1 << (3 - i) : 0;
			}
		}
		break;
	case 3:
		//3oak
		if (rolls.front() == rolls.back()) {
			canScore = 0b111;
		}
		//1s and 5s only
		else {
			for (char i = 0; i < 3; i++) {//dice is guaranteed == 3, and need to shift dice - (i + 1), or just 2 - i
				canScore |= rolls.at(i) == 1 ? 0b1 << (2 - i) : rolls.at(i) == 5 ? 0b1 << (2 - i) : 0;
			}
		}
		break;
	default:
		//1s and 5s only
		for (char i = 0; i < dice; i++) {
			canScore |= rolls.at(i) == 1 ? 0b1 << (dice - i - 1) : rolls.at(i) == 5 ? 0b1 << (dice - i - 1) : 0;
		}
	}
	return canScore;
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

float farkel_cost_func(NeuralNetwork* network) {
	std::vector<float> rolls = { 0, 0, 0, 0, 0, 0 };
	float cost = 0;
	float dif = 0;
	for (float r6 = 6; r6 >= 1; r6--) {
		rolls.at(5) = r6;
		for (float r5 = r6; r5 >= 1; r5--) {
			rolls.at(4) = r5;
			for (float r4 = r5; r4 >= 1; r4--) {
				rolls.at(3) = r4;
				for (float r3 = r4; r3 >= 1; r3--) {
					rolls.at(2) = r3;
					for (float r2 = r3; r2 >= 1; r2--) {
						rolls.at(1) = r2;
						for (float r1 = r2; r1 >= 1; r1--) {
							rolls.at(0) = r1;
							network->set_inputs(rolls);
							network->update_network();
							std::vector<float> networkOut = network->read_output();
							uint8_t truth = farkel_can_score(rolls);
							for (char i = 0; i < 6; i++) {
								dif = std::min(networkOut.at(i), float(1)) - float((truth & (0b1 << (5 - i))) ? 1.0 : 0.0);
								cost += dif * dif;
							}
						}
					}
				}
			}
		}
	}
	return cost;

}

float logarithmic_tune(int gen, NeuralNetwork* net) {
	return 0.15 * std::log(net->get_cost() + 2)/std::log(gen + 2);
}

float linear_tune(int gen, NeuralNetwork* net) {
	return net->get_cost() / float(gen + 1);
}

int main() {
	
	NeuralNetwork* base = new NeuralNetwork({ 6, 8, 8, 6 });

	NeuralNetwork* trained = base->random_evolve_train(farkel_cost_func, 500, 200, 4, logarithmic_tune);
	delete(base);

	trained->save_to_file("farkel_test_lin2.dat");
	trained->load_from_file("farkel_test_lin2.dat");

	std::cout << trained->get_structure() << "\n" << trained->get_cost();
	return 0;
}