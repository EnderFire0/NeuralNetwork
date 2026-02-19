#include "NeuralNetwork.h"
#include <iostream>

NeuralNetwork::NeuralNetwork(std::vector<int> shape) {
	this->layers = {};
	this->size = 0;
	for (int i : shape) {
		this->append_layer(new NeuronLayer(i));
	}
}

NeuralNetwork::~NeuralNetwork() {
	for (int i = this->size - 1; i > 0; i--) {
		delete(this->layers[i]);
	}
}

void NeuralNetwork::update_network() {
	//skip updating input layer to avoid losing data from no input neurons on that layer
	for (int i = 1; i < this->size; i++) {
		this->layers[i]->update_layer();
	}
}

void NeuralNetwork::append_layer(NeuronLayer* layer) {
	if (this->size > 0) {
		layer->connect_input_layer(this->layers.back());
	}
	this->layers.push_back(layer);
	this->size++;
}

void NeuralNetwork::set_inputs(std::vector<float> inputs) {
	this->layers.at(0)->set_neuron_values(inputs);
}

void NeuralNetwork::set_score(int score) {
	this->score = score;
}

void NeuralNetwork::modify_score(int score) {
	this->score += score;
}

//inserts the layer to be at index # "index", e.g. if layers = {a, b, c}, insert_layer(l, 1) -> layers = {a, l, b, c}
void NeuralNetwork::insert_layer(NeuronLayer* layer, int index) {
	if ((index < this->size) && (index >= 0)) {
		this->layers[index]->disconnect_input_layer();
		this->layers[index]->connect_input_layer(layer);
		this->layers.insert(std::next(this->layers.begin(), index), layer);
		if (index != 0) {
			layer->connect_input_layer(this->layers[index - 1]);
		}
		this->size++;
	}
	else {
		std::cout << "Unable to insert layer: Index out of bounds\n";
	}
}

void NeuralNetwork::random_tune_network(float strength) {
	for (int i = 0; i < this->size; i++) {
		this->layers[i]->random_tune_neurons(strength);
	}
}

int NeuralNetwork::get_score() {
	return this->score;
}

void NeuralNetwork::print_structure() {
	std::cout << "Structure:\n";
	for (int i = 1; i < this->size; i++) {
		std::cout << "Layer " << i << ":\n";
		for (int o = 0; o < this->layers[i]->get_size(); o++) {
			std::cout << "Neuron " << o << ": ";
			std::vector<NeuronConnection*> connections = this->layers[i]->get_neurons()[o]->get_connections();
			for (int u = 0; u < connections.size(); u++) {
				std::cout << "(" << connections[u]->weight << ", " << connections[u]->bias << ") ";
			}
			std::cout << "\n";
		}
	}
}

std::vector<int> NeuralNetwork::get_shape() {
	std::vector<int> shape = {};
	for (int i = 0; i < this->size; i++) { shape.push_back(this->layers[i]->get_size()); }
	return shape;
}

std::vector<float> NeuralNetwork::read_output() {
	return this->layers.back()->read_neurons();
}

NeuralNetwork* NeuralNetwork::duplicate() {
	NeuralNetwork* copy = new NeuralNetwork(this->get_shape());

	std::vector<std::vector<NeuronConnection*>> connections;
	for (int layer = 0; layer < this->size; layer++) {

		connections = this->layers[layer]->get_connections();
		std::vector<Neuron*> copyLayerNeurons = copy->layers[layer]->get_neurons();

		for (int neuron = 0; neuron < connections.size(); neuron++) {

			std::vector<NeuronConnection*> copyConnections = copyLayerNeurons[neuron]->get_connections();

			for (int connection = 0; connection < connections[neuron].size(); connection++) {
				copyConnections[connection]->weight = connections[neuron][connection]->weight;
				copyConnections[connection]->bias = connections[neuron][connection]->bias;
			}
		}
	}

	return copy;
}