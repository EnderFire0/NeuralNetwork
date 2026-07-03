#include "Neuron.h"

Neuron::Neuron() {
	this->inputs = {};
	this->activation = 0;
}

Neuron::~Neuron() {
	for (int i = this->inputs.size() - 1; i > 0; i--) {
		delete(this->inputs[i]);
	}
}

void Neuron::update_activation() {
	//in case that input layer get updated, avoids erroring
	if (this->inputs.size() > 0) {
		this->activation = 0;
		for (int i = 0; i < this->inputs.size(); i++) {
			this->activation += (this->inputs[i]->input->activation * this->inputs[i]->weight) + this->inputs[i]->bias;
		}
	}
}

void Neuron::set_activation(float activation) {
	this->activation = activation;
}

void Neuron::connect_neuron(Neuron* neuron, float weight, float bias) {
	NeuronConnection* connection = new NeuronConnection(neuron, weight, bias);
	this->inputs.push_back(connection);
}

void Neuron::disconnect_inputs() {
	for (int i = this->inputs.size() - 1; i >= 0; i--) {
		delete(inputs[i]);
	}
	this->inputs.clear();
}

void Neuron::random_tune_connections(float strength) {
	for (int i = 0; i < this->inputs.size(); i++) {
		this->inputs[i]->random_tune(strength);
	}
}

float Neuron::read() {
	return this->activation;
}

std::vector<NeuronConnection*> Neuron::get_connections() {
	return this->inputs;
}