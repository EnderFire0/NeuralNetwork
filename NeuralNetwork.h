#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuronLayer.h"
#include <string>
#include <fstream>

class NeuralNetwork {
private:
	std::vector<NeuronLayer*> layers;
	std::vector<int> shape;
	int size;
	float cost;
public:
	NeuralNetwork(std::vector<int> shape);
	NeuralNetwork(const NeuralNetwork& network);
	~NeuralNetwork();
	NeuralNetwork& operator= (const NeuralNetwork& network);

	void update_network();
	void append_layer(NeuronLayer* layer);
	void set_inputs(std::vector<float> inputs);
	void reset_cost();
	void modify_cost(float cost);
	void insert_layer(NeuronLayer* layer, int index);
	void random_tune_network(float strength);
	void load_from_file(std::string filename);
	const void save_to_file(std::string filename);

	const std::string get_structure();

	const float get_cost() { return this->cost; };

	const std::vector<int> get_shape();

	const std::vector<float> read_output();

	NeuralNetwork* random_evolve_train(float(*costFunc)(NeuralNetwork*), int genCount, int countPerGen, int parentCount = 1, float(*tuneFunc)(int, NeuralNetwork*) = nullptr);
};

#endif