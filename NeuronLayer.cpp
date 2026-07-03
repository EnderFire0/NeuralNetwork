#include "NeuronLayer.h"
#include <iostream>

NeuronLayer::NeuronLayer(int size) {
	this->input = nullptr;
	this->size = size;
	this->neurons = {};
	for (int i = 0; i < size; i++) {
		this->neurons.push_back(new Neuron());
	}
}

NeuronLayer::~NeuronLayer() {
	for (int i = this->size - 1; i > 0; i--) {
		delete(this->neurons[i]);
	}
}

void NeuronLayer::update_layer() {
	for (int i = 0; i < this->size; i++) {
		this->neurons[i]->update_activation();
	}
}

void NeuronLayer::connect_input_layer(NeuronLayer* layer) {
	this->input = layer;
	for (int i = 0; i < this->size; i++) {
		for (int o = 0; o < layer->size; o++) {
			this->neurons[i]->connect_neuron(layer->neurons[o]);
		}
	}
}

void NeuronLayer::disconnect_input_layer() {
	for (int i = 0; i < size; i++) {
		this->neurons[i]->disconnect_inputs();
	}
}

void NeuronLayer::set_neuron_activations(std::vector<float> activations) {
	if (activations.size() == size) {
		for (int i = 0; i < size; i++) {
			this->neurons[i]->set_activation(activations[i]);
		}
	}
	else {
		std::cout << "Unable to set layer neuron activations: Incorrect number of activations\n";
	}
}

void NeuronLayer::insert_neuron(Neuron* neuron) {
	if (this->input != nullptr) {
		for (int i = 0; i < this->input->size; i++) {
			neuron->connect_neuron(this->input->neurons[i]);
		}
	}
	this->neurons.push_back(neuron);
	this->size++;
}

void NeuronLayer::random_tune_neurons(float strength) {
	for (int i = 0; i < size; i++) {
		this->neurons[i]->random_tune_connections(strength);
	}
}

int NeuronLayer::get_size() {
	return this->size;
}

std::vector<float> NeuronLayer::read_neurons() {
	std::vector<float> output = {};
	for (int i = 0; i < this->size; i++) {
		output.push_back(this->neurons[i]->read());
	}
	return output;
}

std::vector<std::vector<NeuronConnection*>> NeuronLayer::get_connections() {
	std::vector<std::vector<NeuronConnection*>> connections = {};
	for (int i = 0; i < this->size; i++) {
		connections.push_back(this->neurons[i]->get_connections());
	}
	return connections;
}

std::vector<Neuron*> NeuronLayer::get_neurons() {
	return this->neurons;
}