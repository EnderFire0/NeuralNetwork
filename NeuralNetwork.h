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
	double cost;
public:
	NeuralNetwork(std::vector<int> shape = {});
	NeuralNetwork(const NeuralNetwork& network);
	~NeuralNetwork();

	NeuralNetwork& operator= (const NeuralNetwork& network);

	NeuralNetwork& operator+=(const NeuralNetwork& rhs);

	NeuralNetwork& operator/=(const double& rhs);
	NeuralNetwork& operator/=(const float& rhs);
	NeuralNetwork& operator/=(const long& rhs);
	NeuralNetwork& operator/=(const int& rhs);
	NeuralNetwork& operator/=(const short& rhs);
	NeuralNetwork& operator/=(const char& rhs);

	void update_network();
	void append_layer(NeuronLayer* layer);
	void set_inputs(std::vector<double> inputs);
	void reset_cost();
	void modify_cost(double cost);
	void insert_layer(NeuronLayer* layer, int index);
	void random_tune_network(double strength);
	void load_from_file(std::string filename);
	const void save_to_file(std::string filename);

	const std::string get_structure();

	const double get_cost() { return this->cost; };

	const std::vector<int> get_shape();

	const std::vector<double> read_output();

	NeuralNetwork* fine_tune_train(double(*costFunc)(NeuralNetwork*), int iterations);
	NeuralNetwork* random_evolve_train(double(*costFunc)(NeuralNetwork*), int genCount, int countPerGen, int parentCount = 1, double(*tuneFunc)(int, NeuralNetwork*) = nullptr);
};

#endif