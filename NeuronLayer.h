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
	NeuronLayer(const NeuronLayer& layer);//does not connect to any input Layer
	~NeuronLayer();

	int get_size();

	std::vector<float> read_neurons();

	std::vector<Neuron*> get_neurons();

	void connect_input_layer(NeuronLayer* layer);
	void disconnect_input_layer();
	void insert_neuron(Neuron* neuron, std::vector<float> weights = {});
	void random_tune_neurons(float strength);
	void set_neuron_activations(std::vector<float> activations);
	void update_layer();
};

#endif