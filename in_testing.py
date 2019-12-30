import numpy as np
import pickle as cPickle

class Perceptron:
	def __init__(self, input_size,index,rata_invatare,strat):
		self.strat=strat
		self.index = index

		self.input_size = input_size
		self.rata_invatare=rata_invatare
		self.bias = 0.5
		if input_size >0:
			self.weights_list = np.random.normal(0, 1 / np.sqrt(input_size), input_size)
		else :
			self.weights_list=[]
		self.valoare=0


	def functie_activare(self,strat_ant):
		result = np.sum(self.weights_list *strat_ant) + self.bias;
		return result
		
	def prag(self,valoare):
		if valoare > 0:
			return 1 
		return 0
		
	def semiliniara(self,valoare):
		if valoare<0:
			return 0
		elif valoare<1 :
			return valoare
		return 1
		
	def sigmoid(self,valoare):
		return (1 - np.exp(-2*valoare))/(1 + np.exp(-2*valoare))

	def backprop_perceptron(self,strat_ant,expected):
		eroare =expected- self.sigmoid(self.functie_activare(strat_ant))
		
		self.weights_list+=eroare*strat_ant * self.rata_invatare
		self.bias+=eroare*self.rata_invatare;

class PerceptronNetwork:
	def __init__(self, output_size, input_size, learning_rate):
		self.perceptron_list = []
		self.straturi_perceptroni=[]
		self.straturi=[]
		self.output_size = output_size
		for i in range(input_size):
			self.perceptron_list.append(Perceptron(0, i, 0, 0))
		for i in range (0,16):
			self.perceptron_list.append(Perceptron(input_size, i, learning_rate, 1))
			self.perceptron_list.append(Perceptron(16, i, learning_rate, 2))
		for i in range(output_size):
			self.perceptron_list.append(Perceptron(16, i,learning_rate, 3))
		self.compile_strats()

	def compile_strats(self):
		for i in range(4):
			strat_ant= [e for e in self.perceptron_list if e.strat==i]
			self.straturi_perceptroni.append(strat_ant)


	def compile_img(self,img,strat):
		if strat is 0:
			for e in self.straturi_perceptroni[0]:
				e.valoare=img[e.index]
			strat_ant = [e.valoare for e in self.perceptron_list if e.strat == 0]
			self.straturi.append(strat_ant)
		if strat >0:
			for e in self.straturi_perceptroni[strat]:
				e.valoare=e.functie_activare(self.straturi[strat-1])
			strat_ant = [e.valoare for e in self.perceptron_list if e.strat == strat]
			self.straturi.append(strat_ant)

	def back_prop(self,expected):
		for k in range (3,0):
			for e in self.straturi_perceptroni[k]:
				e.backprop_perceptron(self.straturi[k-1],expected)


	def train_perceptrons(self, nr_iterations, train_images_list, train_labels_list):
		for iteration in range(nr_iterations):
			for i in range((len(train_images_list))):
				for j in range(self.output_size):
					expected = 1
					if j != train_labels_list[i]:
						expected = 0
					for k in range (4):
						self.compile_img(train_images_list[i],k)
					self.back_prop(expected)
					'''
					expected = 1
					if j != train_labels_list[i]:
						expected = 0
					self.straturi_perceptroni[4].backprop_perceptron(train_images_list[i], expected)
					'''
					self.straturi.clear()
					print (i)
			img_label_pairs = zip(train_images_list, train_labels_list)
			train_images_list, train_labels_list = zip(*img_label_pairs)

			print("Finished iteration {}".format(iteration + 1))


	def guess_number(self, pixel_list):
		best_index = 0
		max_LC = -1

		for i in range(self.output_size):
			for k in range(4):
				self.compile_img(pixel_list, k)
			#result = self.perceptron_list[i].functie_activare(pixel_list)
			result = max(self.straturi[3])
			if result > max_LC:
				max_LC = result
				best_index = i

		return best_index

	def test_network(self, test_images_list, test_labels_list):
		correctly_guessed = 0
		guessed_by_number = [0 for i in range(10)]
		label_nr = [0 for i in range(10)]

		for i in range(len(test_images_list)):
			label_nr[test_labels_list[i]] = label_nr[test_labels_list[i]] + 1
			result = self.guess_number(test_images_list[i])

			if result == test_labels_list[i]:
				correctly_guessed += 1
				guessed_by_number[result] += 1

		print("Total numbers detected: %d/%d" % (correctly_guessed, 10000))

		for i in range(10):
			print("#%d: %d/%d" % (i, guessed_by_number[i], label_nr[i]))
def run_tema1():
	network = PerceptronNetwork(10, 28 * 28, 0.005)

	with open('mnist.pkl//mnist.pkl', 'rb') as f:
		train_set, valid_set, test_set = cPickle.load(f, encoding='iso-8859-1')
	train_x, train_y = train_set

	train_img, train_label = train_set
	test_img, test_label = test_set

	
	network.train_perceptrons(1, train_img, train_label)
	network.test_network(test_img, test_label)


run_tema1()
			
