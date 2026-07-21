#include "NeuronConnection.h"

NeuronConnection::NeuronConnection(Neuron* i, float w) {
	this->input = i;
	this->weight = w;
}
