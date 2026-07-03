#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include "Neuron.h"

class NeuronLayer {
private:
	std::vector<Neuron*> neurons;
	NeuronLayer* input;
	int size;
public:
	NeuronLayer(int size);
	~NeuronLayer();

	void update_layer();
	void connect_input_layer(NeuronLayer* layer);
	void disconnect_input_layer();
	void set_neuron_activations(std::vector<float> activations);
	void insert_neuron(Neuron* neuron);
	void random_tune_neurons(float strength);

	int get_size();

	std::vector<float> read_neurons();

	std::vector<std::vector<NeuronConnection*>> get_connections();

	std::vector<Neuron*> get_neurons();
};

#endif