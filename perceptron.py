import numpy as np
import pickle as cPickle
import gzip


class Perceptron:
	def __init__(self, input_size,index,rata_invatare):
		self.weights_list = np.random.normal(0, 1/np.sqrt(input_size),  input_size)
		self.input_size = input_size
		self.rata_invatare=rata_invatare
		self.bias = 0.5
		self.index = index

		
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

			
