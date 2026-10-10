#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include "Neuron.h"

class NeuronLayer {
private:
	std::vector<Neuron*> neurons;
	NeuronLayer* input;
	int size;
public:
	NeuronLayer(int size = 0);
	NeuronLayer(std::vector<Neuron*> neurons);
	NeuronLayer(const NeuronLayer& layer);//does not connect to any input Layer
	~NeuronLayer();

	NeuronLayer& operator+=(const NeuronLayer& rhs);

	NeuronLayer& operator/=(const double& rhs);
	NeuronLayer& operator/=(const float& rhs);
	NeuronLayer& operator/=(const long& rhs);
	NeuronLayer& operator/=(const int& rhs);
	NeuronLayer& operator/=(const short& rhs);
	NeuronLayer& operator/=(const char& rhs);

	int get_size();

	std::vector<double> read_neurons();

	std::vector<Neuron*> get_neurons();

	void connect_input_layer(NeuronLayer* layer);
	void disconnect_input_layer();
	void insert_neuron(Neuron* neuron, std::vector<double> weights = {});
	void random_tune_neurons(double strength);
	void set_neuron_activations(std::vector<double> activations);
	void update_layer();
};

#endif