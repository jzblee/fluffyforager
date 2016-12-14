#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include <vector>
#include "factorization.h"

struct triple {
unsigned long a;
unsigned long b;
unsigned long c;
unsigned long sum;
} ;

std::vector< triple> * check(unsigned long fluffy_number);

int count = 0, num_divisors = 0;
unsigned long gamma_bound = 0;
bool pure_only = false;

int main(int argc, char* argv[]) {
  std::string file_string = "output.txt";

  unsigned long lower_limit = 0, upper_limit = 0;
  bool show_progress_bar = false;

  std::string usage = "usage:\t" + std::string(argv[0]) + " [-p] [-g <bound>] lower-limit upper-limit\n"
  "\tsearch for pairs of triples of the form (a,a,b), (x,y,z)\n"
  "\twhere:\ta + a + b = x + y + z\n"
  "\t\ta + a + b and x + y + z are within the lower and upper limits\n"
  "\tp: only search for pure triple pairs, where y = z \n"
  "\tg: only search for triple pairs where a + a + b = x + y + z <= bound\n";

  if (argc == 1) {
    std::cout << usage;
    return 0;
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
  clock_t elapsed_time = ( clock() - start ) / CLOCKS_PER_SEC;
  std::cout << "Search completed in "
      << std::to_string(elapsed_time) << " seconds. "
      << std::to_string(count) << " " << searching_for << " found." << std::endl
      << "Results have been saved to " << file_string << "." << std::endl;
  file << std::endl << "Search completed in " << std::to_string(elapsed_time) << " seconds. "
      << std::to_string(count) << " " << searching_for << " found." << std::endl;

  file.close();
  return 0;
}

std::vector< triple> * check(unsigned long fluffy_number) {
  if (fluffy_number == 0) {
    return NULL;
  }

  std::vector<unsigned long> divisors = factorization::deduce_divisors(fluffy_number);
  num_divisors = divisors.size();

  if (num_divisors < 4) {
    return NULL;
  }

  std::vector<triple> * matches = new std::vector< triple>();
  std::vector<triple> * valid_matches = new std::vector< triple>();

  bool flag = false; // following code could use some more optimization
  for (int i = 0; i < divisors.size(); i++) {
    unsigned long a = divisors[i];
    for (int j = i; j < divisors.size(); j++) {
      unsigned long b = divisors[j];
      for (int k = j; k < divisors.size(); k++) {
        unsigned long c = divisors[k];
        if (a * b * c > fluffy_number) {
          break;
        }
        if ((a * b * c == fluffy_number)) {
           triple new_triple = {a, b, c, a + b + c};
          matches -> push_back(new_triple);
          break;
        }
      }
    }
  }
  flag = false;
  for (int i = 0; i < matches->size(); i++) {
    for (int j = 0; j < matches->size(); j++) {
      if ((i != j)
          && (((*matches)[i].sum == (*matches)[j].sum)
              && ((*matches)[i].a != (*matches)[j].a)
              && (*matches)[i].b != (*matches)[j].b)) {
        valid_matches -> push_back((*matches)[i]);
        flag = true;
      }
    }
  }
  delete matches;
  if (flag) {
    if (valid_matches -> size() == 2) {
      bool keep_sorting = true;
      while (keep_sorting) {
        keep_sorting = false;
        int i = 0;
        while (i < valid_matches -> size() - 1) {
          if ((*valid_matches)[i].sum > (*valid_matches)[i + 1].sum) {
             triple temp = (*valid_matches)[i];
            (*valid_matches)[i] = (*valid_matches)[i + 1];
            (*valid_matches)[i + 1] = temp;
            keep_sorting = true;
            i++;
          }
          else if ((*valid_matches)[i].a == (*valid_matches)[i + 1].a) {
            valid_matches -> erase(valid_matches -> begin() + i - 1);
          }
          else {
            i++;
          }
        }
      }
      if (gamma_bound != 0) {
        if ((*valid_matches)[0].a > gamma_bound
            || (*valid_matches)[0].b > gamma_bound
            || (*valid_matches)[0].c > gamma_bound
            || (*valid_matches)[1].a > gamma_bound
            || (*valid_matches)[1].b > gamma_bound
            || (*valid_matches)[1].c > gamma_bound) {
          delete valid_matches;
          return NULL;
        }
      }
      if (!pure_only && ((*valid_matches)[0].b == (*valid_matches)[0].c
          && (*valid_matches)[1].a != (*valid_matches)[1].b)) {
        count++;
        return valid_matches;
      }
      else if (((*valid_matches)[0].b == (*valid_matches)[0].c
          && (*valid_matches)[1].a == (*valid_matches)[1].b)) {
        count++;
        return valid_matches;
      }
      else {
        delete valid_matches;
        return NULL;
      }
    }
    else {
      delete valid_matches;
      return NULL;
    }
  }
  else {
    delete valid_matches;
    return NULL;
  }

  delete valid_matches;
  return NULL;
}
