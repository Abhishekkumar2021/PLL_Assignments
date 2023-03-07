#include<bits/stdc++.h>
using namespace std;

class KMeans
{
    private:
        int n_clusters;     // Number of clusters
        int max_iter;       // Maximum number of iterations
        int n_smaples;      // Number of samples
        int n_features;     // Number of features
        vector<vector<double>> data;    // Data (Samples)
        vector<vector<double>> centroids;   // Centroids (Cluster Centers)
        vector<vector<double>> old_centroids;   // Old Centroids (Cluster Centers)
        vector<double> distances;       // Distances of each sample to each centroid
        vector<int> cluster_labels;     // Cluster Labels (0, 1, 2, ...)
        vector<int> cluster_sizes;      // Cluster Sizes (Number of samples in each cluster)
        vector<double> cluster_inertia; // Cluster Inertia (Sum of squared distances of samples to their closest cluster center.)
        vector<vector<double>> cluster_data;   // Cluster Data (Sum of all samples in each cluster for each feature to calculate mean)
        bool isFirst = true;
    
    public:
    
    KMeans(int n_clusters = 2, int max_iter = 300)
    {
        this->n_clusters = n_clusters;
        this->max_iter = max_iter;
    }

    void fit(vector<vector<double>> data)
    {
        this->data = data;
        this->n_smaples = data.size();
        this->n_features = data[0].size();
        this->cluster_labels.resize(n_smaples, 0);
        this->cluster_sizes.resize(n_clusters, 0);
        this->cluster_inertia.resize(n_clusters, 0);
        this->cluster_data.resize(n_clusters, vector<double>(n_features, 0));
        this->distances.resize(n_clusters, 0);
        this->centroids.resize(n_clusters, vector<double>(n_features, 0));
        this->old_centroids.resize(n_clusters, vector<double>(n_features, 0));
        if(isFirst)
        {
            this->initialize_centroids();
            isFirst = false;
        }
        this->assign_clusters();
        this->update_centroids();
        int iter = 0;
        while(!this->converged() && iter < max_iter)
        {
            this->cluster_sizes.assign(n_clusters, 0);
            this->cluster_inertia.assign(n_clusters, 0);
            for(int i = 0; i < n_clusters; i++)
            {
                this->cluster_data[i].assign(n_features, 0);
            }
            this->assign_clusters();
            this->update_centroids();
            iter++;
        }
        get_cluster_inertia();
    }

    void initialize_centroids()
    {
        srand(time(0));
        for(int i = 0; i < n_clusters; i++)
        {
            int index = rand() % n_smaples;
            for(int j = 0; j < n_features; j++)
            {
                centroids[i][j] = data[index][j];
            }
        }
    }

    void assign_clusters()
    {
        for(int i = 0; i < n_smaples; i++)
        {
            for(int j = 0; j < n_clusters; j++)
            {
                distances[j] = this->euclidean_distance(data[i], centroids[j]);
            }
            int cluster = min_element(distances.begin(), distances.end()) - distances.begin();
            cluster_labels[i] = cluster;
            cluster_sizes[cluster]++;       // Increment cluster size
            for(int j = 0; j < n_features; j++)
            {
                cluster_data[cluster][j] += data[i][j];
            }
        }
    }

    void update_centroids()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            for(int j = 0; j < n_features; j++)
            {
                old_centroids[i][j] = centroids[i][j];
                centroids[i][j] = cluster_data[i][j] / cluster_sizes[i];
            }
        }
    }

    bool converged()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            for(int j = 0; j < n_features; j++)
            {
                if(centroids[i][j] != old_centroids[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    double euclidean_distance(vector<double> a, vector<double> b)
    {
        double distance = 0;
        for(int i = 0; i < a.size(); i++)
        {
            distance += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return sqrt(distance);
    }

    vector<int> get_labels()
    {
        return cluster_labels;
    }

    vector<vector<double>> get_centroids()
    {
        return centroids;
    }

    vector<int> get_cluster_sizes()
    {
        return cluster_sizes;
    }

    vector<double> get_cluster_inertia()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            for(int j = 0; j < n_smaples; j++)
            {
                if(cluster_labels[j] == i)
                {
                    cluster_inertia[i] += pow(this->euclidean_distance(data[j], centroids[i]), 2);
                }
            }
        }
        return cluster_inertia;
    }

    vector<vector<double>> get_cluster_data()
    {
        return cluster_data;
    }

    void set_centroids(vector<vector<double>> centroids)
    {
        this->centroids = centroids;
    }

    double get_inertia()
    {
        double inertia = 0;
        for(int i = 0; i < n_clusters; i++)
        {
            inertia += cluster_inertia[i];
        }
        return inertia;
    }

    void print_cluster_data()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            cout << "Cluster data of " << i << " : ";
            for(int j = 0; j < n_features; j++)
            {
                cout << cluster_data[i][j] << " ";
            }
            cout << endl;
        }
    }

    void print_cluster_sizes()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            cout << "Cluster size of " << i << " : " << cluster_sizes[i] << endl;
        }
    }

    void print_cluster_inertia()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            cout << "Cluster inertia of " << i << " : " << cluster_inertia[i] << endl;
        }
    }

    void print_cluster_labels()
    {
        for(int i = 0; i < n_smaples; i++)
        {
            cout << "Cluster Label of sample " << i << " : " << cluster_labels[i] << endl;
        }
    }

    void print_centroids()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            cout << "Centroid " << i << " : ";
            for(int j = 0; j < n_features; j++)
            {
                cout << centroids[i][j] << " ";
            }
            cout << endl;
        }
    }

    void print_data()
    {
        for(int i = 0; i < n_smaples; i++)
        {
            cout << "Sample " << i << " : {";
            for(int j = 0; j < n_features; j++)
            {
                if (j == n_features - 1)
                {
                    cout << data[i][j];
                }
                else{
                    cout << data[i][j] << ",";
                }
            }
            cout << "}" << endl;
        }
    }

   ~KMeans()
    {
        data.clear();
        cluster_labels.clear();
        cluster_sizes.clear();
        cluster_inertia.clear();
        cluster_data.clear();
        distances.clear();
        centroids.clear();
        old_centroids.clear();
    } 
};

class SampleDataGenerator
{
    private:
    int n_samples;
    int n_features;
    int n_clusters;
    vector<vector<double>> data;
    vector<vector<double>> centroids;
    vector<int> cluster_labels;

    public:
    SampleDataGenerator(int n_samples, int n_features, int n_clusters)
    {
        this->n_samples = n_samples;
        this->n_features = n_features;
        this->n_clusters = n_clusters;
        data.resize(n_samples, vector<double>(n_features));
        centroids.resize(n_clusters, vector<double>(n_features));
        cluster_labels.resize(n_samples);
    }

    vector<vector<double>> generate_data()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            for(int j = 0; j < n_features; j++)
            {
                centroids[i][j] = rand() % 200;
            }
        }

        // Normal distribution
        default_random_engine generator;
        normal_distribution<double> distribution(0.0, 3.0);
        for(int i = 0; i < n_samples; i++)
        {
            int cluster = rand() % n_clusters;
            // Get a point which is distance away from the centroid
            for(int j = 0; j < n_features; j++)
            {
                data[i][j] = centroids[cluster][j] + distribution(generator);
            }
            cluster_labels[i] = cluster;
        }
        return data;
    }

    vector<vector<double>> get_centroids()
    {
        return centroids;
    }

    vector<int> get_labels()
    {
        return cluster_labels;
    }

    ~SampleDataGenerator()
    {
        data.clear();
        centroids.clear();
    }
};

class KMeansClassifier{
    vector<vector<double>> data;
    vector<vector<double>> centroids;
    int max_iter;
    int n_clusters;
    double inertia;
    KMeans kmeans;

    public:
    KMeansClassifier(vector<vector<double>> data, int n_clusters = 2, int max_iter = 300)
    {
        this->n_clusters = n_clusters;
        this->max_iter = max_iter;
        this->data = data;
        // initialise inertia to max double value
        this->inertia = numeric_limits<double>::max();
        for(int i=0; i < max_iter; i++)
        {
            KMeans temp = KMeans(n_clusters, max_iter);
            temp.fit(data);
            double current_inertia = temp.get_inertia();
            if(current_inertia < inertia)
            {
                inertia = current_inertia;
                centroids = temp.get_centroids();
                this->kmeans = temp;
            }
        }
    }

    void fit()
    {
        kmeans.set_centroids(centroids);
        kmeans.fit(data);
    }


    KMeans get_kmeans()
    {
        return kmeans;
    }
};

class ElbowMethod{
    vector<vector<double>> data;
    int max_iter;
    int n_clusters;
    vector<double> inertia;

    public:
    ElbowMethod(vector<vector<double>> data, int max_iter = 300)
    {
        this->data = data;
        this->max_iter = max_iter;
        this->n_clusters = 10;
        this->inertia.resize(n_clusters);
    }

    void fit()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            KMeansClassifier kmc(data, i+1, max_iter);
            kmc.fit();
            inertia[i] = kmc.get_kmeans().get_inertia();
        }
    }

    void print_inertia()
    {
        for(int i = 0; i < n_clusters; i++)
        {
            cout << "Inertia for " << i+1 << " clusters : " << inertia[i] << endl;
        }
    }

    vector<double> get_inertia()
    {
        return inertia;
    }


};

class Saver{
    vector<int> int_data;
    vector<double> double_data;
    vector<vector<int>> int_2d_data;
    vector<vector<double>> double_2d_data;

    public:
    Saver(vector<int> int_data){
        this->int_data = int_data;
    }
    Saver(vector<double> double_data){
        this->double_data = double_data;
    }
    Saver(vector<vector<int>> int_2d_data){
        this->int_2d_data = int_2d_data;
    }
    Saver(vector<vector<double>> double_2d_data){
        this->double_2d_data = double_2d_data;
    }

    void save(string filename){
        ofstream file;
        file.open(filename);
        string column_name = filename.substr(0, filename.find("."));
        if(int_data.size() != 0){
            file << column_name << endl;
            for(int i = 0; i < int_data.size(); i++){
                if(i == int_data.size() - 1){
                    file << int_data[i];
                }
                else{
                    file << int_data[i] << endl;
                }
            }
        }
        else if(double_data.size() != 0){
            file << column_name << endl;
            for(int i = 0; i < double_data.size(); i++){
                if(i == double_data.size() - 1){
                    file << double_data[i];
                }
                else{
                    file << double_data[i] << endl;
                }
            }
        }
        else if(int_2d_data.size() != 0){
            for(int i=0; i<int_2d_data[0].size(); i++){
                if(i == int_2d_data[0].size() - 1)
                    file << column_name << i << endl;
                else
                    file << column_name << i << ",";
            }
            for(int i = 0; i < int_2d_data.size(); i++){
                for(int j = 0; j < int_2d_data[i].size(); j++){
                    if(j == int_2d_data[i].size() - 1){
                        if(i == int_2d_data.size() - 1){
                            file << int_2d_data[i][j];
                        }
                        else{
                            file << int_2d_data[i][j] << endl;
                        }
                    }
                    else{
                        file << int_2d_data[i][j] << ",";
                    }
                }
            }
        }
        else if(double_2d_data.size() != 0){
            for(int i=0; i<double_2d_data[0].size(); i++){
                if(i == double_2d_data[0].size() - 1)
                    file << column_name << "_" << i << endl;
                else
                    file << column_name << "_" << i << ",";
            }
            
            for(int i = 0; i < double_2d_data.size(); i++){
                for(int j = 0; j < double_2d_data[i].size(); j++){
                    if(j == double_2d_data[i].size() - 1){
                        if(i == double_2d_data.size() - 1){
                            file << double_2d_data[i][j];
                        }
                        else{
                            file << double_2d_data[i][j] << endl;
                        }
                    }
                    else{
                        file << double_2d_data[i][j] << ",";
                    }
                }
            }
        }
        file.close();
    }
    
};

int main()
{
    SampleDataGenerator sdg(100, 2, 3);
    vector<vector<double>> data = sdg.generate_data();

    // Save data to csv file
    Saver(data).save("data.csv");

    // Save true labels to csv file
    Saver(sdg.get_labels()).save("true_labels.csv");

    ElbowMethod em(data);
    em.fit();

    // Save inertia to csv file
    Saver(em.get_inertia()).save("inertia.csv");

    // Save generated labels to csv file
    KMeansClassifier kmc(data, 3);
    kmc.fit();
    Saver(kmc.get_kmeans().get_labels()).save("generated_labels.csv");

    return 0;
}      