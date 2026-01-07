#include <mlpack/methods/random_forest/random_forest.hpp>
#include <mlpack/methods/softmax_regression/softmax_regression.hpp>

using namespace mlpack;

int main()
{
    // load dataset
    arma::mat dataset;
    data::Load("covtype.csv", dataset, true); // fatal on failure

    // strip labels out of dataset into own object and adjust range to 0-6
    arma::Row<size_t> labels = arma::conv_to<arma::Row<size_t>>::from(
        dataset.row(dataset.n_rows - 1)) -1;

    dataset.shed_row(dataset.n_rows - 1); // remove labels from ds

    // split dataset into training set and test set
    arma::mat trainData, testData;
    arma::Row<size_t> trainLabels, testLabels;
    data::Split(dataset, labels, trainData, testData, trainLabels, testLabels, 0.2 /*20 percent*/);

    // print the sizes of the data train/test set
    std::cout << "Training set has " << trainData.n_cols << " points" << std::endl;
    std::cout << "Test set has " << testData.n_cols << " points" << std::endl;

    // train model
    RandomForest rf(trainData, trainLabels, 7); // num of classes randomforest

    // eval performance of the model on both sets
    arma::Row<size_t> trainPredictions, testPredictions;
    rf.Classify(trainData, trainPredictions);
    rf.Classify(testData, testPredictions);

    double trainAccuracy  = 100.0 * arma::accu(trainPredictions == trainLabels) /
        static_cast<double>(trainLabels.n_elem);

    double testAccuracy = 100.0 * arma::accu(testPredictions == testLabels) /
        static_cast<double>(testLabels.n_elem);

    std::cout << "-------------RANDOM FOREST------------------" << std::endl;
    std::cout << "Training set accuracy: " << trainAccuracy << "%" << std::endl;
    std::cout << "Test set accuracy: " << testAccuracy << "%" << std::endl;

    SoftmaxRegression sr(trainData, trainLabels, 7); // num of classes soft max regression

    arma::Row<size_t> trainPredictionsSR, testPredictionsSR;
    sr.Classify(trainData, trainPredictions);
    sr.Classify(testData, testPredictions);

    double trainAccuracySR = 100.0 * arma::accu(trainPredictions == trainLabels) /
        static_cast<double>(trainLabels.n_elem);

    double testAccuracySR = 100.0 * arma::accu(testPredictions == testLabels) /
        static_cast<double>(testLabels.n_elem);

    std::cout << "-------------Soft Max Regression-----------------" << std::endl;
    std::cout << "Training set accuracy: " << trainAccuracySR << "%" << std::endl;
    std::cout << "Test set accuracy: " << testAccuracySR <<  "%" << std::endl;
    // random forest is better in this case as sm regression isnt optimized, maybe needs gradient desc

}
