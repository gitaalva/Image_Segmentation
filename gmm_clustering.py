from __future__ import division
import numpy as np
import cv2
from sklearn.cluster import KMeans
from scipy.spatial import distance
import os
import time
import itertools
import numpy as np
from scipy import linalg
import matplotlib.pyplot as plt
import matplotlib as mpl
from sklearn import mixture
import csv
import sys

def getData(imageFile,n):
	#path = os.path.dirname(os.path.abspath(__file__))
	#imageFile = path + "/piechart.thumbnail"
	#imageFile = "/home/aditya/image_seg_final/piechart.thumbnail"
	img = cv2.imread(imageFile)
	newimg = img.reshape(-1,3)

	# Fit a mixture of Gaussians with EM using two components
	gmm = mixture.GMM(n_components=n,  random_state = None, covariance_type='tied', n_init = 10)

	t0 = time.time()

	gmm.fit(newimg)

	t1 = time.time()

	y_pred = gmm.predict(newimg)

	a = gmm.predict_proba(newimg)
	#print gmm.score_samples(newimg)

	with open ("output.txt", "w") as f:
		for data in range(0,len(a)):
			#if len(a[data]) <= 2:
			#	f.write(str(int(round(a[data][0]*10)))+"\n")
			#else:
			line = ""
	    	        for i in range(0,len(a[data])):
		                a[data][i] = round(a[data][i]*10)
			        if i < len(a[data])-1:
			                line += str(int(a[data][i])) + " "
			        else:
					line += str(int(a[data][i])) + "\n"
			f.write(line)
	f.close()

args = sys.argv
#path = os.path.dirname(os.path.abspath(__file__)) + "/gmm_clustering.py"
getData(args[1],int(args[2]))
