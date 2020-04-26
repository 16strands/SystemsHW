#include "cache.hh"

#include <iostream>
#include <cstring>
#include <string>
#include <cassert>
#include <boost/program_options.hpp>
#include <vector>
#include <chrono>
#include <boost/math/distributions/skew_normal.hpp>
#include <random>
#include <fstream>
#include <algorithm>


bool DEBUG = false;

namespace po = boost::program_options;

bool FORCE_SET_NUM_KEYS = true;
int NUM_KEYS = 20'700;

//calculate how many unique queries there are 
//this can be changed to look at performance of stuff
// 5 min bins is 20.7% and 60 min bin is 44.5%
double ratio_of_keys_to_requests = 0.207; // for every key, there are 2 requests
//TO DO! find out what real ratio of keys to requests is

double frequency_of_gets = .66;
double frequency_of_dels = 0.05;
double frequency_of_updates = 0.29;



//this is all global???????????????????
// Setup generators
std::random_device rd;

std::default_random_engine noise_generator;

// Sample from a uniform distribution i.e. [0,1)
std::uniform_real_distribution<double> uniform_dist(0,1.0);

auto skew_norm_dist = boost::math::skew_normal_distribution<double>(
.15, 2., 10.);







//this should generate a random value based on a distribution that looks like this:
//   _,  
//  /  ''=..
// /        ''-......
//1    200         5000   
char* get_value(int& size){

    //generate a random number on the distribution thing
    //then we gotta make a char* that's that long!

    // Take a different value every time to generate probabilities from 0 to 1
    noise_generator.seed(rd());
    auto probability = uniform_dist(noise_generator);
    double a_number = boost::math::quantile(skew_norm_dist, probability);

    //uint is the length of the char*, the value
    int a_number_bxn_1_and_5k = std::abs(a_number * 500);

    //std::cout<<"length of val is "<< a_number_bxn_1_and_5k<<std::endl;

    std::string value = "";

    for (int i =0; i< a_number_bxn_1_and_5k; i ++)
    {
        value = value +"a";
    }

    size = a_number_bxn_1_and_5k + 1;

    return strdup(value.c_str());

}




Cache* makeWarmCache(std::string host, std::string port, int num_requests)
{
    
    int num_keys = ratio_of_keys_to_requests * num_requests;
    if (FORCE_SET_NUM_KEYS) num_keys = NUM_KEYS;

    Cache* my_cache = new Cache(host, port);
    for (int i = num_keys-1; i >= 0; i--)
    {
        //make keys
        std::string key = "key" + std::to_string(i);

        if (DEBUG) std::cout <<"adding key "<<key <<" to cache"<<std::endl;

        int size_of_val = 0;

        auto value  = get_value(size_of_val);
        
        if (DEBUG) std::cout <<"the val is "<<value <<std::endl;

        my_cache->set(key, value, size_of_val);
        //do we need to delete value here? TO DO 
        //EITAN HELP!
    }
    return my_cache;
}

//this randomly updates key 
std::vector<std::string> random_update_requests(int num_update_requests, int num_kv_pairs)
{
    std::vector<std::string> set_requests;


    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> random_key(0,num_kv_pairs);
    //for however many set requests there, are, it'll update a random key
    for (int i = 0; i < num_update_requests; i++)
    {
        set_requests.push_back(std::to_string(random_key(generator)));
    }
    return set_requests;
}


//this generates deletes, It doesn't take into account deleting the same 
//object twice, because that should probably be handled
std::vector<std::string> random_del_requests(int num_del_requests, int num_kv_pairs)
{
    std::vector<std::string> del_requests;

    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> random_key(0,num_kv_pairs);
    //for however many set requests there, are, it'll update a random key
    for (int i = 0; i < num_del_requests; i++)
    {
        del_requests.push_back(std::to_string(random_key(generator)));
    }
    return del_requests;
}


//given a number of get requests to generate, this generates get requests 
// 10% of unique keys make up 90% of total requests
// 40% of unique keys make up 9% of total requests
// 50% of unique keys make up 1% of total requests
std::vector<std::string> random_get_requests(int num_get_requests, int num_kv_pairs)
{
    // taking into account reuse of keys:
    // key number is indicative of how popular it is, so key_1 is most popular, and key_num_requests is least popular

    //the way this will work is that the lowest 10% of keys (since every key is a number, 
    // there can be low ones) will correspond to the most popular queries. Same with the
    // next 40% and the next 50%.

    int place_marker_for_first_10_percent = (static_cast<double>(num_kv_pairs) *.1);

    int place_marker_for_second_40_percent =(static_cast<double>(num_kv_pairs) *.5); //the math works out here cuz subtraction, u feel me?

    //the most popular keys are keys with numbers less than place_marker_for_first_10_percent
    //the next most popular keys (which are 40% of all unique keys) are keys bigger than the 
    // 10th %ile and less than the place_marker_for_second_40_percent
    //finally, the least popular are keys greater than place_maerker_for_second_40_percent

    //now we iterate through and create new get requests. to do this we generate a 
    // random number 0-99. if the number is 0-89, then the get request is for a random key
    // within 0 and place_marker_for_first_10_percent


    //all the random number gens:
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution_of_request(0,99);
    std::uniform_int_distribution<int> random_common_key(0,place_marker_for_first_10_percent);
    std::uniform_int_distribution<int> random_uncommon_key(place_marker_for_first_10_percent+1,place_marker_for_second_40_percent);
    std::uniform_int_distribution<int> random_rare_key(place_marker_for_second_40_percent+1,num_kv_pairs);

    //finally, here is the list of get requests
    std::vector<std::string> get_requests;


    //what about compound keys?
    // this is for you to think about, grader.
    for(int i = 0; i < num_get_requests; i++) 
    {

        //how this works is a random number is generated.
        int key_commonness = distribution_of_request(generator);

        if (key_commonness <= 89){
            get_requests.push_back(std::to_string(random_common_key(generator)));
        }
        else if(key_commonness <= 98){
            get_requests.push_back(std::to_string(random_uncommon_key(generator)));
        }
        else {
            get_requests.push_back(std::to_string(random_rare_key(generator)));
        }
    }
    return get_requests;

}

std::vector<std::string> list_of_random_values(int num_values){
    std::vector<std::string> list_of_values;

    int size = 0;

    for(int i = 0; i < num_values; i++)
    {
        std::string val (get_value(size));
        list_of_values.push_back(val);
    }
    return list_of_values;
}


//make a list of requests
//generate request


// we have n requests            v this is interchangeable with updates
// out of those n requests, like 28% are deletes, 1/30th are updates, get is 2/3
// value sizes are mostly 1-5k bytes, avg is 150 bytes, distribute things like that
// key reuse:
//     50% of keys are almost single use, they're used in 1% of requests
//     the next 40% of keys are used in 9% of requests
//     the top 10% of keys are used in 90%  of requests



float timed_gets(Cache* my_cache,
            int num_requests)
{


    int num_kv_pairs = num_requests * ratio_of_keys_to_requests;
    if (FORCE_SET_NUM_KEYS) num_kv_pairs = NUM_KEYS;

    std::vector<std::string> get_requests = random_get_requests(num_requests,num_kv_pairs);


    //an example get request:
    //Cache::size_type size_of_val = 0;
    //auto value = (my_cache->get(get_requests[i], size_of_val));

    //we need to pass this as an arg to my_cache->get
    Cache::size_type size_of_val = 0;

    std::vector<int> count(num_kv_pairs,0);

    double num_hits = 0;

    //we did this in the first hw...........
    auto startTime = std::chrono::high_resolution_clock::now();
    


    for(int i = 0; i < num_requests; i++){
        auto key = "key"+get_requests[i];
        auto value = (my_cache->get(key, size_of_val)); 
        if(value) num_hits++;
        delete value;
//        count[std::stoi(get_requests[i])] ++;
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    double hit_rate = num_hits/num_requests;

    std::cout<<"hit rate is "<<hit_rate<<std::endl;

    if (DEBUG)std::cout<<"num vals is" <<num_kv_pairs<<std::endl;
    
    std::ofstream debug_file_stream;

    debug_file_stream.open("keys_and_how_frequently_they_are_getted.txt");

    for (int i = 0; i < num_kv_pairs; i++)
    {
        if (DEBUG) std::cout<<i<<"\t" <<count[i] <<std::endl;
        debug_file_stream << i<<"\t" <<count[i] <<std::endl;

    }
    debug_file_stream.close();

    float t = std::chrono::duration_cast<std::chrono::milliseconds>( endTime - startTime ).count();
    return t;
}

float timed_updates(Cache* my_cache,
            int num_requests)
{

    int num_kv_pairs = num_requests * ratio_of_keys_to_requests;
    if (FORCE_SET_NUM_KEYS) num_kv_pairs = NUM_KEYS;

    std::vector<std::string> update_requests = random_update_requests(num_requests,num_kv_pairs);



    //we did this in the first hw...........
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < num_requests; i++){
        int size;
        auto key = "key"+update_requests[i];
        auto value = (get_value(size)); 
        my_cache->set(key, value, size);
        delete value;
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    float t = std::chrono::duration_cast<std::chrono::milliseconds>( endTime - startTime ).count();
    return t;
}

float timed_dels(std::string host, std::string port,
            int num_requests)
{

    Cache* my_cache = makeWarmCache(host, port, num_requests);
    int num_kv_pairs = num_requests * ratio_of_keys_to_requests;
    if (FORCE_SET_NUM_KEYS) num_kv_pairs = NUM_KEYS;

    std::vector<std::string> del_requests = random_del_requests(num_requests,num_kv_pairs);



    //we did this in the first hw...........
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < num_requests; i++){
        auto key = "key"+del_requests[i];
        my_cache->del(key);
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    delete my_cache;
    float t = std::chrono::duration_cast<std::chrono::milliseconds>( endTime - startTime ).count();
    return t;
}



//this takes an nreq (number requests) and outputs information about mode, average, 
// quartiles, for time latencies for gets, updates, and dels!
void baseline_latency (std::string host, 
            std::string port, 
            int num_requests)
{
    Cache* my_cache = makeWarmCache(host, port, num_requests);
    int num_trials = 5;

    float min =999999;
    float max =0;
    float avg = 0;
    for (int i = 0; i < num_trials; i++)
    {
        float time = timed_gets(my_cache, num_requests);

        if (time < min) min = time;
        if (time > max) max = time;
        avg+=time;


    }
    avg = avg/ static_cast<float>(num_trials);


    std::ofstream debug_file_stream;

    debug_file_stream.open("times.txt");

        
    debug_file_stream << 1<<"\t" <<min <<std::endl;
    debug_file_stream << 2<<"\t" <<avg <<std::endl;
    debug_file_stream << 3<<"\t" <<max <<std::endl;

    

//now for updates
    min =999999;
    max =0;
    avg = 0;
    for (int i = 0; i < num_trials; i++)
    {
        float time = timed_updates(my_cache,num_requests);

        if (time < min) min = time;
        if (time > max) max = time;
        avg+=time;
    }
    avg = avg/ static_cast<float>(num_trials);

    debug_file_stream << 4<<"\t" <<min <<std::endl;
    debug_file_stream << 5<<"\t" <<avg <<std::endl;
    debug_file_stream << 6<<"\t" <<max <<std::endl;


//now dels
    min =999999;
    max =0;
    avg = 0;
    for (int i = 0; i < num_trials; i++)
    {
        float time = timed_dels(host, port,num_requests);

        if (time < min) min = time;
        if (time > max) max = time;
        avg+=time;
    }
    avg = avg/ static_cast<float>(num_trials);

    debug_file_stream << 7<<"\t" <<min <<std::endl;
    debug_file_stream << 8<<"\t" <<avg <<std::endl;
    debug_file_stream << 9<<"\t" <<max <<std::endl;



    debug_file_stream.close();

    delete my_cache;

    std::cout <<"done!"<<std::endl;

}

// Run all tests
// Change to true for debugging print statements
int main(int argc, char** argv)
{
    
    // Check command line arguments.
    std::string server_ip;
    std::string port;
    int nreq;

    // optional command line arguments with flags and default values
    // got this logic from https://stackoverflow.com/questions/11280136/optional-command-line-arguments
    po::options_description desc("Options for my program");
    desc.add_options()
        // Option 'server' and 's' are equivalent.
        ("server_ip,s", po::value<std::string>(& server_ip)->default_value("127.0.0.1"),
         "server address")
        // Option 'port' and 'p' are equivalent.
        ("port,p", po::value<std::string>(& port)->default_value("2020"),
         "port number")
        // Option nreq
        // Option 'port' and 'p' are equivalent.
        ("nreq,r", po::value<int>(& nreq)->default_value(100000),
         "number requests")

        //frequency of updates TO DO
        //("u_freq", po::value<std::string()>)
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);



    //std::cout<<timed_gets(server_ip, port,  num_requests);

    baseline_latency(server_ip, port, nreq);
    return 0;

}
