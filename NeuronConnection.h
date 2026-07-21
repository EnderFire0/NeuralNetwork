#ifndef NEURONCONNECTION_H
#define NEURONCONNECTION_H

class Neuron;

class NeuronConnection {
public:
	Neuron* input;
	float weight;

	NeuronConnection(Neuron* i = nullptr, float w = 0);
};

#endif