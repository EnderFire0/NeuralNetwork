#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuronLayer.h"

class NeuralNetwork {
private:
	std::vector<NeuronLayer*> layers;
	int size;
	int score;
public:
	NeuralNetwork(std::vector<int> shape);
	~NeuralNetwork();

	void update_network();
	void append_layer(NeuronLayer* layer);
	void set_inputs(std::vector<float> inputs);
	void set_score(int score);
	void modify_score(int score);
	void insert_layer(NeuronLayer* layer, int index);
	void random_tune_network(float strength);
	void print_structure();

	int get_score();

	std::vector<int> get_shape();

	std::vector<float> read_output();

	NeuralNetwork* duplicate();
};

#endif