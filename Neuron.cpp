#include "Neuron.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

Neuron::Neuron() {
	this->inputs = {};
	this->activation = 0;
	this->bias = 0;
}

Neuron::~Neuron() {
	for (int i = this->inputs.size() - 1; i > 0; i--) {
		delete(this->inputs[i]);
	}
}

void Neuron::update_activation() {
	//in case that input layer get updated, avoids erroring
	if (this->inputs.size() > 0) {
		this->activation = this->bias;
		for (int i = 0; i < this->inputs.size(); i++) {
			this->activation += (this->inputs[i]->input->activation * this->inputs[i]->weight);
		}
	}
}

void Neuron::set_activation(float activation) {
	this->activation = activation;
}

void Neuron::connect_neuron(Neuron* neuron, float weight) {
	NeuronConnection* connection = new NeuronConnection(neuron, weight);
	this->inputs.push_back(connection);
}

void Neuron::disconnect_inputs() {
	for (int i = this->inputs.size() - 1; i >= 0; i--) {
		delete(inputs[i]);
	}
	this->inputs.clear();
}

void Neuron::random_tune(float strength) {
	for (int i = 0; i < this->inputs.size(); i++) {
		this->inputs[i]->weight += dis(gen) * strength;
	}
	this->bias += dis(gen) * strength;
}

float Neuron::read_activation() {
	return this->activation;
}

std::vector<NeuronConnection*> Neuron::get_connections() {
	return this->inputs;
}