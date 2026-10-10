#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron {
public:
	std::vector<Neuron*> inputs;
	std::vector<double> weights;
	double bias;
	double activation;

	Neuron(std::vector<double> w = {}, double b = 0);
	Neuron(const Neuron& neuron);//does not connect to any input Neurons

	Neuron& operator+=(const Neuron& rhs);

	Neuron& operator/=(const double& rhs);
	Neuron& operator/=(const float& rhs);
	Neuron& operator/=(const long& rhs);
	Neuron& operator/=(const int& rhs);
	Neuron& operator/=(const short& rhs);
	Neuron& operator/=(const char& rhs);

	void connect_neuron(Neuron* neuron = nullptr, double weight = 0);
	void disconnect_inputs();
	void random_tune(double strength);
	void update_activation();
};

#endif