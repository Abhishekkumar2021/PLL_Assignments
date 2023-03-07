#include<bits/stdc++.h>
using namespace std;

/*
    LGB Algorithm - Vector quantization algorithm (Mostly like K-means)

*/

class LGB{
    vector<vector<double>> codebook;        // Initial codebook
    vector<vector<double>> data;            // Data to be quantized
    vector<vector<double>> new_codebook;    // New codebook after each iteration
    vector<int> cluster;                    // Cluster of each data
    vector<double> distortion;              // Distortion of each data
    vector<int> cluster_size;               // Size of each cluster
    vector<vector<double>> cluster_data;    // Data in each cluster
    int dim;                                // Dimension of data
    int n;                                  // Number of data
    int m;                                  // Number of elements in codebook
    int max_iter;                           // Maximum number of iterations
    double total_distortion;                // Total distortion

    public:
    LGB(vector<vector<double>> data, int m, int max_iter = 100){
        this->data = data;
        this->m = m;
        this->max_iter = max_iter;
        this->dim = data[0].size();
        this->n = data.size();
        this->total_distortion = 0;
        this->codebook.resize(2*m, vector<double>(dim, 0));
        this->new_codebook.resize(2*m, vector<double>(dim, 0));
        this->cluster.resize(n, 0);
        this->distortion.resize(n, 0);
        this->cluster_size.resize(2*m, 0);
        this->cluster_data.resize(2*m, vector<double>(dim, 0));
        initialize_codebook();
    }

    void initialize_codebook(){
        
        // Initialize codebook
        srand(time(0));
        for(int i = 0; i < m; i++){
            int idx = rand() % n;
            codebook[i] = data[idx];
        }

        // Divide each codewrod into two parts & replace with two new codewords with one common part
        vector<vector<double>> temp_codebook;
        for(int i = 0; i < m; i++){
            int half = dim / 2;
            vector<double> temp1, temp2;
            double random = (rand() % 20 - 10)/5.0;
            for(int j = 0; j < half; j++){
                temp1.push_back(codebook[i][j]);
                temp2.push_back(codebook[i][j] + random);
            }
            for(int j = half; j < dim; j++){
                temp1.push_back(codebook[i][j] + random);
                temp2.push_back(codebook[i][j]);
            }
            temp_codebook.push_back(temp1);
            temp_codebook.push_back(temp2);
        }

        // Replace codebook with new codebook
        codebook = temp_codebook;
        m *= 2 ;
    }
    
    // Run Algorithm
    void run(){
        int iter = 0;
        while(iter < max_iter){
            // Assign each data to a cluster
            assign_cluster();

            // Calculate distortion of each data
            distortion = get_data_distortion();

            // Calculate total distortion
            total_distortion = get_total_distortion();
            cout << "Iteration: " << iter << " Total distortion: " << total_distortion << endl;

            // Calculate new codebook
            for(int i = 0; i < n; i++){
                cluster_size[cluster[i]]++;
                for(int j = 0; j < dim; j++){
                    cluster_data[cluster[i]][j] += data[i][j];
                }
            }

            // Calculate new codebook
            for(int i = 0; i < m; i++){
                for(int j = 0; j < dim; j++){
                    if(cluster_size[i] != 0)
                        new_codebook[i][j] = cluster_data[i][j] / cluster_size[i];
                }
            }

            // Check if codebook has converged
            if(converged()){
                cout << "Hurray! Converged 😊" << endl;
                break;
            }

            // Replace codebook with new codebook
            codebook = new_codebook;

            // Clear cluster data
            cluster_data.assign(m, vector<double>(dim, 0));
            cluster_size.assign(m, 0);
            iter++;
        }
        print_codebook();
    }

    // Check if codebook has converged
    bool converged(){
        for(int i = 0; i < m; i++){
            for(int j = 0; j < dim; j++){
                if(codebook[i][j] != new_codebook[i][j]) return false;
            }
        }
        return true;
    }

    // Assign each data to a cluster
    void assign_cluster(){
        for(int i = 0; i < n; i++){
            double min_dist = numeric_limits<double>::max();
            int min_idx = 0;
            for(int j = 0; j < m; j++){
                double temp_dist = 0;
                for(int k = 0; k < dim; k++){
                    temp_dist += (data[i][k] - codebook[j][k]) * (data[i][k] - codebook[j][k]);
                }
                if(temp_dist < min_dist){
                    min_dist = temp_dist;
                    min_idx = j;
                }
            }
            cluster[i] = min_idx;
        }
    }

    // Calculate distortion
    double get_total_distortion(){
        double total_distortion = 0;
        for(int i = 0; i < n; i++){
            total_distortion += distortion[i];
        }
        return total_distortion;
    }

    // Calculate distortion of each data
    vector<double> get_data_distortion(){
        vector<double> distortion(n, 0);
        for(int i = 0; i < n; i++){
            double temp_distortion = 0;
            for(int j = 0; j < dim; j++){
                temp_distortion += (data[i][j] - codebook[cluster[i]][j]) * (data[i][j] - codebook[cluster[i]][j]);
            }
            distortion[i] = temp_distortion;
        }
        return distortion;
    }

    // Print codebook
    void print_codebook(){
        cout << "Codebook: " << endl;
        for(auto x: codebook){
            for(auto y: x){
                cout << y << " ";
            }
            cout << endl;
        }
    }

    // Print cluster
    void print_cluster(){
        for(int i=0; i<m; i++){
            cout << "Cluster " << i << ": ";
            for(int j=0; j<n; j++){
                if(cluster[j] == i) cout << j << " ";
            }
            cout << endl;
        }
    }
};

class DataGenerator{
    int n;
    int dim;
    int m;
    public:
    DataGenerator(int n, int dim, int m){
        this->n = n;
        this->dim = dim;
        this->m = m;
    }
    vector<vector<double>> generate_data(){
        vector<vector<double>> data;
        srand(time(0));
        for(int i = 0; i < n; i++){
            vector<double> temp;
            for(int j = 0; j < dim; j++){
                temp.push_back(rand() % 100);
            }
            data.push_back(temp);
        }
        return data;
    }
};

int main(){
    vector<vector<double>> data = DataGenerator(1000, 2, 10).generate_data();
    LGB lgb(data, 10, 100);
    lgb.run();
}
