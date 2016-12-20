#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include <vector>
#include "factorization.h"

struct triple {
  unsigned long a, b, c, sum;
};

std::vector< triple> * check(unsigned long candidate);

int count = 0, num_divisors = 0;
unsigned long gamma_bound = 0;
bool pure_only = false;

int main(int argc, char* argv[]) {
  std::string file_string = "output.txt";

  unsigned long lower_limit = 0, upper_limit = 0;
  bool show_progress_bar = false;

  std::string usage = "usage:\t" + std::string(argv[0]) + " [-p] [-g <bound>] lower-limit upper-limit\n"
  "\tsearch for pairs of triples of the form (a,b,b), (x,y,z)\n"
  "\twhere:\ta + b + b = x + y + z\n"
  "\t\ta + b + b and x + y + z are within the lower and upper limits\n"
  "\tp: only search for pure triple pairs, where x = y \n"
  "\tg: only search for triple pairs where a + a + b = x + y + z <= bound\n";

  if (argc < 3) {
    std::cout << argv[0] << ": not enough arguments" << std::endl;
    std::cout << usage;
    return -1;
  }

  int c;
  while ((c = getopt(argc, argv, "pg:")) != -1) {
    switch (c) {
      case 'p':
      pure_only = true;
      break;
      case 'g':
      if (atoi(optarg)) {
        gamma_bound = atoi(optarg);
      }
      else {
        std::cout << argv[0] << ": invalid gamma bound" << std::endl;
        std::cout << usage;
        return -1;
      }
      break;
      case ':':
      case '?':
      std::cout << usage;
      return -1;
      break;

    }
  }
  if (argc - optind >= 2) {
    lower_limit = atol(argv[optind]);
    upper_limit = atol(argv[optind + 1]);
  }
  if (argc > optind + 2) {
    std::cout << argv[0] << ": ignoring extra arguments:";
    for (int i = optind + 2; i < argc; i++) {
      std::cout << " " << argv[i];
    }
    std::cout << std::endl;
  }

  std::string searching_for = "fluffy numbers (";

  if (lower_limit > upper_limit) {
    std::cout << argv[0] << ": inconsistent limits" << std::endl;
    std::cout << usage;
    return -1;
  }
  if (pure_only) {
    searching_for += "pure";
    if (gamma_bound) searching_for += ", ";
  }
  if (gamma_bound) searching_for += "gamma{" + std::to_string(gamma_bound) + "}";
  if (searching_for.substr(searching_for.length() - 1) == "(") {
    searching_for += "all";
  }
  searching_for += ")";

  unsigned long interval = (unsigned long)((upper_limit - lower_limit) / 50.0);

  std::ofstream file;
  file.open(file_string);
  file << "The Fluffy Forager, v5" << std::endl << std::flush;

  std::cout << "Search criteria: " << searching_for << "; "
      << "limits: " << lower_limit << " <= # <= " << upper_limit << std::endl;
  file << "Search criteria: " << searching_for << "; "
      << "limits: " << lower_limit << " <= # <= " << upper_limit << std::endl << std::flush;
  if (interval > 30) {
    show_progress_bar = true;
  }
  if (show_progress_bar) {
    std::string pbar = " ** PLEASE WAIT - SEARCH IN PROGRESS **";
    for (int i = pbar.length(); i < 50; i++) {
      pbar += " ";
    }
    std::cout << " 0% |" << pbar << "| 100%" << std::endl;
    std::cout << "    |";
  }
  file << std::endl << "      #|  dvs|      |      |      |   sum|"
      << std::endl << std::endl << std::flush;

  clock_t start = clock ();

  bool mark = false;

  std::vector< triple> * test;

  for (unsigned long i = lower_limit; i <= upper_limit; i++) {
    test = check(i);
    if (test != NULL) {
      mark = true;
      for (int j = 0; j < (*test).size(); j++) {
        if (j == 0) {
          file << std::setfill(' ') << std::setw(7) << i;
          file << std::setfill(' ') << std::setw(6) << num_divisors;
        }
        else {
          file << "             ";
        }
        file << std::setfill(' ') << std::setw(7) << (*test)[j].a
            << std::setw(7) << (*test)[j].b << std::setw(7)
            << (*test)[j].c << std::flush; // prints triple elements
        if (j == 0) {
          file << std::setfill(' ') << std::setw(7) << (*test)[j].sum; // prints sum
        }
        file << std::endl;
      }
    }
    delete test;
    if (show_progress_bar && i % interval == interval - 1) {
      if (mark) {
        std::cout << "=";
        mark = false;
      }
      else {
        std::cout << "-";
      }
      std::cout << std::flush;
    }
  }
  if (show_progress_bar) {
    std::cout << "!" << std::endl;
  }
  std::string plural = count == 1 ? "" : "s";
  clock_t elapsed_time = ( clock() - start ) / CLOCKS_PER_SEC;
  std::cout << "Completed." << std::endl
      << std::to_string(elapsed_time) << " seconds elapsed. "
      << std::to_string(count) << " result" <<  plural << " saved to "
      << file_string << "." << std::endl;
  file << std::endl << std::to_string(elapsed_time) << " seconds elapsed. "
      << std::to_string(count) << " result" <<  plural << "." << std::endl;

  file.close();
  return 0;
}

std::vector< triple> * check(unsigned long candidate) {
  if (!candidate) {
    return NULL;
  }

  std::vector<unsigned long> divisors = factorization::deduce_divisors(candidate);
  num_divisors = divisors.size();
  // the first element of a triple must be equal to or less than the cube root of the candidate
  // the second must be equal to or less than the square root of the candidate over the first element
  // limits:
  // 1: 1 <= a <= cubeR(cand)
  // 2: 1 <= b <= sqR(cand / a)
  // 3: cubeR(cand) <= c <= cand

  if (num_divisors < 4) {
    return NULL;
  }

  std::vector<triple> matches;
  std::vector<triple> valid_matches;

  int cube_root_index, square_root_index;

  cube_root_index = square_root_index = 0;

  while (divisors[cube_root_index] <= pow(candidate, (1.0 / 3.0))) {
    cube_root_index++;
  }
  square_root_index = cube_root_index;
  while (divisors[square_root_index] <= pow(candidate, (0.5))) {
    square_root_index++;
  }

  for (int i = 0; i < cube_root_index; i++) {
    unsigned long a = divisors[i];
    int j_cutoff = pow(candidate / a, 0.5);
    for (int j = i; j < square_root_index; j++) {
      unsigned long b = divisors[j];
      if (divisors[j] > j_cutoff) {
        break;
      }
      for (int k = j; k < divisors.size(); k++) {
        unsigned long c = divisors[k];
        if (a * b * c > candidate) {
          break;
        }
        if ((a * b * c == candidate)) {
           triple new_triple = {a, b, c, a + b + c};
          matches.push_back(new_triple);
          break;
        }
      }
    }
  }
  bool flag = false;
  for (int i = 0; i < matches.size(); i++) {
    for (int j = i + 1; j < matches.size(); j++) {
      if (matches[i].sum == matches[j].sum) {
        if (flag) return NULL;
        valid_matches.push_back(matches[i]);
        valid_matches.push_back(matches[j]);
        flag = true;
      }
    }
  }
  if (!valid_matches.size()) return NULL;
  if (gamma_bound) {
    if (   valid_matches[0].a > gamma_bound
        || valid_matches[1].a > gamma_bound
        || valid_matches[0].b > gamma_bound
        || valid_matches[1].b > gamma_bound
        || valid_matches[0].c > gamma_bound
        || valid_matches[1].c > gamma_bound) {
      return NULL;
    }
  }
  if (!pure_only && (valid_matches[0].b == valid_matches[0].c
      && valid_matches[1].a != valid_matches[1].b)) {
    count++;
    return new std::vector<triple>(valid_matches);
  }
  else if (valid_matches[0].b == valid_matches[0].c
      && valid_matches[1].a == valid_matches[1].b) {
    count++;
    return new std::vector<triple>(valid_matches);
  }
  return NULL;
}
