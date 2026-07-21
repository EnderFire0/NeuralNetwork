#include "Neuron.h"
#include <random>
#include <cmath>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

Neuron::Neuron() {
	this->inputs = {};
	this->weights = {};
	this->activation = 0;
	this->bias = 0;
}

void Neuron::update_activation() {
	//in case that input layer get updated, avoids erroring
	if (this->inputs.size() > 0) {
		this->activation = this->bias;
		for (int i = 0; i < this->inputs.size(); i++) {
			this->activation += (this->inputs[i]->activation * this->weights[i]);
		}
		this->activation = std::max(float(0), this->activation);
	}
}

void Neuron::connect_neuron(Neuron* neuron, float weight) {
	this->inputs.push_back(neuron);
	this->weights.push_back(weight);
}

void Neuron::disconnect_inputs() {
	this->inputs.clear();
	this->weights.clear();
}

void Neuron::random_tune(float strength) {
	for (int i = 0; i < this->inputs.size(); i++) {
		this->weights[i] += dis(gen) * strength;
	}
	this->bias += dis(gen) * strength * this->weights.size();
}