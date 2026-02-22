#ifndef NEURONCONNECTION_H
#define NEURONCONNECTION_H

class Neuron;

class NeuronConnection {
public:
	Neuron* input;
	float weight;
	float bias;

	NeuronConnection(Neuron* i = nullptr, float w = 0, float b = 0);
	void random_tune(float strength);
};

#endif