#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

// Generate random data points
std::vector<std::vector<double> > generateData(int numPoints, int numDimensions) {
    std::vector<std::vector<double> > data;
    data.reserve(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        std::vector<double> point;
        point.reserve(numDimensions);
        for (int j = 0; j < numDimensions; ++j) {
            point.push_back(static_cast<double>(std::rand()) / RAND_MAX);
        }
        data.push_back(point);
    }
    return data;
}

// Compute Euclidean distance between two points
double euclideanDistance(const std::vector<double>& p1, const std::vector<double>& p2) {
    double sum = 0.0;
    for (size_t i = 0; i < p1.size(); ++i) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return std::sqrt(sum);
}

// Perform K-Means Clustering
void kMeansClustering(const std::vector<std::vector<double> >& data, int numClusters, int numIterations) {
    const int numPoints = data.size();
    const int numDimensions = data[0].size();

    std::vector<std::vector<double> > centroids(numClusters, std::vector<double>(numDimensions));

    // Initialize centroids randomly
    for (int i = 0; i < numClusters; ++i) {
        centroids[i] = data[std::rand() % numPoints];
    }

    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::vector<int> clusterCounts(numClusters, 0);
        std::vector<std::vector<double> > newCentroids(numClusters, std::vector<double>(numDimensions));

        // Assignment step
        for (int i = 0; i < numPoints; ++i) {
            double minDistance = std::numeric_limits<double>::max();
            int closestCluster = 0;
            for (int j = 0; j < numClusters; ++j) {
                double distance = euclideanDistance(data[i], centroids[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestCluster = j;
                }
            }
            clusterCounts[closestCluster]++;
            for (int d = 0; d < numDimensions; ++d) {
                newCentroids[closestCluster][d] += data[i][d];
            }
        }

        // Update centroids
        for (int i = 0; i < numClusters; ++i) {
            for (int d = 0; d < numDimensions; ++d) {
                centroids[i][d] = newCentroids[i][d] / clusterCounts[i];
            }
        }
    }
}

int main() {
    const int numPoints = 1000;
    const int numDimensions = 2;
    const int numClusters = 5;
    const int numIterations = 10;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::vector<double> > data = generateData(numPoints, numDimensions);

    kMeansClustering(data, numClusters, numIterations);

    return 0;
}