#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron {
public:
	std::vector<Neuron*> inputs;
	std::vector<float> weights;
	float bias;
	float activation;

	Neuron();

	void update_activation();
	void connect_neuron(Neuron* neuron = nullptr, float weight = 0);
	void disconnect_inputs();
	void random_tune(float strength);
};

#endif