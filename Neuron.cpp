#include "Neuron.h"
#include <random>
#include <cmath>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

Neuron::Neuron(std::vector<float> w, float b) {
	this->inputs = {};
	this->weights = w;
	this->activation = 0;
	this->bias = b;
}

//does not connect to any input Neurons
Neuron::Neuron(const Neuron& neuron) {
	this->inputs = {};
	this->activation = neuron.activation;
	this->weights = neuron.weights;
	this->bias = neuron.bias;
}

Neuron& Neuron::operator+=(const Neuron& rhs) {
	if (this->weights.size() != rhs.weights.size()) {
		throw std::runtime_error("Incompatible weight ranges");
	}

	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) += rhs.weights.at(i);
	}
	this->bias += rhs.bias;

	return *this;
}

Neuron& Neuron::operator/=(const double& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

Neuron& Neuron::operator/=(const float& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

Neuron& Neuron::operator/=(const long& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

Neuron& Neuron::operator/=(const int& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

Neuron& Neuron::operator/=(const short& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

Neuron& Neuron::operator/=(const char& rhs) {
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights.at(i) /= rhs;
	}
	this->bias /= rhs;

	return *this;
}

void Neuron::connect_neuron(Neuron* neuron, float weight) {
	this->inputs.push_back(neuron);
	if (this->inputs.size() > this->weights.size()) {
		this->weights.push_back(weight);
	}
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