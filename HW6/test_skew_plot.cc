#include <boost/math/distributions/skew_normal.hpp>

#include <vector>
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>


int main(){


	// Setup generators
	std::random_device rd;
	std::default_random_engine noise_generator;

	// Sample from a uniform distribution i.e. [0,1)
	std::uniform_real_distribution<double> uniform_dist(0,1.0);

	// Take a different value every time to generate probabilities from 0 to 1
	noise_generator.seed(rd());
	auto probability = uniform_dist(noise_generator);

	auto skew_norm_dist = boost::math::skew_normal_distribution<double>(
	.15, 2., 10.);

	// Use the probability from the uniform distribution with the percent point
	// function of the skew_normal
	double skew_normal_sample_point = boost::math::quantile(skew_norm_dist, probability);
	std::cout << "Sample point: " << skew_normal_sample_point << std::endl;



	


	std::vector<int> count(5000,0);



	//generate the data

	for (int i = 0; i < 100000; i++)
	{

		auto probability = uniform_dist(noise_generator);
		double a_number = boost::math::quantile(skew_norm_dist, probability);
		


		uint a_number_bxn_1_and_5k = std::abs(a_number * 500);

		//std::cout <<"here is a plot point: "<<a_number_bxn_1_and_5k<<std::endl;
		count[a_number_bxn_1_and_5k] ++;


		std::cout<<"we are at number "<<i<<std::endl;
	}


	std::ofstream debug_file_stream;

	debug_file_stream.open("distribution_of_value_sizes.txt");

	for (int i = 0; i < 5000; i++)
	{
		debug_file_stream << i<<"\t" <<count[i] <<std::endl;

	}
	debug_file_stream.close();

}