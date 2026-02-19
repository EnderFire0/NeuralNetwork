#include "NeuronConnection.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

NeuronConnection::NeuronConnection(Neuron* i, float w, float b) {
	this->input = i;
	this->weight = w;
	this->bias = b;
}

void NeuronConnection::random_tune(float strength) {
	this->weight += dis(gen) * strength;
	this->bias += dis(gen) * strength;
}
