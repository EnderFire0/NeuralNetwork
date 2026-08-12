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
	Neuron(const Neuron& neuron);//does not connect to any input Neurons

	Neuron& operator+=(const Neuron& rhs);

	Neuron& operator/=(const double& rhs);
	Neuron& operator/=(const float& rhs);
	Neuron& operator/=(const long& rhs);
	Neuron& operator/=(const int& rhs);
	Neuron& operator/=(const short& rhs);
	Neuron& operator/=(const char& rhs);

	void connect_neuron(Neuron* neuron = nullptr, float weight = 0);
	void disconnect_inputs();
	void random_tune(float strength);
	void update_activation();
};

#endif