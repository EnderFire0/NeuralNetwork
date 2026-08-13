#include "NeuralNetwork.h"
#include <iostream>
#include <algorithm>

const static struct
{
	bool operator()(NeuralNetwork* a, NeuralNetwork* b) const { return a->get_cost() > b->get_cost(); }
}
sortingObj;

static float CONSTANT_ONE(int gen, NeuralNetwork* net) { return 1; }

NeuralNetwork::NeuralNetwork(std::vector<int> shape) {
	this->layers = {};
	this->shape = {};
	this->size = 0;
	for (int i : shape) {
		this->append_layer(new NeuronLayer(i));
	}
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& network) {
	this->layers = {};
	this->size = 0;
	this->shape = {};
	this->cost = network.cost;

	for (NeuronLayer* oldLay : network.layers) {
		std::vector<Neuron*> oldNeurons = oldLay->get_neurons();
		std::vector<Neuron*> newNeurons = {};

		for (Neuron* oldNeu : oldNeurons) {
			Neuron* newNeuron = new Neuron(*oldNeu);
			newNeurons.push_back(newNeuron);
		}
		NeuronLayer* newLayer = new NeuronLayer(newNeurons);

		this->append_layer(newLayer);
	}
}

NeuralNetwork::~NeuralNetwork() {
	for (int i = this->size - 1; i >= 0; i--) {
		delete(this->layers[i]);
	}
}

NeuralNetwork& NeuralNetwork::operator= (const NeuralNetwork& network) {
	if (this == &network) {
		return *this;
	}

	for (int i = this->size - 1; i >= 0; i--) {
		delete(this->layers[i]);
		this->layers.pop_back();
	}

	this->size = 0;
	this->shape = {};
	this->cost = network.cost;

	for (NeuronLayer* oldLay : network.layers) {
		NeuronLayer* newLayer = new NeuronLayer(0);

		std::vector<Neuron*> oldNeurons = oldLay->get_neurons();

		for (Neuron* oldNeu : oldNeurons) {
			Neuron* newNeuron = new Neuron(*oldNeu);
			newLayer->insert_neuron(newNeuron);
		}

		this->append_layer(newLayer);
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator+=(const NeuralNetwork& rhs) {
	if (this->shape != rhs.shape) {
		throw std::runtime_error("Incompatbile shapes");
	}

	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) += *(rhs.layers.at(i));
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const double& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const float& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const long& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const int& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const short& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

NeuralNetwork& NeuralNetwork::operator/=(const char& rhs) {
	for (int i = 0; i < this->size; i++) {
		*(this->layers.at(i)) /= rhs;
	}

	return *this;
}

void NeuralNetwork::update_network() {
	//skip updating input layer to avoid losing data from no input neurons on that layer
	for (int i = 1; i < this->size; i++) {
		this->layers[i]->update_layer();
	}
}

void NeuralNetwork::append_layer(NeuronLayer* layer) {
	if (!this->layers.empty()) {
		layer->connect_input_layer(this->layers.back());
	}
	this->layers.push_back(layer);
	this->shape.push_back(layer->get_size());
	this->size++;
}

void NeuralNetwork::set_inputs(std::vector<float> inputs) {
	this->layers.at(0)->set_neuron_activations(inputs);
}

void NeuralNetwork::reset_cost() {
	this->cost = 0;
}

void NeuralNetwork::modify_cost(float cost) {
	this->cost += cost;
}

//inserts the layer to be at index # "index", e.g. if layers = {a, b, c}, insert_layer(l, 1) -> layers = {a, l, b, c}
void NeuralNetwork::insert_layer(NeuronLayer* layer, int index) {
	if ((index < this->size) && (index >= 0)) {
		this->layers[index]->disconnect_input_layer();
		this->layers[index]->connect_input_layer(layer);
		this->layers.insert(std::next(this->layers.begin(), index), layer);
		if (index != 0) {
			layer->connect_input_layer(this->layers[index - 1]);
		}
		this->size++;
	}
	else {
		std::cout << "Unable to insert layer: Index out of bounds\n";
	}
}

void NeuralNetwork::random_tune_network(float strength) {
	for (int i = 0; i < this->size; i++) {
		this->layers[i]->random_tune_neurons(strength);
	}
}

void NeuralNetwork::load_from_file(std::string filename) {
	//destructor to be able to modify in-place
	for (int i = this->size - 1; i >= 0; i--) {
		delete(this->layers[i]);
		this->layers.pop_back();
	}
	this->shape = {};
	this->size = 0;

	std::uint8_t intSize;
	std::uint8_t floatSize;
	std::uint16_t layerCount;
	std::vector<int> layerSizes = {};
	
	std::ifstream f{ filename, std::ios_base::in | std::ios_base::binary };
	if (!f.is_open()) { return; }
	else {
		//reading header information to know how to read file
		f.read(reinterpret_cast<char*>(&intSize), sizeof(std::uint8_t));
		f.read(reinterpret_cast<char*>(&floatSize), sizeof(std::uint8_t));
		//reading information about shape of the network
		f.read(reinterpret_cast<char*>(&layerCount), sizeof(std::uint16_t));
		//for (int layer = 0; layer < layerCount; layer++) { }
		for (int layer = 0; layer < layerCount; layer++) {
			layerSizes.push_back(0);
			f.read(reinterpret_cast<char*>(&(layerSizes.at(layer))), intSize);
		}
		//closing to reduce risk while building network
		f.close();
	}
	for (int layer : layerSizes) {
		this->append_layer(new NeuronLayer(layer));
	}
	f.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!f.is_open()) { return; }
	else {
		f.seekg(4 + intSize * layerCount, std::ios::beg);//4 comes from the four bytes used to store intSize, floatSize, and layerCount

		//looping through layers
		//start at layer 1 as layer 0 has no information(weights/biases)
		for (int lyr = 1; lyr < layerCount; lyr++) {
			//looping through neurons
			NeuronLayer* layer = this->layers.at(lyr);
			std::vector<Neuron*> layerNeurons = layer->get_neurons();
			for (int neu = 0; neu < layer->get_size(); neu++) {
				//looping through connection weights
				Neuron* neuron = layerNeurons.at(neu);
				std::vector<float>* weights = &neuron->weights;
				for (int wght = 0; wght < neuron->inputs.size(); wght++) {
					if (!f) { std::cout << "Bad file read\nfail: " << f.fail() << "\nbad: " << f.bad() << "\neof: " << f.eof() << "\n"; f.close(); return; }
					//reading neural connection weight data
					f.read(reinterpret_cast<char*>(&(weights->at(wght))), floatSize);
				}
				//neuron bias data
				f.read(reinterpret_cast<char*>(&(neuron->bias)), floatSize);
			}
		}
		//setting cost value
		f.read(reinterpret_cast<char*>(&(this->cost)), floatSize);
	}
	f.close();
}

const void NeuralNetwork::save_to_file(std::string filename) {
	if (!this->size) { return; }

	std::uint8_t intSize = sizeof(int);
	std::uint8_t floatSize = sizeof(float);
	std::uint16_t layerCount = this->size;//casting to uint16_t to force consistency across architechtures

	std::ofstream f{ filename , std::ios_base::out | std::ios_base::binary };
	if (!f.is_open()) { f.close(); return; }
	else {
		//writing header information to make file readable across architectures
		f.write(reinterpret_cast<char*>(&intSize), sizeof(std::uint8_t));
		f.write(reinterpret_cast<char*>(&floatSize), sizeof(std::uint8_t));
		//all the information about the shape of the network needed to reproduce it
		f.write(reinterpret_cast<char*>(&layerCount), sizeof(std::uint16_t));
		for (int pos = 0; pos < this->size; pos++) {
			f.write(reinterpret_cast<char*>(&(this->shape.at(pos))), intSize);
		}
		//looping through layers
		//start at layer 1 as layer 0 has no information(weights/biases)
		for (int lyr = 1; lyr < this->size; lyr++) {
			//looping through neurons
			NeuronLayer* layer = this->layers.at(lyr);
			std::vector<Neuron*> layerNeurons = layer->get_neurons();
			for (int neu = 0; neu < layer->get_size(); neu++) {
				//looping through connection weights
				Neuron* neuron = layerNeurons.at(neu);
				std::vector<float> weights = neuron->weights;
				for (int wght = 0; wght < weights.size(); wght++) {
					//writing neural connection weight data
					f.write(reinterpret_cast<char*>(&(weights.at(wght))), floatSize);
				}
				//neuron bias data
				f.write(reinterpret_cast<char*>(&(neuron->bias)), floatSize);
			}
		}
		//writing cost value
		f.write(reinterpret_cast<char*>(&(this->cost)), floatSize);
		f.flush();
	}
	f.close();
}

const std::string NeuralNetwork::get_structure() {
	std::string output = "";
	output = output + "Structure:\n";
	for (int i = 1; i < this->size; i++) {
		output = output + "Layer " + std::to_string(i) + ":\n";
		for (int o = 0; o < this->layers[i]->get_size(); o++) {
			output = output + "Neuron " + std::to_string(o) + ": ";
			Neuron* neuron = this->layers[i]->get_neurons()[o];
			for (int u = 0; u < neuron->weights.size(); u++) {
				output = output + "(" + std::to_string(neuron->weights[u]) + "), ";
			}
			output = output + "[" + std::to_string(neuron->bias) + "]\n";
		}
	}
	return output;
}

const std::vector<int> NeuralNetwork::get_shape() {
	std::vector<int> shape = {};
	for (int i = 0; i < this->size; i++) { shape.push_back(this->layers[i]->get_size()); }
	return shape;
}

const std::vector<float> NeuralNetwork::read_output() {
	return this->layers.back()->read_neurons();
}

NeuralNetwork* NeuralNetwork::random_evolve_train(float(*costFunc)(NeuralNetwork*), int genCount, int countPerGen, int parentCount, float(*tuneStrengthFunc)(int, NeuralNetwork*)) {
	if (parentCount < 1 || genCount < 1 || countPerGen < 1 || countPerGen < parentCount) { return nullptr; }
	if (!tuneStrengthFunc) { tuneStrengthFunc = CONSTANT_ONE; }
	
	NeuralNetwork* templateNet = new NeuralNetwork(*this);

	for (int generation = 0; generation < genCount; generation++) {

		std::vector<NeuralNetwork*> parents = {};
		for (int i = 0; i < parentCount; i++) { parents.push_back(nullptr); }


		for (int member = 0; member < countPerGen; member++) {
			NeuralNetwork* test_member = new NeuralNetwork(*templateNet);
			test_member->random_tune_network(tuneStrengthFunc(generation, test_member));

			test_member->cost = costFunc(test_member);
			
			if (member < parentCount) {
				parents.at(member) = test_member;
			}
			else {
				std::sort(parents.begin(), parents.end(), sortingObj);
				if (parents.front()->cost > test_member->cost) {
					delete(parents.front());
					parents.front() = test_member;
				}
				else {
					delete(test_member);
				}
			}
			
		}
		NeuralNetwork* copy = new NeuralNetwork(templateNet->shape);

		for (NeuralNetwork* parent : parents) {
			*copy += *parent;
			delete(parent);
		}
		*copy /= float(parentCount);

		delete(templateNet);
		templateNet = copy;
	}
	templateNet->cost = costFunc(templateNet);

	return templateNet;
}