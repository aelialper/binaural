
# import aifc
# import math
# import struct
# from __future__ import division
# from pylab import plot,show,subplot

# sampleRate = 44100;
# SAMPLE_LEN = sampleRate;

# filename = 'binaural_output'

# binaural_output = aifc.open( filename, 'w' )
# binaural_output.setparams(( 2, 2, sampleRate, 0, 'NONE', 'not compressed' ))

# amplitude = pow( 2, 16 );

# targetFrequencyLeft = 20;
# targetFrequencyRight = 100;
# incrementLeft = targetFrequencyLeft / sampleRate
# incrementRight =  targetFrequencyRight / sampleRate

# valuesLeft = []
# valuesRight = []
# for i in range(0, SAMPLE_LEN):
#         value = math.sin( i * incrementLeft ) * amplitude
#         # packed_value = struct.pack( 'h', value )
#         # valuesLeft.append( packed_value )
#         valuesLeft.append( value )
#         value = math.sin( i * incrementRight ) * amplitude
#         # packed_value = struct.pack( 'h', value )
#         # valuesRight.append( packed_value )
#         valuesRight.append( value )

# ## this is something I took form the wav file write example
# # value_str_Right = ''.join(valuesRight)
# # value_str_Left = ''.join(valuesLeft)

# ## this is something i took from stackexchange
# # for s, t in zip(valuesRight, valuesLeft):
# # 	binaural_output.writeframes(s)
# # 	binaural_output.writeframes(t)

# ## You need to close the writer
# # binaural_output.close()

# plot(valuesLeft,valuesRight)
# show()

from scipy.io.wavfile import write
from numpy import sin,pi,linspace
from pylab import plot,show,subplot, imshow
import numpy as np
import random


# def Increment( state, newState, )

def MarkovChain( state, transitionMatrix ):
	randomNumber = random.uniform(0,1)
	transitionProbabilities = np.cumsum(transitionMatrix[state])
	newState = np.where(transitionProbabilities > randomNumber)[0][0] # first occuranve where the random number larger is the transition
	return newState

transitionFrequency = [ [0.5, 0.25, 0.25], [0.5, 0.0, 0.5], [0.25, 0.25, 0.5] ]
transitionSpeed = [ [0.5, 0.25, 0.25], [0.5, 0.0, 0.5], [0.25, 0.25, 0.5] ] # the speed of transition from state to new state

size = 44100
ratio = [1.1, 1.11, 1.12]

delta = pi/2
t = linspace(-pi, pi, size )

state = 0

leftChannel = []
rightChannel = []
amplitude = pow( 2, 0 );


imageArray = np.zeros((size, size))
for i in range(0,1):

	newState = MarkovChain( state, transitionFrequency )
	if (newState != state):
		state = newState
		a = 600
		b = a * ratio[state]
	
	x = sin(a * t + delta)
	y = sin(b * t)
	leftChannel.extend(x * amplitude)
	rightChannel.extend(y * amplitude)

	for j in range(0, size):
		xCoordinate = x[j] * size/2 + size/2 - 1
		yCoordinate = y[j] * size/2 + size/2 - 1
		imageArray[ xCoordinate, yCoordinate] += 1


!cd ~/Projects/binaural_beats/
scaled = np.asarray(np.int16(leftChannel/np.max(np.abs(leftChannel)) * 32767))
write('test.wav', 44100, scaled)
!play test.wav

# plot(x,y)
imshow(imageArray, interpolation='none')
show()




