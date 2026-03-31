#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuronLayer.h"
#include <string>

class NeuralNetwork {
private:
	std::vector<NeuronLayer*> layers;
	std::vector<int> shape;
	int size;
	float fitness;
public:
	NeuralNetwork(std::vector<int> shape);
	NeuralNetwork(const NeuralNetwork& network);
	~NeuralNetwork();
	NeuralNetwork& operator= (const NeuralNetwork& network);

	void update_network();
	void append_layer(NeuronLayer* layer);
	void set_inputs(std::vector<float> inputs);
	void reset_fitness();
	void modify_fitness(float fitness);
	void insert_layer(NeuronLayer* layer, int index);
	void random_tune_network(float strength);

	const std::string get_structure();

	const float get_fitness() { return this->fitness; };

	const std::vector<int> get_shape();

	const std::vector<float> read_output();

	NeuralNetwork* train(float(*fitnessFunc)(NeuralNetwork*), int genCount, int countPerGen, int parentCount = 1, float(*tuneFunc)(int, NeuralNetwork*) = nullptr);
};

#endif