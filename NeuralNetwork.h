#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuronLayer.h"

float defaultTuneStr(int gen) {
	return 1;
};

class NeuralNetwork {
private:
	std::vector<NeuronLayer*> layers;
	int size;
	float score;
public:
	NeuralNetwork(std::vector<int> shape);
	~NeuralNetwork();

	void update_network();
	void append_layer(NeuronLayer* layer);
	void set_inputs(std::vector<float> inputs);
	void reset_score();
	void modify_score(float score);
	void insert_layer(NeuronLayer* layer, int index);
	void random_tune_network(float strength);
	void print_structure();

	int get_score();

	std::vector<int> get_shape();

	std::vector<float> read_output();

	NeuralNetwork* duplicate();
	NeuralNetwork* train(float(*scoreFunc)(NeuralNetwork*), int genCount, int countPerGen, int parentCount, float(*tuneStrengthFunc)(int) = defaultTuneStr);
};

#endif