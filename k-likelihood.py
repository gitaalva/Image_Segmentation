from __future__ import division
import numpy as np
import cv2
import sys
from sklearn.cluster import KMeans
from scipy.spatial import distance

def getLikelihood(imageFile,k):
    img = cv2.imread(imageFile)
    newimg = img.reshape(-1,3)
    kmeans = KMeans(init='k-means++', n_clusters=k, n_init=100)
    kmeans.fit_predict(newimg)
    error = kmeans.inertia_
    centroids = kmeans.cluster_centers_
    #print centroids
    labels = kmeans.labels_
    likelihoodList = []
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
			likelihood = []
			dist_sum = 0
			for k in range(0,len(centroids)):
				dist = distance.euclidean(img[i][j],centroids[k])
				dist_sum += dist
				likelihood.append(dist)
			for w in range(0,len(likelihood)):
				norm_dist = int(round(likelihood[w]/dist_sum*10))
				likelihood[w] = norm_dist
			likelihoodList.append(str(likelihood).translate(None, '[],'))
    return likelihoodList,len(likelihoodList)

def getData(imageFile,k):
	f = open('output.txt', 'w')	
	dataList,dataSize = getLikelihood(imageFile,k)
	for data in range(0,dataSize):
		f.write(str(dataList[data])+"\n")
	f.close()
	print dataSize

args = sys.argv
getData(args[1],int(args[2]))
        
#getData("rgb.thumbnail",2)


