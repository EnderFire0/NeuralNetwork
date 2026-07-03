#ifndef NEURON_H
#define NEURON_H

#include "NeuronConnection.h"
#include <vector>

class Neuron {
private:
	std::vector<NeuronConnection*> inputs;
	float activation;
public:
	Neuron();
	~Neuron();

	void update_activation();
	void set_activation(float activation);
	void connect_neuron(Neuron* neuron = nullptr, float weight = 1, float bias = 0);
	void disconnect_inputs();
	void random_tune_connections(float strength);

	float read();

	std::vector<NeuronConnection*> get_connections();
};

#endif